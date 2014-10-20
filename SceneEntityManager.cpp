#include "SceneEntity.h"
#include "SceneEntityManager.h"
using namespace hiveCrowd::Kernel;

CSceneEntityManager::CSceneEntityManager( void )
{

}

CSceneEntityManager::~CSceneEntityManager( void )
{

}

SceneEntityPtr CSceneEntityManager::create( const Ogre::String& vName, const Ogre::String& vGroup )
{
	SceneEntityPtr NewEntity(_createImpl(vName, vGroup));
	_addImpl(NewEntity);

	return NewEntity;
}

void CSceneEntityManager::_addImpl( SceneEntityPtr vSceneEntityPtr )
{
	if (_findOrCreateEntityGroup(vSceneEntityPtr->getGroup()))
		m_SceneEntityMap.insert(SceneEntityMap::value_type(vSceneEntityPtr->getName(), vSceneEntityPtr));
	else
	{
		SceneEntityWithGroupMap::iterator GroupItr = m_SceneEntityWithGroupMap.find(vSceneEntityPtr->getGroup());
		if (GroupItr == m_SceneEntityWithGroupMap.end())
		{
			SceneEntityMap Dummy;
			m_SceneEntityWithGroupMap.insert(SceneEntityWithGroupMap::value_type(vSceneEntityPtr->getGroup(), Dummy));
			GroupItr = m_SceneEntityWithGroupMap.find(vSceneEntityPtr->getGroup());
		}
		GroupItr->second.insert(SceneEntityMap::value_type(vSceneEntityPtr->getName(), vSceneEntityPtr));
	}
}

bool CSceneEntityManager::_findOrCreateEntityGroup( const Ogre::String& vGroup ) const
{
	return m_SceneEntityWithGroupMap.find(vGroup) != m_SceneEntityWithGroupMap.end();
}
