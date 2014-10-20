#include "ObjectPoolManager.h"
#include "ObjectFactoryContainer.h"
using namespace hiveCrowd::Common;

CObjectPoolManager* CSingleton<CObjectPoolManager>::m_Singleton = nullptr;
boost::mutex CSingleton<CObjectPoolManager>::m_Mutex;

CObject* CObjectPoolManager::allocateObject(const Ogre::String& vSignature)
{
	CObject* NewObject					= CREATE_OBJECT(vSignature);
	CObjectPool<CObject>* ObjectPool	= nullptr;
	Ogre::String UpperSignature			= boost::algorithm::to_upper_copy(vSignature);
	if (m_ObjectPoolMap.count(UpperSignature) == 0)
	{
		ObjectPool						= new CObjectPool<CObject>();
		m_ObjectPoolMap[UpperSignature] = ObjectPool;
	}
	m_ObjectPoolMap[UpperSignature]->push(NewObject);

	return NewObject;
}

CObject* CObjectPoolManager::allocateObjects(const Ogre::String& vSignature, size_t vSize)
{
	CObject* NewObjects					= CREATE_OBJECT_ARRAY(vSignature, vSize);
	CObjectPool<CObject>* ObjectPool	= nullptr;
	Ogre::String UpperSignature			= boost::algorithm::to_upper_copy(vSignature);
	if (m_ObjectPoolMap.count(UpperSignature) == 0)
	{
		ObjectPool						= new CObjectPool<CObject>();
		m_ObjectPoolMap[UpperSignature] = ObjectPool;
	}
	ObjectPool = m_ObjectPoolMap[UpperSignature];
	for (size_t i=0 ; i<vSize ; ++i)
		ObjectPool->push(&NewObjects[i]);

	return NewObjects;
}

void CObjectPoolManager::freeObject(CObject* vObject)
{
	const Ogre::String UpperType(vObject->getType());
	const Ogre::String UpperName(vObject->getName());
	if (m_ObjectPoolMap.count(UpperType) == 0)
		return ;

	CObjectPool<CObject>& ObjectPool(*(m_ObjectPoolMap[UpperType]));
	if (ObjectPool.find(*vObject) != ObjectPool.end())
		SAFE_DELETE(vObject); // TODO: Fixed it!!!
}
