#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include "Common/ObjectFactory.h"
#include "Log.h"
using namespace hiveCrowd;

Common::CObjectFactory<LogSystem::CLog> TheCreator(typeid(LogSystem::CLog).name());
boost::recursive_mutex rmtx;

LogSystem::CLog::CLog() 
	: m_DebugOutput(true), m_SupressFileOutput(false), m_TimeStampOutput(true), m_LogName("HiveCrowdLog")
{

}

LogSystem::CLog::CLog(const Ogre::String& vName, bool vDebugOuput, bool vSupressFileOutput)
	: m_DebugOutput(vDebugOuput), m_SupressFileOutput(vSupressFileOutput), m_TimeStampOutput(true), m_LogName(vName)
{
	if (!vSupressFileOutput)
		m_LogStream.open(vName.c_str());
}

LogSystem::CLog::~CLog()
{
	// TODO:



}

void LogSystem::CLog::registerListener(LogSystem::CLogListener* vListener)
{
	boost::recursive_mutex::scoped_lock Locker(rmtx);

	if (std::find(m_LogListeners.begin(), m_LogListeners.end(), vListener) == m_LogListeners.end())
		m_LogListeners.push_back(vListener);
}

void LogSystem::CLog::unregisterListener(LogSystem::CLogListener* vListener)
{
	boost::recursive_mutex::scoped_lock Locker(rmtx);

	std::vector<LogSystem::CLogListener*>::iterator iter = std::find(m_LogListeners.begin(), m_LogListeners.end(), vListener);
	if (iter != m_LogListeners.end())
		m_LogListeners.erase(iter);
}

void LogSystem::CLog::logMessage(const Ogre::String& vMessage, LogSystem::LogLevel vLevel, bool vDebugMask)
{
	boost::recursive_mutex::scoped_lock Locker(rmtx);
	// TODO: 



}

LogSystem::CStream LogSystem::CLog::createStream(LogSystem::LogLevel vLevel /*= LOG_NORMAL_LEVEL*/, bool vDebugMask /*= false*/)
{
	return LogSystem::CStream(this, vLevel, vDebugMask);
}
