#pragma once
#include "CommonExport.h"
#include "Singleton.h"
#include "Object.h"
#include "ObjectPool.h"
namespace hiveCrowd
{
	namespace Common
	{
		class COMMON_DLL_API CObjectPoolManager : public CSingleton<CObjectPoolManager>
		{
		public:
			typedef Ogre::map<Ogre::String, CObjectPool<CObject>*>::type	ObjectPoolMap;
			typedef ObjectPoolMap::iterator									ObjectPoolMapIterator;
			typedef ObjectPoolMap::const_iterator							ObjectPoolMapConstIterator;

			CObjectPoolManager(void){}
			~CObjectPoolManager(void){}

			CObject* allocateObject(const Ogre::String& vSignature);
			void	 freeObject(CObject* vObject);
			CObject* allocateObjects(const Ogre::String& vSignature, size_t vSize);
		private:
			ObjectPoolMap m_ObjectPoolMap;
		}; 

#define ALLOCATE_OBJECT(vSignature) hiveCrowd::Common::CObjectPoolManager::getSingletonPtr()->allocateObject(vSignature)
#define ALLOCATE_OBJECT_ARRAY(vSignature, vSize) hiveCrowd::Common::CObjectPoolManager::getSingletonPtr()->allocateObjects(vSignature, vSize)
#define FREE_OBJECT(vObjectPtr) hiveCrowd::Common::CObjectPoolManager::getSingletonPtr()->freeObject(vObjectPtr)
	}
}
