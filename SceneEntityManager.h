#pragma once
#include "KernelExport.h"
#include "KernelPrerequisites.h"
#include "Common/ObjectPool.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class KERNEL_DLL_API CSceneEntityManager
		{
		public:
			typedef HashMap<Ogre::String, SceneEntityPtr> SceneEntityMap;
			typedef HashMap<Ogre::String, SceneEntityMap> SceneEntityWithGroupMap;

			explicit CSceneEntityManager(void);
			virtual ~CSceneEntityManager(void);

			SceneEntityPtr		 create(const Ogre::String& vName, const Ogre::String& vGroup);
			void				 remove(const SceneEntityPtr vEntity);
			void				 remove(const Ogre::String& vName);
			const SceneEntityPtr get(const Ogre::String& vName);
			SceneEntityPtr       fetch(const Ogre::String& vName);
		protected:
			void				  _addImpl(SceneEntityPtr vSceneEntityPtr);
			virtual CSceneEntity* _createImpl(const Ogre::String& vName, const Ogre::String& vGroup);
			bool				  _findOrCreateEntityGroup(const Ogre::String& vGroup) const;
		protected:
			 SceneEntityMap			 m_SceneEntityMap;
			 SceneEntityWithGroupMap m_SceneEntityWithGroupMap;
		};
	}
}
