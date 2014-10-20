#include <boost/algorithm/string.hpp>
#include "Agent.h"
#include "View.h"
#include "Brain.h"
#include "Script/Script.h"
#include "PathEngine/Path.h"
#include "PathEngine/AStarPathfindingInterface.h"
#include "PathEngine/AStarPathNode.h"
#include "Common/ObjectFactory.h"
using namespace hiveCrowd::Kernel;

hiveCrowd::Common::CObjectFactory<CAgent> TheCreator(typeid(CAgent).name());

CAgent::CAgent(void) : m_View(nullptr), m_Brain(nullptr)
{
	m_Type = typeid(*this).name();
}

CAgent::CAgent(const CAgent& vRhs) : m_View(vRhs.m_View), m_Property(vRhs.m_Property)
{
	m_Type = vRhs.m_Type;
}

CAgent::CAgent(const Ogre::String& vName, const Ogre::String& vGroup)
	: CSceneEntity(vName, vGroup), 
	  m_View(nullptr), 
	  m_Brain(nullptr)
{

}

CAgent& CAgent::operator=(const CAgent& vRhs)
{
	if (this != &vRhs)
	{
		// TODO: Delete the object accordingly!!!
		m_View	   = vRhs.m_View;
		m_Property = vRhs.m_Property;
	}

	return *this;
}

CAgent::~CAgent(void)
{
	SAFE_DELETE(m_View);
}

void CAgent::setBrain(const Ogre::String& vBrainName, const Ogre::String& vBrainGroup)
{

}

void CAgent::_update(Ogre::Real vDeltaTime)
{
	Ogre::Vector3 PrevPos   = m_Property.CurPos;
	Ogre::Vector3 TargetPos = m_Property.TargetPos;
	Ogre::Vector3 CurPos    = m_Property.CurPos + m_Property.CurLookAt*m_Property.NormalSpeed*vDeltaTime;
	if (CurPos.positionEquals(m_Property.SceneExit))
	{
		m_Property.CurPos = m_Property.SceneExit;
		m_Property.NormalSpeed = 0.0;
	}
	else if (!__isAdjacent(PrevPos, TargetPos))
	{
		m_Property.CurPos = CurPos;  
	}
	else
	{
		m_Property.CurPos = m_Property.TargetPos;
		hiveCrowd::PathEngine::CPath* Path = m_Property.PathMemory->findPath(m_Property.CurPos, m_Property.SceneExit);

		std::vector<Ogre::Vector3> PathNodePosList;
		std::vector<unsigned int> StraightPathPolyRefs;
		std::vector<unsigned char> StraightPathFlag;
		m_Property.PathMemory->findStraightPath(m_Property.CurPos, m_Property.SceneExit, *Path, PathNodePosList, StraightPathPolyRefs, StraightPathFlag);

		size_t Index = 1;
		size_t PathSize = PathNodePosList.size();
		Ogre::Vector3 NewTargetPos = PathNodePosList.at(0);

		while (__isAdjacent(NewTargetPos, m_Property.CurPos) && Index < PathSize)
		{
			NewTargetPos = PathNodePosList.at(Index ++);
		}
		
		if (Index == PathSize)
			NewTargetPos = m_Property.SceneExit;

		m_Property.TargetPos = NewTargetPos;

		this->__updateBodySmoothly(vDeltaTime);

		m_Property.PathMemory->reset();
		delete Path;
	}
}

void CAgent::__updateBodySmoothly(Ogre::Real vDeltaTime)
{
// 	//Ogre::Vector3 PrevSpeedDir = m_Property.CurLookAt;
// 	Ogre::Real CurDirLength    = (m_Property.TargetPos - m_Property.CurPos).length();
// 	m_Property.CurLookAt       = (m_Property.TargetPos - m_Property.CurPos)/CurDirLength;
// 
// 	Ogre::Quaternion ToGoal = PrevSpeedDir.getRotationTo(m_Property.CurLookAt);
// 	Ogre::Real YawToGoal    = ToGoal.getYaw().valueDegrees();
// 	Ogre::Real YawSpeed		= YawToGoal/std::abs(YawToGoal) * vDeltaTime * 250;
// 
// 	if (YawToGoal < 0)
// 		YawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(YawToGoal, YawSpeed));
// 	if (YawToGoal > 0)
// 		YawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(YawToGoal, YawSpeed));
// 
// 	m_SceneNode->yaw(Ogre::Degree(YawToGoal));
}

bool CAgent::__isAdjacent(const Ogre::Vector3& vCurPos, const Ogre::Vector3& vTargetPos)
{
	return vCurPos.positionEquals(vTargetPos);
}

hiveCrowd::Common::CObject* CAgentFactory::_createProductImpl(const Ogre::String& vName) const 
{

}

hiveCrowd::Common::CObject* CAgentFactory::_createProductImpl(const char* vName) const 
{

}
