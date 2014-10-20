#include <boost/algorithm/string.hpp>
#include "AgentManager.h"
#include "Agent.h"
#include "Common/ObjectFactory.h"
#include "PathEngine/Path.h"
#include "PathEngine/AStarPathNode.h"
#include "PathEngine/PathUtility.h"
#include "PathEngine/LocalBoundary.h"
using namespace hiveCrowd::Kernel;
using namespace hiveCrowd::PathEngine;

//************************************************************************************************
//FUNCTION:
CAgentManager::CAgentManager(void)
{
// 	m_Type = typeid(*this).name();
	m_ObstacleAvoidanceData = new CObstacleAvoidanceData;
	m_PathQueue				= new CPathQueue;
	m_ProximityGrid			= new CProximityGrid;
	m_ObstacleAvoQueryParam = new hiveCrowd::PathEngine::SObstacleAvoidanceParam;
	m_PathQueue->initialise(256);
	m_ProximityGrid->initialise(10, 1.0f);
	m_ObstacleAvoidanceData->initialise(8);
	m_ObstacleAvoQueryParam->VelBias = 0.4f;
	m_ObstacleAvoQueryParam->WeightDesiredVel = 2.0f;
	m_ObstacleAvoQueryParam->WeightCurrentVel = 0.75f;
	m_ObstacleAvoQueryParam->WeightSide		  = 0.75f;
	m_ObstacleAvoQueryParam->WeightToi		  = 2.5f;
	m_ObstacleAvoQueryParam->HorizTime		  = 2.5f;
	m_ObstacleAvoQueryParam->GridSize		  = 33;
	m_ObstacleAvoQueryParam->AdaptiveDivs	  = 7;
	m_ObstacleAvoQueryParam->AdaptiveRings	  = 2;
	m_ObstacleAvoQueryParam->AdaptiveDepth	  = 5;
}

//************************************************************************************************
//FUNCTION:
CAgentManager::CAgentManager(const Ogre::String& vName) : CSceneObjectManager(vName)
{
	m_ObstacleAvoidanceData = new CObstacleAvoidanceData;
	m_PathQueue				= new CPathQueue;
	m_ProximityGrid			= new CProximityGrid;
	m_ObstacleAvoQueryParam = new hiveCrowd::PathEngine::SObstacleAvoidanceParam;
	m_PathQueue->initialise(256);
	m_ProximityGrid->initialise(10, 1.0f);
	m_ObstacleAvoidanceData->initialise(8);
	m_ObstacleAvoQueryParam->VelBias = 0.4f;
	m_ObstacleAvoQueryParam->WeightDesiredVel = 2.0f;
	m_ObstacleAvoQueryParam->WeightCurrentVel = 0.75f;
	m_ObstacleAvoQueryParam->WeightSide		  = 0.75f;
	m_ObstacleAvoQueryParam->WeightToi		  = 2.5f;
	m_ObstacleAvoQueryParam->HorizTime		  = 2.5f;
	m_ObstacleAvoQueryParam->GridSize		  = 33;
	m_ObstacleAvoQueryParam->AdaptiveDivs	  = 7;
	m_ObstacleAvoQueryParam->AdaptiveRings	  = 2;
	m_ObstacleAvoQueryParam->AdaptiveDepth	  = 5;
}

//************************************************************************************************
//FUNCTION:
CAgentManager::CAgentManager(const Ogre::String& vName, Ogre::SceneManager* vSceneMgr) : CSceneObjectManager(vName, vSceneMgr)
{
	m_ObstacleAvoidanceData = new CObstacleAvoidanceData;
	m_PathQueue				= new CPathQueue;
	m_ProximityGrid			= new CProximityGrid;
	m_ObstacleAvoQueryParam = new hiveCrowd::PathEngine::SObstacleAvoidanceParam;
	m_PathQueue->initialise(256);
	m_ProximityGrid->initialise(10, 1.0f);
	m_ObstacleAvoidanceData->initialise(8);
	m_ObstacleAvoQueryParam->VelBias = 0.4f;
	m_ObstacleAvoQueryParam->WeightDesiredVel = 2.0f;
	m_ObstacleAvoQueryParam->WeightCurrentVel = 0.75f;
	m_ObstacleAvoQueryParam->WeightSide		  = 0.75f;
	m_ObstacleAvoQueryParam->WeightToi		  = 2.5f;
	m_ObstacleAvoQueryParam->HorizTime		  = 2.5f;
	m_ObstacleAvoQueryParam->GridSize		  = 33;
	m_ObstacleAvoQueryParam->AdaptiveDivs	  = 7;
	m_ObstacleAvoQueryParam->AdaptiveRings	  = 2;
	m_ObstacleAvoQueryParam->AdaptiveDepth	  = 5;
}

//************************************************************************************************
//FUNCTION:
CAgentManager::CAgentManager(const Ogre::String& vName, Ogre::InstanceManager* vInstanceMgr) : CSceneObjectManager(vName, vInstanceMgr)
{
	m_ObstacleAvoidanceData = new CObstacleAvoidanceData;
	m_PathQueue				= new CPathQueue;
	m_ProximityGrid			= new CProximityGrid;
	m_ObstacleAvoQueryParam = new hiveCrowd::PathEngine::SObstacleAvoidanceParam;

	m_PathQueue->initialise(256);
	m_ProximityGrid->initialise(10, 1.0f);
	m_ObstacleAvoidanceData->initialise(8);
	m_ObstacleAvoQueryParam->VelBias = 0.4f;
	m_ObstacleAvoQueryParam->WeightDesiredVel = 2.0f;
	m_ObstacleAvoQueryParam->WeightCurrentVel = 0.75f;
	m_ObstacleAvoQueryParam->WeightSide		  = 0.75f;
	m_ObstacleAvoQueryParam->WeightToi		  = 2.5f;
	m_ObstacleAvoQueryParam->HorizTime		  = 2.5f;
	m_ObstacleAvoQueryParam->GridSize		  = 33;
	m_ObstacleAvoQueryParam->AdaptiveDivs	  = 7;
	m_ObstacleAvoQueryParam->AdaptiveRings	  = 2;
	m_ObstacleAvoQueryParam->AdaptiveDepth	  = 5;
}

//************************************************************************************************
//FUNCTION:
CAgentManager::~CAgentManager()
{
	delete m_ObstacleAvoidanceData;
	delete m_PathQueue;
	delete m_ProximityGrid;
	delete m_ObstacleAvoQueryParam;
}

//************************************************************************************************
//FUNCTION:
int CAgentManager::getAgentIndex( CAgent* vAgent )
{
	_ASSERT(vAgent);

	for (unsigned int i=0 ; i<m_AgentGroup.size() ; ++i)
	{
		if (m_AgentGroup[i] == vAgent)
			return i;
	}

	return -1;
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::setMoveTarget(bool vAdjust)
{
	if (vAdjust)
	{
		for (unsigned int i=0 ; i<m_AgentGroup.size() ; ++i)
		{
			CAgent* Agent = m_AgentGroup[i];
			AgentProperty& Property = Agent->fetchProperty();
			if (Property.isActive == 0)
				continue;
			requestMoveVelocity(i, Property.CurVel);
		}
	}
	else
	{
		for (unsigned int i=0 ; i<m_AgentGroup.size() ; ++i)
		{
			CAgent* Agent = m_AgentGroup[i];
			AgentProperty& Property = Agent->fetchProperty();
			if (Property.isActive == 0)
				continue;
			requestMoveTarget(i, Property.TargetPos);
		}
	}
}

//************************************************************************************************
//FUNCTION:
bool CAgentManager::requestMoveTarget(unsigned int vAgentIndex, const Ogre::Vector3& vPos)
{
	if (vAgentIndex < 0 || vAgentIndex >= m_AgentGroup.size())
		return false;

	CAgent* Agent = m_AgentGroup[vAgentIndex];
	AgentProperty& Property = Agent->fetchProperty();
	Property.TargetPathQueueRef = HIVE_PATHQ_INVALID;
	Property.TargetReplan		= false;
	if (Property.TargetPolyRef)
		Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_REQUESTING;
	else
		Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_FAILED;

	return true;
}

//************************************************************************************************
//FUNCTION:
bool CAgentManager::requestMoveVelocity(unsigned int vAgentIndex, const Ogre::Vector3& vVel)
{
	if (vAgentIndex < 0 || vAgentIndex > m_AgentGroup.size())
		return false;
	
	CAgent* Agent = m_AgentGroup[vAgentIndex];
	AgentProperty& Property			= Agent->fetchProperty();
	Property.TargetPolyRef			= 0;
	Property.TargetPos				= vVel;
	Property.TargetPathQueueRef		= HIVE_PATHQ_INVALID;
	Property.TargetReplan			= false;
	Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_VELOCITY;

	return true;
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_checkPathValidity(Ogre::Real vDeltaTime)
{
	const int CHECK_LOOKHEAD = 10;
	const Ogre::Real TARGET_REPLAN_DELAY = 1.0;
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		AgentProperty& Property = m_AgentGroup[m_ActiveAgentIndexList[i]]->fetchProperty();
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
			continue;
		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_NONE || Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			continue;

		Property.TargetReplanTime += vDeltaTime;
		bool Replan = false;
		unsigned int AgentIndex  = m_ActiveAgentIndexList[i];
		Ogre::Vector3 AgentPos   = Property.CurPos;
		CAStarPathNode* PathNode = Property.Corridor->fetchPathNode(0);

		if (Property.TargetMoveRequestState != HIVE_CROWDAGENT_TARGET_NONE && Property.TargetMoveRequestState != HIVE_CROWDAGENT_TARGET_FAILED)
		{
			Ogre::Vector3 Nearest;

			//TOOD:
		}
	}
}

//************************************************************************************************
//FUNCTION:
int CAgentManager::__addToPathQueue( unsigned int vAgentIndex, unsigned int* vAgentIndexList, unsigned int vAgentCount, unsigned int vMaxPathAgent )
{
	CAgent* Agent = m_AgentGroup[vAgentIndex];
	AgentProperty& CurProperty  = Agent->fetchProperty();
	AgentProperty& LastProperty = m_AgentGroup[vAgentIndexList[vAgentCount-1]]->fetchProperty(); 

	int Slot = 0;
	if (!vAgentCount)
		Slot = vAgentCount;
	else if (CurProperty.TargetReplanTime <= LastProperty.TargetReplan)
		Slot = vAgentCount;
	else
	{
		int i = 0;
		for (; i<vAgentCount ; ++i)
		{
			AgentProperty& TempProperty = m_AgentGroup[m_ActiveAgentIndexList[i]]->fetchProperty();
			if (CurProperty.TargetReplanTime >= TempProperty.TargetReplan)
				break;
		}

		if (i+1 < vMaxPathAgent)
			memcpy(&vAgentIndexList[i+1], &vAgentIndexList[i], sizeof(unsigned int)*(vAgentCount-i));
		Slot = i;
	}

	vAgentIndexList[Slot] = vAgentIndex;

	return std::min(vAgentCount+1, vMaxPathAgent);
}

//************************************************************************************************
//FUNCTION:
void  CAgentManager::_updateMoveRequest()
{
	const int PATH_MAX_AGENTS = 8;
	unsigned int Queue[PATH_MAX_AGENTS];
	unsigned int QueueSize = 0;
	for (unsigned int i=0 ; i<m_AgentGroup.size() ; ++i)
	{
		AgentProperty& Property = m_AgentGroup[i]->fetchProperty();
		if (!Property.isActive)
			continue;
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_INVALID)
			continue;
		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_NONE || Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			continue;
		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_REQUESTING)
		{
			CPath* Path = Property.Corridor->fetchPath();
			unsigned int PathSize = Path->getPathSize();
			_ASSERT(PathSize);

			Ogre::Vector3 RequestPos = Ogre::Vector3::ZERO;
			CPath* NewPath = Property.PathMemory->findPath(Property.CurPos, Property.TargetPos);
			//Property.PathMemory->findSlicedPathParallel(Property.CurPos, Property.TargetPos, Property.TargetReplan, *Path, RequestPos, *NewPath);
			if (!NewPath->getPathSize())
			{
				RequestPos = Property.CurPos;
				NewPath->pushback(Path->fetchPathNode(0));
			}
			else
			{
				RequestPos = NewPath->getPathNode(NewPath->getPathSize()-1)->getNodePos();
			}

			Property.Corridor->setCorridor(RequestPos, NewPath);
			Property.Boundary->reset();
			delete Path;
			Property.PathMemory->reset();

			const CAStarPathNode* LastPathNode = NewPath->getPathNode(NewPath->getPathSize()-1);
			if (LastPathNode->getPolyID() == Property.TargetPolyRef)
			{
				Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_VALID;
				Property.TargetReplanTime = 0.0;
			}
			else
				Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_WAITTING_FOR_QUEUE;
		}

		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_WAITTING_FOR_QUEUE)
			QueueSize = __addToPathQueue(i, Queue, QueueSize, PATH_MAX_AGENTS);
	}

	for (unsigned int i=0 ; i<QueueSize ; ++i)
	{
		CAgent* Agent = m_AgentGroup[Queue[i]];
		AgentProperty& Property = Agent->fetchProperty();
		Property.TargetPathQueueRef = m_PathQueue->request(Property.Corridor->getPathPoly(0), Property.TargetPos, Property.TargetPolyRef, Property.TargetPos);
		if (Property.TargetPathQueueRef != HIVE_PATHQ_INVALID)
			Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_WAITTING_FOR_PATH;
	}

	m_PathQueue->update(HIVE_MAX_ITERS_PER_UPDATE);

	unsigned int State;
	//process path results.
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		if (!Property.isActive)
			continue;
		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_NONE || Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			continue;

		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_WAITTING_FOR_PATH)
		{
			State = m_PathQueue->getRequestState(Property.TargetMoveRequestState);
			if (State == HIVE_FAILURE)
			{
				Property.TargetPathQueueRef = HIVE_PATHQ_INVALID;
				if (Property.TargetPolyRef)
					Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_REQUESTING;
				else
					Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_FAILED;

				Property.TargetReplanTime = 0.0;
			}
			else if (State == HIVE_SUCCESS)
			{
				CPath* Path = Property.Corridor->fetchPath();
				unsigned int PathSize = Path->getPathSize();
				_ASSERT(Path->getPathSize());

				Ogre::Vector3 TargetPos = Property.TargetPos;
				CPath* PathResult = nullptr;
				bool Valid = true;
				State = m_PathQueue->getPathResult(Property.TargetPathQueueRef, PathResult);
				if (State == HIVE_FAILURE)
					Valid = false;
				if (Valid && Path->getPathNode(PathSize-1) != PathResult->getPathNode(0))
					Valid = false;

				if (Valid)
				{
					Property.Corridor->setCorridor(TargetPos, PathResult);
					Property.Boundary->reset();
					Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_VALID;
				}
				else
					Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_FAILED;

				Property.TargetReplanTime = 0.0;
			}
		}
	}
}

//************************************************************************************************
//FUNCTION:
int CAgentManager::__addToOptQueue(unsigned int vAgentIndex, unsigned int* vAgentIndexList, unsigned int vAgentCount, unsigned int vMaxPathAgent)
{
	int Slot = 0;
	if (!vAgentCount)
		Slot = vAgentCount;

	CAgent* Agent = m_AgentGroup[vAgentIndex];
	AgentProperty& CurProperty  = Agent->fetchProperty();
	AgentProperty& LastProperty = m_AgentGroup[vAgentIndexList[vAgentCount-1]]->fetchProperty(); 

	
	if (CurProperty.TopologyOptTime <= LastProperty.TopologyOptTime)
	{
		if (vAgentCount >= vMaxPathAgent)
			return vAgentCount;

		Slot = vAgentCount;
	}
	else
	{
		int i = 0;
		AgentProperty& TempProperty = m_AgentGroup[m_ActiveAgentIndexList[i]]->fetchProperty();
		for ( ; i<vAgentCount ; ++i)
		{
			if (CurProperty.TopologyOptTime >= TempProperty.TopologyOptTime)
				break;
		}

		if (i+1 < vMaxPathAgent)
			memcpy(&vAgentIndexList[i+1], &vAgentIndexList[i], sizeof(unsigned int)*(vAgentCount-i));
		Slot = i;
	}

	vAgentIndexList[Slot] = vAgentIndex;

	return std::min(vAgentCount+1, vMaxPathAgent);
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_updateTopologyOptimization(Ogre::Real vDeltaTime)
{
	if (!m_ActiveAgentIndexList.size())
		return ;

	const Ogre::Real HIVE_OPT_TIME_THR = 0.5f;
	const int HIVE_OPT_MAX_AGENTS = 1;
	unsigned int Queue[HIVE_OPT_MAX_AGENTS];
	unsigned int QueueSize = 0;

	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		if (Property.CurPolyState != HIVE_CROWDAGENT_STATE_WALKING)
			continue;
		if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_NONE || Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			continue;
		if ((Property.AgentParam->UpdateFlags & HIVE_CROWD_OPTIMIZE_TOPO) == 0)
			continue;

		Property.TopologyOptTime += vDeltaTime;
		if (Property.TopologyOptTime >= HIVE_OPT_TIME_THR)
			QueueSize = __addToOptQueue(i, Queue, QueueSize, HIVE_OPT_MAX_AGENTS);
	}

	for (unsigned int i=0 ; i<QueueSize ; ++i)
	{
		CAgent* Agent = m_AgentGroup[Queue[i]];
		AgentProperty& Property = Agent->fetchProperty();
		Property.Corridor->optimizePathTopolopy(Property.PathMemory);
		Property.TopologyOptTime = 0;
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_updateActiveAgentList()
{
	_ASSERT(m_AgentGroup.size());

	for (unsigned int i=0 ; i<m_AgentGroup.size() ; ++i)
	{
		if (m_AgentGroup[i]->getProperty().isActive)
		{
			m_ActiveAgentIndexList.push_back(i);
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__registerProximityGrid()
{
	m_ProximityGrid->clear();
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		const Ogre::Vector3 Pos = Property.CurPos;
		const Ogre::Real Radius = Property.AgentParam->Radius;
		m_ProximityGrid->addItem(i, Pos[0]-Radius, Pos[2]-Radius, Pos[0]+Radius, Pos[2]+Radius);
	}
}

//************************************************************************************************
//FUNCTION:
Ogre::Real CAgentManager::__calcPathLength( const CAgent* vAgent, const Ogre::Vector3& vStartPos, const Ogre::Vector3& vTargetPos )
{
	CAStarPathfindingInterface* PathFinder = vAgent->getProperty().PathMemory;
	CPath* Path = PathFinder->findPath(vStartPos, vTargetPos);

	unsigned int PathSize = Path->getPathSize();
	if (PathSize == 0)
		return 0.0;

	Ogre::Real    PathLength      = 0.0f;
	Ogre::Vector3 CurPathNodePos  = Ogre::Vector3::ZERO;
	Ogre::Vector3 PrevPathNodePos = vStartPos;
	for (unsigned int i=1 ; i<PathSize ; ++i)
	{
		const CAStarPathNode* PathNode = Path->getPathNode(i);
		CurPathNodePos = PathNode->getNodePos();
		PathLength += CurPathNodePos.distance(PrevPathNodePos);
		PrevPathNodePos = CurPathNodePos;
	}

	PathFinder->reset();
	return PathLength;
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_addNeighbor( CAgent* vAgent, unsigned int vNeighborAgentIndex, Ogre::Real vDist2Neighbor )
{
	AgentProperty& Property = vAgent->fetchProperty();
	Ogre::vector<unsigned int>::type& NeiAgentRefList = Property.InfluenceSpace.Neighbors;
	Ogre::vector<Ogre::Real>::type& NeiAgentDistList  = Property.InfluenceSpace.NeighborDistList;
	unsigned int NeighborSize = NeiAgentRefList.size();
	if (NeighborSize == 0)
	{
		NeiAgentRefList.push_back(vNeighborAgentIndex);
		NeiAgentDistList.push_back(vDist2Neighbor);
	}
	else if (vDist2Neighbor >= NeiAgentDistList[NeighborSize-1])
	{
		NeiAgentRefList.push_back(vNeighborAgentIndex);
		NeiAgentDistList.push_back(vDist2Neighbor);
	}
	else
	{
		unsigned int i = 0;
		for (; i<NeighborSize ; ++i)
		{
			if (vDist2Neighbor <= NeiAgentDistList[i])
				break;
		}

		memmove(&NeiAgentDistList[i+1], &NeiAgentDistList[i], sizeof(Ogre::Real)*(NeighborSize-1-i));
		memmove(&NeiAgentRefList[i+1], &NeiAgentRefList[i], sizeof(unsigned int)*(NeighborSize-1-i));

		NeiAgentRefList[i]  = vNeighborAgentIndex;
		NeiAgentDistList[i] = vDist2Neighbor;
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_dumpNeighborInfo( const Ogre::Vector3& vPos, Ogre::Real vAgentHeight, Ogre::Real vNeighborSearchRange, const CAgent* vSkipAgent )
{
	const int MAX_NEIGHBORS = 256;
	std::vector<short> IndexList;
	IndexList.reserve(MAX_NEIGHBORS);

	int Count = m_ProximityGrid->queryItems(vPos[0]-vNeighborSearchRange, vPos[2], vPos[0]+vNeighborSearchRange, vPos[2]+2*vNeighborSearchRange, IndexList);

	for (unsigned int i=0 ;  i<IndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[IndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		if (Agent != vSkipAgent)
		{
			Ogre::Vector3 Diff = vPos - Property.CurPos;
			if (std::fabsf(Diff[1]) < (vAgentHeight + Property.AgentParam->Height)/2.0f)
			{
				Diff[1] = 0;
				Ogre::Real SqrDist = Diff.squaredLength();
				if (SqrDist <= Ogre::Math::Sqr(vNeighborSearchRange))
				{
					Ogre::Real Dist2Neighbor = __calcPathLength(Agent, vPos, Property.CurPos);
					const Ogre::Real PATHESPILON = 4.8;
					if (Ogre::Math::Sqr(Dist2Neighbor) <= SqrDist + Ogre::Math::Sqr(PATHESPILON))
					{
						_addNeighbor(Agent, IndexList[i], Dist2Neighbor);
					}
				}
			}
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__updateNeighborMessage()
{
	const size_t SCALE = 2000;
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
		{
			const Ogre::Real UpdateThreshold = Property.AgentParam->CollosionQueryRange*0.25f;
			if (Property.CurPos.squaredDistance(Property.Boundary->getCenter()) > Ogre::Math::Sqr(UpdateThreshold))
				Property.Boundary->update((Property.Corridor)->getPathPoly(0), Property.CurPos, Property.AgentParam->CollosionQueryRange, Property.PathMemory);

			_dumpNeighborInfo(Property.CurPos, Property.AgentParam->Height, Property.AgentParam->NeighborSearchDistance, Agent);
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__calcSmoothSteerDirection( const CAgent* vAgent, Ogre::Vector3& voDirection )
{
	_ASSERT(vAgent);

	const AgentProperty& Property = vAgent->getProperty();
	CPathCorridor* Corridor = Property.Corridor;
	unsigned int CornerSize = Corridor->getCorridor().size();
	if (!CornerSize)
	{
		voDirection = Ogre::Vector3::ZERO;
		return ;
	}

	const Ogre::Vector3 PointA = Corridor->getCorner(0);
	const Ogre::Vector3 PointB = Corridor->getCorner(std::min((unsigned int)1, CornerSize-1));

	Ogre::Vector3 DirectionA = PointA - Property.CurPos;
	Ogre::Vector3 DirectionB = PointB - Property.CurPos;
	DirectionA[1] = 0.0;
	DirectionB[1] = 0.0;

	Ogre::Real LengthA = DirectionA.length();
	Ogre::Real LengthB = DirectionB.length();
	if (LengthB > 0.001f)
		DirectionB = DirectionB/LengthB;

	voDirection = DirectionA - DirectionB*LengthA*0.5f;
	voDirection.normalise();
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__calcStraightSteerDirection( const CAgent* vAgent, Ogre::Vector3& voDirection )
{
	_ASSERT(vAgent);

	const AgentProperty& Property = vAgent->getProperty();
	CPathCorridor* Corridor = Property.Corridor;
	unsigned int CornerSize = Corridor->getCorridor().size();
	if (!CornerSize)
	{
		voDirection = Ogre::Vector3::ZERO;
		return ;
	}

	voDirection = Corridor->getCorner(0) - Property.CurPos;
	voDirection[1] = 0.0;
	voDirection.normalise();
}

//************************************************************************************************
//FUNCTION:
Ogre::Real CAgentManager::__getDistanceToGoal(const CAgent* vAgent, Ogre::Real vRadius)
{
	_ASSERT(vAgent);

	const AgentProperty& Property = vAgent->getProperty();
	CPathCorridor* Corridor = Property.Corridor;
	unsigned int CornerSize = Corridor->getCorridor().size();
	if (!CornerSize)
		return vRadius;

	const bool isEndOfPath = (Property.CornerFlag[CornerSize-1] & HIVE_PATH_ENGINE_STRAIGHTPATH_END) ? true : false;
	if (isEndOfPath)
		return std::min(Property.CurPos.distance(Corridor->getCorner(CornerSize-1)), vRadius);

	return vRadius;
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__calculateSteering()
{
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		AgentProperty& Property = Agent->fetchProperty();
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING && Property.TargetMoveRequestState != HIVE_CROWDAGENT_TARGET_NONE)
		{
			Ogre::Vector3 DesiredVel = Ogre::Vector3::ZERO;
			if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			{
				DesiredVel = Property.TargetPos;
				Property.DesiredSpeed = Property.TargetPos.length();
			}
			else
			{
				if (Property.AgentParam->UpdateFlags & HIVE_CROWD_ANTICIPATE_TURNS)
					__calcSmoothSteerDirection(Agent, DesiredVel);
				else
					__calcStraightSteerDirection(Agent, DesiredVel);

				Ogre::Real SlowDownRadius = Property.AgentParam->Radius * 2;
				Ogre::Real SpeedScale	  = __getDistanceToGoal(Agent, SlowDownRadius)/SlowDownRadius;
				SpeedScale = 1.0;
				Property.DesiredSpeed = Property.AgentParam->MaxSpeed;
				DesiredVel = DesiredVel*Property.DesiredSpeed/SpeedScale;
			}

			if (Property.AgentParam->UpdateFlags & HIVE_CROWD_SEPARATION)
			{
				Ogre::Real SeparationDist	 = Property.AgentParam->CollosionQueryRange;
				Ogre::Real InvSeparationDist = 1.0f/SeparationDist;
				Ogre::Real SeparationWeight  = Property.AgentParam->SeparationWeight;
				Ogre::Real W = 0.0f;
				Ogre::Vector3 DisPos = Ogre::Vector3::ZERO;
				Ogre::vector<unsigned int>::type NeiAgentIndexList = Property.InfluenceSpace.Neighbors;
				for (unsigned int i=0 ; i<NeiAgentIndexList.size() ; ++i)
				{
					CAgent* NeiAgent = m_AgentGroup[m_ActiveAgentIndexList[i]];
					AgentProperty& NeiProperty = NeiAgent->fetchProperty();
					Ogre::Vector3 Diff = Property.CurPos - NeiProperty.CurPos;
					Diff[1] = 0;
					Ogre::Real SqrDist = Diff.squaredLength();
					if (SqrDist < 0.00001f)
						continue ;
					if (SqrDist > Ogre::Math::Sqr(SeparationDist))
						continue ;

					const Ogre::Real Dist   = Ogre::Math::Sqrt(SqrDist);
					const Ogre::Real Weight = SeparationWeight * (1.0f - Ogre::Math::Sqr(Dist*InvSeparationDist));
					DisPos = DisPos + Diff*Weight/Dist;
					W += 1.0f;
				}

				if (W > 0.0001f)
				{
					DesiredVel = DesiredVel + DisPos/W;
					const Ogre::Real SqrSpeed = DesiredVel.squaredLength();
					const Ogre::Real SqrDesired = Ogre::Math::Sqr(Property.DesiredSpeed);
					if (SqrSpeed > SqrDesired)
						DesiredVel = DesiredVel*SqrDesired/SqrSpeed;
				}
			}

			Property.DesiredVel = DesiredVel;
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::_update( Ogre::Real vDeltaTime )
{
	vDeltaTime = 0.1f;

	m_VelocitySampleCount = 0;

	_updateActiveAgentList();
	//_checkPathValidity(vDeltaTime);
	_updateMoveRequest();
	_updateTopologyOptimization(vDeltaTime);

	__registerProximityGrid();
	__updateNeighborMessage();

	for (SceneObjectGroupIterator itr=m_SceneObjectGroup.begin() ; itr!=m_SceneObjectGroup.end() ; ++itr)
	{
		hiveCrowd::Common::CObjectPool<CSceneObject>& SceneObjectPool(*(itr->second));
		const size_t NumSceneObject(SceneObjectPool.size());
		for (size_t i=0 ; i<NumSceneObject ; ++i)
		{
			CAgent& Agent(dynamic_cast<CAgent&>(SceneObjectPool[i]));
			Agent._update(vDeltaTime);
		}
	}

	__calculateSteering();
	__makeVelocityPlan();

	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		CAgent* Agent = m_AgentGroup[m_ActiveAgentIndexList[i]];
		if (Agent->getProperty().CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
			__integrate(Agent, vDeltaTime);
	}

	__handleCollisions();
	///////////////////////////////////////////////////////////////Debug Here  
	for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
	{
		AgentProperty& Property = m_AgentGroup[m_ActiveAgentIndexList[i]]->fetchProperty();
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
		{
			Property.Corridor->moveStartToPosition(Property.CurPos, Property.PathMemory);
			Property.CurPos = Property.Corridor->getCurPos();
			if (Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_NONE || Property.TargetMoveRequestState == HIVE_CROWDAGENT_TARGET_VELOCITY)
			{
				CAStarPathNode* PathNode = Property.Corridor->fetchPathNode(0);
				Property.Corridor->reset(Property.CurPos, PathNode);
			}
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__makeVelocityPlan()
{
	m_VelocitySampleCount = 0;

	unsigned int ActiveAgentCount = m_ActiveAgentIndexList.size();
	_ASSERT(ActiveAgentCount);

	const int MAX_CIRCLE_COUNT  = 6;
	const int MAX_SEGMENT_COUNT = 8;
	for (unsigned int i=0 ; i<ActiveAgentCount ; ++i)
	{
		CAgent* Agent = m_AgentGroup[i];
		AgentProperty& Property = Agent->fetchProperty();
		if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
		{
			if (Property.AgentParam->UpdateFlags & HIVE_CROWD_OBSTACLE_AVOIDANCE)
			{
				CObstacleAvoidanceQuery OAQuery;
				OAQuery.initialise(MAX_CIRCLE_COUNT, MAX_SEGMENT_COUNT);
				AgentProperty& CurProperty = Agent->fetchProperty();
				Ogre::vector<unsigned int>::type CurAgentNeiList = Agent->getProperty().InfluenceSpace.Neighbors;
				for (unsigned int k=0 ; k<CurAgentNeiList.size() ; ++k)
				{
					const AgentProperty& NeiAgent = m_AgentGroup[CurAgentNeiList.at(k)]->getProperty();
					OAQuery.addCircle(NeiAgent.CurPos, NeiAgent.AgentParam->Radius, NeiAgent.CurVel, NeiAgent.DesiredVel);
				}

				for (unsigned int k=0 ; k<CurProperty.Boundary->getSegmentCount() ; ++k)
				{
					const Ogre::Vector3& FirstSegEnd  = CurProperty.Boundary->getSegment(k).first;
					const Ogre::Vector3& SecondSegEnd = CurProperty.Boundary->getSegment(k).second;
					if (calcTriangleArea2D(CurProperty.CurPos, FirstSegEnd, SecondSegEnd))
						continue ;

					OAQuery.addSegment(FirstSegEnd, SecondSegEnd);
				}

				bool Adaptive = true;
				int  Count	  = 0;
				if (Adaptive)
					Count = OAQuery.sampleVelocityAdaptive(CurProperty.CurPos, CurProperty.AgentParam->Radius, CurProperty.DesiredSpeed, CurProperty.CurVel, CurProperty.DesiredVel, CurProperty.NewVel, m_ObstacleAvoQueryParam, m_ObstacleAvoidanceData);
				else
					Count = OAQuery.sampleVelocityGrid(CurProperty.CurPos, CurProperty.AgentParam->Radius, CurProperty.DesiredSpeed, CurProperty.CurVel, CurProperty.DesiredVel, CurProperty.NewVel, m_ObstacleAvoQueryParam, m_ObstacleAvoidanceData);

				m_VelocitySampleCount += Count;
			}
			else
				Property.CurVel = Property.DesiredVel;
		}
	}
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__integrate(CAgent* vAgent, Ogre::Real vDeltaTime)
{
	AgentProperty& Property = vAgent->fetchProperty();
	const Ogre::Real MaxDelta = Property.AgentParam->MaxAcceleration * vDeltaTime;
	Ogre::Vector3 DeltaVel = Property.NewVel - Property.CurVel;
	Ogre::Real DelatVelLength = DeltaVel.length();
	if (DelatVelLength > MaxDelta)
		DeltaVel = DeltaVel*MaxDelta/DelatVelLength;
	Property.CurVel = Property.CurVel + DeltaVel;

	if (Property.CurVel.length() > 0.0001f)
		Property.CurPos = Property.CurPos + Property.CurVel*vDeltaTime;
	else
		Property.CurVel = Ogre::Vector3::ZERO;
}

//************************************************************************************************
//FUNCTION:
void CAgentManager::__handleCollisions()
{
	//All the agent index is order in m_AgentGroup.
	const Ogre::Real COLLISION_RESOLVE_FACTOR = 0.7f;
	for (int iter=0 ; iter<4 ; ++iter)
	{
		for (int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
		{
			unsigned int AgentIndex = m_ActiveAgentIndexList[i];
			AgentProperty& Property = m_AgentGroup[AgentIndex]->fetchProperty();
			if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
			{
				Property.DisPos = Ogre::Vector3::ZERO;
				Ogre::Real W = 0.0f;
				for (int k=0 ; k<Property.InfluenceSpace.Neighbors.size() ; ++k)
				{
					unsigned int NeiAgentIndex = Property.InfluenceSpace.Neighbors[i];
					AgentProperty& NeiAgentProperty = m_AgentGroup[NeiAgentIndex]->fetchProperty();
					Ogre::Vector3 Diff = Property.CurPos - NeiAgentProperty.CurPos;
					Diff[1] = 0;
					Ogre::Real Dist = Diff.squaredLength();
					if (Dist <= Ogre::Math::Sqr(Property.AgentParam->Radius + NeiAgentProperty.AgentParam->Radius))
					{
						Dist = Ogre::Math::Sqr(Dist);
						Ogre::Real Penalty = (Property.AgentParam->Radius + NeiAgentProperty.AgentParam->Radius) - Dist;
						if (Dist < 0.0001f)
						{
							if (AgentIndex > NeiAgentIndex)
								Diff = Ogre::Vector3(-Property.DesiredVel[2], 0, Property.DesiredVel[0]);
							else
								Diff = Ogre::Vector3(Property.DesiredVel[2], 0, -Property.DesiredVel[0]);

							Penalty = 0.0f;
						}
						else
							Penalty = (1.0f/Dist*Penalty*0.5f*COLLISION_RESOLVE_FACTOR);

						Property.DisPos = Property.DisPos + Diff*Penalty;
						W += 1.0f;
					}
				}

				if (W > 0.0001f)
					Property.DisPos = Property.DisPos/W;
			}
		}

		for (unsigned int i=0 ; i<m_ActiveAgentIndexList.size() ; ++i)
		{
			AgentProperty& Property = m_AgentGroup[m_ActiveAgentIndexList[i]]->fetchProperty();
			if (Property.CurPolyState == HIVE_CROWDAGENT_STATE_WALKING)
				Property.CurPos = Property.CurPos + Property.DisPos;
		}
	}
}
