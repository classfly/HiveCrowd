#include "Log.h"
#include "LogManager.h"
#include "Common/ObjectPoolManager.h"
using namespace hiveCrowd;

LogSystem::CLog* LogSystem::CLogManager::createLog(const Ogre::String& vName, bool vDefaultLog /*= false*/, bool vDebuggerOutput /*= true*/, bool vSuppressFileOutput /*= false*/)
{
	return dynamic_cast<LogSystem::CLog*>(Common::CObjectPoolManager::getSingletonPtr()->allocateObject(vName));
}
