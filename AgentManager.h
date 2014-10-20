#pragma once
#include "KernelExport.h"
#include "PathEngine/ProximityGrid.h"
#include "PathEngine/PathQueue.h"
#include "PathEngine/ObstacleAvoidance.h"
#include "SceneEntityManager.h"
namespace hiveCrowd
{
	namespace Script{ class CScript; }
	namespace Kernel
	{
		class CAgent;
		class SObstacleAvoidanceParam;
		class KERNEL_DLL_API CAgentManager : public CSceneEntityManager
		{
		public:
			explicit CAgentManager(void);
			explicit CAgentManager(const Ogre::String& vName);
			explicit CAgentManager(const Ogre::String& vName, Ogre::SceneManager* vSceneMgr);
			explicit CAgentManager(const Ogre::String& vName, Ogre::InstanceManager* vInstanceMgr);
			~CAgentManager(void);

			int	 getAgentIndex(CAgent* vAgent);
			void setMoveTarget(bool vAdjust = false);
			bool requestMoveTarget(unsigned int vAgentIndex, const Ogre::Vector3& vPos);
			bool requestMoveVelocity(unsigned int vAgentIndex, const Ogre::Vector3& vVel);
		protected:
			void _update(Ogre::Real vDeltaTime);
			void _updateActiveAgentList();
			void _updateMoveRequest();
			void _checkPathValidity(Ogre::Real vDeltaTime);
			void _updateTopologyOptimization(Ogre::Real vDeltaTime);
			bool _requestMoveTargetReplan(unsigned int vIndex, unsigned int vPolyRef, Ogre::Vector3& vPos);
			void _dumpNeighborInfo(const Ogre::Vector3& vPos, Ogre::Real vAgentHeight, Ogre::Real vNeighborSearchRange, const CAgent* vSkipAgent);
			void _addNeighbor(CAgent* vAgent, unsigned int vNeighborAgentIndex, Ogre::Real vDist2Neighbor);
		private:
			void __updateCrowd(Ogre::Real vDeltaTime);
			void __makeVelocityPlan();
			void __registerProximityGrid();
			void __updateNeighborMessage();
			void __calculateSteering();
			void __integrate(CAgent* vAgent, Ogre::Real vDeltaTime);
			void __handleCollisions();
			int  __addToPathQueue(unsigned int vAgentIndex, unsigned int* vAgentIndexList, unsigned int vAgentCount, unsigned int vMaxPathAgent);
			int  __addToOptQueue(unsigned int vAgentIndex, unsigned int* vAgentIndexList, unsigned int vAgentCount, unsigned int vMaxPathAgent);
			void __calcSmoothSteerDirection(const CAgent* vAgent, Ogre::Vector3& voDirection);
			void __calcStraightSteerDirection(const CAgent* vAgent, Ogre::Vector3& voDirection);
			Ogre::Real __getDistanceToGoal(const CAgent* vAgent, Ogre::Real vRadius);
			Ogre::Real __calcPathLength(const CAgent* vAgent, const Ogre::Vector3& vStartPos, const Ogre::Vector3& vTargetPos);

			int m_VelocitySampleCount;

			std::vector<AgentPtr>						    m_AgentGroup;
			std::vector<unsigned int>						m_ActiveAgentIndexList;
			hiveCrowd::PathEngine::CPathQueue*				m_PathQueue;
			hiveCrowd::PathEngine::CProximityGrid*			m_ProximityGrid;
			hiveCrowd::PathEngine::CObstacleAvoidanceData*  m_ObstacleAvoidanceData;
			hiveCrowd::PathEngine::SObstacleAvoidanceParam* m_ObstacleAvoQueryParam;

			friend class CScene;
		};
	}
}
