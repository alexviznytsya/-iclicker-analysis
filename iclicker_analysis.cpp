/*main.cpp*/

//
// iClicker Analysis
//
// Alex Viznytsya
// Fall 2017
//


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "iclicker.h"

// Check if filename exist, and if so return true:
bool isFileExist(std::string filename)
{
	std::ifstream file(filename);
	return file.good();
}

// Process master file and return vector of file names of available sessions:
std::vector<std::string> getSessionFileNames(const std::string masterFile)
{
	std::vector<std::string> sessionFileNames;
	std::string fileLine;
	if (isFileExist(masterFile))
	{
		// Process each line inside of master file:
		std::ifstream file(masterFile);
		while (std::getline(file, fileLine))
		{
			// Check if line has more than 4 characres (.xml), and if so
			// add to the vector of session names:
			if (fileLine.size() > 4)
			{
				sessionFileNames.push_back(fileLine);
			}
		}
	}
    else
    {
        std::cout << "Error: Cannot find " << masterFile << std::endl;
    }
	return sessionFileNames;
}

// Extract XML attribute from XML tag. It does not check if attribute exist inside
// the XML tag. So, passed attribute name exist:
std::string extractXMLAttribute(const std::string &line, const std::string &attribute)
{
	int attributeBegin = line.find(attribute) + attribute.length() + 2;
	int attributeEnd = line.find("\"", attributeBegin);
	return line.substr(attributeBegin, (attributeEnd - attributeBegin));
}

void parseSessionXMLFile(const std::string filename, std::map<std::string, IClicker> &iclickers, std::vector<Session> &sessions)
{
	// If filename does not exist, skip this file:
	if (!isFileExist(filename))
	{
        std::cout << "Error: Cannot open " << filename << std::endl;
		return;
	}

	// Otherwise:

	std::cout << ">>Parsing '" << filename << "'..." << std::endl;

	// Extract time and date form file name:
	std::string tSessionDate = "" + filename.substr(3, 2) + "/" + filename.substr(5, 2) + "/" + filename.substr(1, 2);
	std::string tSessionTime = "" + filename.substr(7, 2) + ":" + filename.substr(9, 2);

	// Open xml file:
	std::ifstream file(filename);

	// Cotainer for single line from xml file:
	std::string fileLine = "";

	// Get the line where session begins:
	while (std::getline(file, fileLine) && fileLine.find("<ssn") == std::string::npos)
	{
		std::getline(file, fileLine);
	}

	// Get name of the session:
	std::string tSessionName = extractXMLAttribute(fileLine, " ssnn");

	// Create temporary sessio container:
	Session tSession(tSessionName, tSessionDate, tSessionTime);

	// Create semporary set for all iClicker devices:
	std::set<std::string> clickerSet;

	// Loop through <ssn .../> tag:
	while (std::getline(file, fileLine) && fileLine.find("</ssn") == std::string::npos)
	{
		if (fileLine.find("<p") >= 0)
		{
			// Extract correct answer from qusetion:
			std::string tCorrectAnswer = extractXMLAttribute(fileLine, " cans");

			// Update question count:
			tSession.setNumberOfQuestions(tSession.getNumberOfQuestions() + 1);

			// Create temporary session container for iClicker session:
			Session tClickerSession(tSessionName, tSessionDate, tSessionTime);
			tClickerSession.setNumberOfClickers(1);

			// Loop through question <p> tags:
			while (getline(file, fileLine) && fileLine.find("</p>") == std::string::npos)
			{
				// Process each answer <v.../> record:
				if (fileLine.find("<v") >= 0)
				{
					// Get student's clicker ID and answer and save them to temp variables:
					std::string tClickerID = extractXMLAttribute(fileLine, " id").erase(0, 1);
					clickerSet.insert(tClickerID);
					std::string tStudentAnswer = extractXMLAttribute(fileLine, " ans");
					std::vector<Session>::iterator clickerSessionIterator;

					// Check if iclider id exists in iclickers map, and if not, then add it to the map:
					std::map<std::string, IClicker>::iterator clickerFindResult = iclickers.find(tClickerID);
					if (clickerFindResult == iclickers.end())
					{
						iclickers.insert(std::pair<std::string, IClicker>(tClickerID, IClicker(tClickerID)));
						iclickers[tClickerID].addSession(tClickerSession);
					}

					// Check if iclicker device has session in its record, if not, then add it:
					if (!iclickers[tClickerID].isSessionExist(tSessionName))
					{
						iclickers[tClickerID].addSession(tClickerSession);
					}

					// Ger current session from iclicker record:
					clickerSessionIterator = iclickers[tClickerID].getLastSession();
					clickerSessionIterator->setNumberOfQuestions(clickerSessionIterator->getNumberOfQuestions() + 1);
					// Check if clicker device sent any answer, and if so, update its record:
					if (tStudentAnswer.size() > 0)
					{
						tSession.setNumberOfAnswers(tSession.getNumberOfAnswers() + 1);
						clickerSessionIterator->setNumberOfAnswers(clickerSessionIterator->getNumberOfAnswers() + 1);
						if (tCorrectAnswer.find(tStudentAnswer) != std::string::npos)
						{
							tSession.setNumberOfCorrectAnswers(tSession.getNumberOfCorrectAnswers() + 1);
							clickerSessionIterator->setNumberOfCorrectAnswers(clickerSessionIterator->getNumberOfCorrectAnswers() + 1);
						}
					}
				}
			}
		}
	}

	// Update count for number of iClickers in the session:
	tSession.setNumberOfClickers(clickerSet.size());

	// Check if session exist in sessions vector. If not, then add it:
	auto sessionSearchResult = std::find_if(sessions.begin(), sessions.end(),
		[&](Session &s)
	{
		return s.getSessionName() == tSession.getSessionName();
	}
	);
	if (sessionSearchResult == sessions.end())
	{
		sessions.push_back(tSession);
	}
}

int main()
{
	std::string masterFileName = "files.txt";
	std::vector<std::string> sessionFileNames;
	std::map<std::string, IClicker> iclickers;
	std::vector<Session> sessions;

	std::cout << "**Starting**" << std::endl;

	// Process xml file from master file:
	sessionFileNames = getSessionFileNames(masterFileName);
	for (std::string &filename : sessionFileNames)
	{
		parseSessionXMLFile(filename, iclickers, sessions);
	}

	std::cout << std::endl;
	std::cout << "**Class Analysis**" << std::endl;
	std::cout << ">>Total sessions:  " << sessions.size() << std::endl;

	int totalQuestionInSessions = 0;
	for (Session &s : sessions)
	{
		totalQuestionInSessions += s.getNumberOfQuestions();
	}

	std::cout << ">>Total questions: " << totalQuestionInSessions << std::endl;
	std::cout << ">>Answered:" << std::endl;
	for (Session &s : sessions)
	{
		int numOfQuestions = s.getNumberOfQuestions();
		int numOfClickers = s.getNumberOfClickers();
		int numOfAnswers = s.getNumberOfAnswers();
		double percentage = ((double)numOfAnswers / (numOfQuestions * numOfClickers)) * 100;
		std::cout << "  \"" << s.getSessionName() << "\": " << percentage << "% (" << numOfQuestions
			<< " questions, " << numOfClickers << " clickers, " << numOfAnswers << " answers)" << std::endl;
	}
	std::cout << ">>Correctly:" << std::endl;
	for (Session &s : sessions)
	{
		int numOfQuestions = s.getNumberOfQuestions();
		int numOfClickers = s.getNumberOfClickers();
		int numOfCorrectAnswers = s.getNumberOfCorrectAnswers();
		double percentage = ((double)numOfCorrectAnswers / (numOfQuestions * numOfClickers)) * 100;
		std::cout << "  \"" << s.getSessionName() << "\": " << percentage << "% (" << numOfQuestions
			<< " questions, " << numOfClickers << " clickers, " << numOfCorrectAnswers << " correct answers)" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "**Student Analysis**" << std::endl;
	std::cout << ">> Enter a clicker id (# to quit): ";

	std::string userInput = "";
	std::getline(std::cin, userInput);
	while (userInput != "#")
	{
		std::map<std::string, IClicker>::iterator searchResult = iclickers.find(userInput);
		if (searchResult != iclickers.end()) {
			std::cout << std::endl;
			for (Session s : searchResult->second.getDeviceSeession())
			{
				int numOfQuestions = s.getNumberOfQuestions();
				int numOfAnswers = s.getNumberOfAnswers();
				int numOfCorrectAnswers = s.getNumberOfCorrectAnswers();
				double percentage = ((double)numOfCorrectAnswers / numOfQuestions) * 100;
				std::cout << "  \"" << s.getSessionName() << "\": " << numOfAnswers << " out of "
					<< numOfQuestions << " answered, " << percentage << "% correctly" << std::endl;
			}
		}
		else
		{
			std::cout << std::endl;
			std::cout << "** not found..." << std::endl;
		}
		std::cout << std::endl;
		std::cout << ">> Enter a clicker id (# to quit): ";
		std::getline(std::cin, userInput);
	}

	std::cout << std::endl;
	std::cout << "**END**" << std::endl;
	return 0;
}

