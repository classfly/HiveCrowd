#pragma once
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <OgreCommon.h>
#include "KernelExport.h"
#include "Common/Object.h"
#include "Common/Subject.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class CSceneEntity;
		class CSceneEntityManager;
		class KERNEL_DLL_API CScene : public Common::CObject, public Common::CSubject
		{
		public:
			explicit CScene(const Ogre::String& vName, Ogre::SceneManager* vSceneMgr, Ogre::InstanceManager* vInstanceMgr = nullptr);
			~CScene(void){}

			CSceneEntity*		createEntity(const Ogre::String& vName, const Ogre::NameValuePairList& vParams);
			CSceneEntity*		createEntity(const Ogre::String& vName, const Ogre::String& vTypeName, const Ogre::NameValuePairList& vParams);
			void				removeEntity(const Ogre::String& vName);
			void				removeEntity(const Ogre::String& vName, const Ogre::String& vTypeName);
			const CSceneEntity* getEntity(const Ogre::String& vName) const;
		private:
			void __update(Ogre::Real vDeltaTime);
		private:
			boost::mutex		   m_Mutex;
			CSceneEntityManager*   m_SceneEntityMgr;
			Ogre::SceneManager*	   m_SceneMgr;
			Ogre::InstanceManager* m_InstanceMgr;
		};
	}
}
