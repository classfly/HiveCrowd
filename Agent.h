#pragma once
#include "SceneEntity.h"
#include "Geometry/GeometricShape.h"
#include "PathEngine/LocalBoundary.h"
#include "PathEngine/AStarPathCorridor.h"
#include "PathEngine/AStarPathfindingInterface.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		const int HIVE_CROWD_MAX_NEIGHBORS = 6;
		const int HIVE_CROWD_FIXED_NEIGHBOR = 6;
		const int HIVE_CROWDAGENT_MAX_CORNERS = 4;
		const int HIVE_CROWD_MAX_OBSTAVOIDANCE_PRAMAS = 8;

		enum ECrowdAgentState
		{
			HIVE_CROWDAGENT_STATE_INVALID,
			HIVE_CROWDAGENT_STATE_WALKING,
		};

		enum ECrowdAgentType
		{
			EXPERIENCED_AGENT,
			UNEXPERIENCED_AGENT,
		};

		enum EMoveRequestState
		{
			HIVE_CROWDAGENT_TARGET_NONE = 0,
			HIVE_CROWDAGENT_TARGET_FAILED,
			HIVE_CROWDAGENT_TARGET_VALID,
			HIVE_CROWDAGENT_TARGET_REQUESTING,
			HIVE_CROWDAGENT_TARGET_WAITTING_FOR_QUEUE,
			HIVE_CROWDAGENT_TARGET_WAITTING_FOR_PATH,
			HIVE_CROWDAGENT_TARGET_VELOCITY,
		};

		enum EUpdateFlags
		{
			HIVE_CROWD_ANTICIPATE_TURNS = 1,
			HIVE_CROWD_OBSTACLE_AVOIDANCE = 2,
			HIVE_CROWD_SEPARATION = 4,
			HIVE_CROWD_OPTIMIZE_VIS = 8,
			HIVE_CROWD_OPTIMIZE_TOPO = 16,
		};

		struct SAgentParam
		{
			ECrowdAgentType AgentType;
			Ogre::Real Radius;
			Ogre::Real Height;
			Ogre::Real MaxAcceleration;
			Ogre::Real MaxSpeed;

			Ogre::Real SocialFactor;
			Ogre::Real IndividualFactor;
			Ogre::Real PsychologicalTendency;
			Ogre::Real DistacneAttenubation;

			Ogre::Real CollosionQueryRange;
			Ogre::Real NeighborSearchDistance;
			Ogre::Real PathOptimizationRange;
			Ogre::Real SeparationWeight;

			unsigned char UpdateFlags;
			unsigned char ObstacleAvoidanceType;
		};

		struct SCrowdAgentAnimation
		{
			unsigned char Active;
			Ogre::Vector3 InitPos, StartPos, EndPos;
			unsigned int  PolyRef;
			Ogre::Real    Temp, TempMax; //TODO: rename;
		};

		struct AgentProperty : public SceneObjectProperty
		{
			typedef struct NeighborHood
			{
				NeighborHood(void)
				{
					NeighborArea		= nullptr;
					NeighborAreaDensity = 0.0f;
				}
				~NeighborHood(void)
				{
					SAFE_DELETE(NeighborArea);
				}

				Ogre::vector<unsigned int>::type Neighbors;
				Ogre::vector<Ogre::Real>::type	 NeighborDistList;
				Geometry::CGeometricShape*		 NeighborArea;
				Ogre::Real						 NeighborAreaDensity;
			}IS;
			
			hiveCrowd::PathEngine::CAStarPathfindingInterface* PathMemory;
			Ogre::Real	    NormalSpeed, HurrySpeed, MaxSpeed, MaxAcceleration, Speed;
			Ogre::Real	    DensityThreshold;
			Ogre::Vector3   CurDirection, TargetPos;
			Ogre::Vector3   SceneExit;
			
			//CrowdAgentData:
			hiveCrowd::PathEngine::CLocalBoundary* Boundary;
			hiveCrowd::PathEngine::CPathCorridor*  Corridor;
			Ogre::Real	  DesiredSpeed;
			Ogre::Real    TopologyOptTime;
			Ogre::Real	  TargetReplanTime;
			Ogre::Vector3 DisPos;
			Ogre::Vector3 CurVel;
			Ogre::Vector3 NewVel;
			Ogre::Vector3 DesiredVel;
			unsigned int  TargetPolyRef;
			unsigned int  TargetPathQueueRef;
			SAgentParam*  AgentParam;
			bool		  isActive;
			bool		  TargetReplan;
			IS			  InfluenceSpace;

			unsigned char CurPolyState;
			unsigned char TargetMoveRequestState;

			unsigned char CornerFlag[HIVE_CROWDAGENT_MAX_CORNERS];
		};

		class CView;
		class CBrain;
		class CAgentManager;
		class KERNEL_DLL_API CAgent : public CSceneEntity
		{
		public:
			explicit CAgent(void);
			explicit CAgent(const Ogre::String& vName, const Ogre::String& vGroup);
			explicit CAgent(const CAgent& vRhs);
			CAgent&  operator=(const CAgent& vRhs);
			~CAgent(void);

			void setBrain(const Ogre::String& vBrainName, const Ogre::String& vBrainGroup);
		protected:
			void _update(Ogre::Real vDeltaTime) override;
		private:
			void __updateView(void);
			void __updateDecision(void);
			void __updateBodySmoothly(Ogre::Real vDeltaTime);
			bool __isAdjacent(const Ogre::Vector3& vCurPos, const Ogre::Vector3& vTargetPos);
		private:
			CView*		  m_View;
			CBrain*		  m_Brain;

			friend class CAgentManager;
			friend class CAgentFactory;
		};

		class KERNEL_DLL_API CAgentFactory : public Common::CObjectFactory<Common::CObject>
		{
		public:
			CAgentFactory(void){}
			virtual ~CAgentFactory(void){}
		protected:
			Common::CObject* _createProductImpl(const Ogre::String& vName) const override;
			Common::CObject* _createProductImpl(const char* vName)		   const override;
		};
	}
}
