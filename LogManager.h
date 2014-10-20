#pragma once
#include <OgrePrerequisites.h>
#include "LogExport.h"
#include "Common/ObjectPool.h"
namespace hiveCrowd
{
	namespace LogSystem
	{
		class CLog;
		class LOG_DLL_API CLogManager
		{
		public:
			CLogManager(){}
			~CLogManager(){}

			CLog* createLog( const Ogre::String& vName, bool vDefaultLog = false, bool vDebuggerOutput = true, bool vSuppressFileOutput = false);
		};
	}
}
