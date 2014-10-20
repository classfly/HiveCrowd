#include <boost/algorithm/string.hpp>
#include "SceneEntity.h"
#include "Scene.h"
using namespace hiveCrowd::Kernel;

CSceneEntity::CSceneEntity(void) : hiveCrowd::Common::CObject(),
								   m_NumAnimation(0), 
								   m_SceneNode(nullptr), 
								   m_AnimationStateSet(nullptr)
{
	m_Type = typeid(*this).name();
}

CSceneEntity::CSceneEntity(const Ogre::String& vName, const Ogre::String& vGroup)
	: hiveCrowd::Common::CObject(vName), 
	  m_NumAnimation(0), 
	  m_SceneNode(nullptr), 
	  m_AnimationStateSet(nullptr)
{
	m_Type = typeid(*this).name();
}

void CSceneEntity::_fadeAnimations(Ogre::Real vDeltaTime)
{
	Ogre::AnimationStateIterator AnimationItr(m_AnimationStateSet->getAnimationStateIterator());
	size_t AnimationIndex = 0;
	while (AnimationItr.hasMoreElements())
	{
		Ogre::AnimationState* AnimationState(AnimationItr.getNext());
		Ogre::Real NewAnimationWeight(0.0f);
		if (m_AnimationFadeInGroup[AnimationIndex])
		{
			NewAnimationWeight = AnimationState->getWeight() + vDeltaTime;
			AnimationState->setWeight(Ogre::Math::Clamp<Ogre::Real>(NewAnimationWeight, 0.0f, 1.0f));
			if (NewAnimationWeight > 1.0f)
				m_AnimationFadeInGroup[AnimationIndex] = true;
		}
		else if (m_AnimationFadeOutGroup[AnimationIndex])
		{
			NewAnimationWeight = AnimationState->getWeight() - vDeltaTime;
			AnimationState->setWeight(Ogre::Math::Clamp<Ogre::Real>(NewAnimationWeight, 0.0f, 1.0f));
			if (NewAnimationWeight < 0.0f)
			{
				AnimationState->setEnabled(false);
				m_AnimationFadeOutGroup[AnimationIndex] = false;
			}
		}
	}
}

CSceneEntity* CSceneEntity::_clone(const Ogre::String& vName)
{
	return nullptr;
}

size_t CSceneEntity::getTypeFlag(void) const
{
	return (m_Creator == nullptr ? 0xFFFFFFFF : m_Creator->getTypeFlag());
}

// void CSceneEntity::load(const Ogre::String& vMeshSignature, 
// 						const Ogre::String& vNameSignature,  
// 						Ogre::SceneManager* vSceneMgr, 
// 						Ogre::InstanceManager* vInstanceMgr)
// {
// 	const Ogre::String& UppperNameSignature(vNameSignature);
// 	const Ogre::String& UpperMeshSignature(vMeshSignature);
// 	if (m_SceneNode != nullptr)
// 		vSceneMgr->getRootSceneNode()->removeChild(UppperNameSignature);
// 	m_SceneNode = vSceneMgr->getRootSceneNode()->createChildSceneNode((vInstanceMgr == nullptr ? UppperNameSignature : UpperMeshSignature));
// 	_ASSERT(m_SceneNode != nullptr);
// 	vInstanceMgr == nullptr ? _loadImplNormal(UpperMeshSignature, vSceneMgr) : _loadImplInstanced(UpperMeshSignature, vInstanceMgr);
// }

// void CSceneObject::_loadImplNormal(const Ogre::String& vMeshSignature, Ogre::SceneManager* vSceneMgr)
// {
// 	m_NormalEntity = vSceneMgr->createEntity(m_SceneNode->getName(), vMeshSignature);
// 	m_SceneNode->attachObject(m_NormalEntity);
// }

// void CSceneObject::_loadImplInstanced(const Ogre::String& vMeshSignature, Ogre::InstanceManager* vInstanceMgr)
// {
// 	m_InstancedEntity = vInstanceMgr->createInstancedEntity(m_SceneNode->getName());
// 	m_SceneNode->attachObject(m_InstancedEntity);
// }
