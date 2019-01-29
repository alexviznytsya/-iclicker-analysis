#pragma once

class Session 
{
	private:
		std::string sessionName;
		std::string sessionDate;
		std::string sessionTime;
		int numberOfQuestions;
		int numberOfClickers;
		int numberOfAnswers;
		int numberOfCorrectAnswers;

	public:
		Session(std::string name, std::string date, std::string time)
		{
			this->sessionName = name;
			this->sessionDate = date;
			this->sessionTime = time;
			this->numberOfQuestions = 0;
			this->numberOfClickers = 0;
			this->numberOfAnswers = 0;
			this->numberOfCorrectAnswers = 0;
		}

		std::string getSessionName()
		{
			return this->sessionName;
		}

		void setSessionName(std::string name)
		{
			this->sessionName = name;
		}

		std::string getSessionDate()
		{
			return this->sessionDate;
		}

		void setSessionDate(std::string date)
		{
			this->sessionDate = date;
		}

		std::string getSessionTime()
		{
			return this->sessionTime;
		}

		void setSessionTime(std::string time)
		{
			this->sessionTime = time;
		}	

		int getNumberOfQuestions()
		{
			return this->numberOfQuestions;
		}

		void setNumberOfQuestions(int num)
		{
			this->numberOfQuestions = num;
		}

		int getNumberOfClickers()
		{
			return this->numberOfClickers;
		}

		void setNumberOfClickers(int num)
		{
			this->numberOfClickers = num;
		}

		int getNumberOfAnswers()
		{
			return this->numberOfAnswers;
		}

		void setNumberOfAnswers(int num)
		{
			this->numberOfAnswers = num;
		}

		int getNumberOfCorrectAnswers()
		{
			return this->numberOfCorrectAnswers;
		}

		void setNumberOfCorrectAnswers(int num)
		{
			this->numberOfCorrectAnswers = num;
		}

};