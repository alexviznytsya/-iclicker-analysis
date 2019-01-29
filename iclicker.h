#pragma once
#include "session.h"

class IClicker
{
	private:
		std::string deviceID;
		std::vector<Session> deviceSessions;

	public:
		IClicker(std::string id) {
			this->deviceID = id;
		}
		IClicker() 
		{
		}
	std::string getDeviceID()
	{
		return this->deviceID;
	}

	void setDeviceID(std::string dName)
	{
		this->deviceID = dName;
	}

	std::vector<Session> getDeviceSeession()
	{
		return this->deviceSessions;
	}

	void setDeviceSessions(std::vector<Session> sessions)
	{
		this->deviceSessions = sessions;
	}

	void addSession(Session session) {
		this->deviceSessions.push_back(session);
	}

	std::vector<Session>::iterator getLastSession() {
		return this->deviceSessions.end() - 1;
	}

	bool isSessionExist(std::string sName) {
		auto searchResult = find_if(this->deviceSessions.begin(), this->deviceSessions.end(),
			[&](Session &s)
			{
			return s.getSessionName() == sName;
			}
		);
		return !(searchResult == this->deviceSessions.end());

	}
};