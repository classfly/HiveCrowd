#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include "Common/ObjectPool.h"
#include "NormalSimulation.h"
#include "InputDeviceModule.h"
#include "Scene.h"
#include "SceneInfoSpaceManager.h"
#include "Agent.h"
#include "AnimSceneObject.h"
#include "AgentManager.h"
#include "StaticSceneObject.h"
#include "PathEngine/Path.h"
#include "PathEngine/AStarPathNode.h"
#include "PathEngine/AStarPathfindingInterface.h"
#include "RandomSelectNodeController.h"
#include "SequenceNodeController.h"
#include "ConditionNodeController.h"
#include "ActionNodeController.h"
#include "SelectNodeProperty.h"
#include "SequenceNodeProperty.h"
#include "ConditionNodeProperty.h"
#include "ActionNodeProperty.h"
#include "MoveAwayCrowd.h"
#include "SlowDownModule.h"
#include "ActionSignature.h"
using namespace hiveCrowd::PathEngine;
using namespace hiveCrowd::Kernel;

const size_t SCENE_SHADOW_SIZE = 1 << 9;
const size_t SCENE_SHADOW_FAR_DISTANCE = 1 << 10;
const size_t MAX_PATH_SIZE = 512;

CNormalSimulation::CNormalSimulation(void) : m_Root(nullptr), 
											 m_IsShutDown(false), 
											 m_RenderWindow(nullptr), 
											 m_OverlaySystem(nullptr), 
											 m_SimulationSceneMgr(nullptr), 
											 m_InputDeviceModule(nullptr)
{
	m_Enter = Ogre::Vector3(-57, 0.0, 58.3333359);
	m_Exit  = Ogre::Vector3(60, 0.0, -50);

	m_BehaviorTree = new CBehaviorTree;
	m_NavMap	   = new CNavMap;
	m_PathMemory   = new CAStarPathfindingInterface;
	m_PathMemory->initialize(m_NavMap, MAX_PATH_SIZE);
}

CNormalSimulation::~CNormalSimulation(void)
{
	delete m_PathMemory;
	delete m_NavMap;
	delete m_BehaviorTree;

	if (m_InputDeviceModule)
		m_InputDeviceModule->destroy();
	m_SimulationSceneMgr->clearScene();
	if (m_Root)
		m_Root->destroySceneManager(m_SimulationSceneMgr);
	m_RenderWindow->destroy();
	m_Root->shutdown();
}

void CNormalSimulation::startSimulation(void)
{
	if (_setup() == false)
		return ;

	unsigned long LastFrameTime		 = 0;
	unsigned long CurrentFrameTime   = 0;
	unsigned long TimeSinceLastFrame = 0;
	Ogre::Timer* Timer = m_Root->getTimer();
	while (m_IsShutDown == false)
	{
		CurrentFrameTime   = Timer->getMilliseconds();
		TimeSinceLastFrame = CurrentFrameTime - LastFrameTime;
		LastFrameTime	   = CurrentFrameTime;
		if (m_RenderWindow->getStatistics().lastFPS > 30.0f)
			Sleep(20);
		m_InputDeviceModule->captureEvents();

		m_Scene->update(TimeSinceLastFrame);
		
		m_Root->renderOneFrame();
		Ogre::WindowEventUtilities::messagePump();
	}
}

bool CNormalSimulation::frameRenderingQueued(const	Ogre::FrameEvent& vEvent)
{
	return m_SdkCameraMan->frameRenderingQueued(vEvent);
}

bool CNormalSimulation::_setup(void)
{
	m_Root = new Ogre::Root();
	if (_configure() == false)
		return false;

	_createSceneManager();
	_createCamera();
	_loadResources();
	_createListener();
	_createScene();
	_createBehaviorTree();
	_createCrowd();
	
	return true;
}

bool CNormalSimulation::_configure(void)
{
	if (m_Root->restoreConfig() || m_Root->showConfigDialog())
	{
		m_RenderWindow = m_Root->initialise(true, typeid(*this).name());
		
		return true;
	}

	return false;
}

bool CNormalSimulation::_loadResources(void)
{
	Ogre::ConfigFile CfgFile;
	CfgFile.load("resources.cfg");
	Ogre::ConfigFile::SectionIterator SectionItr = CfgFile.getSectionIterator();
	Ogre::String SectionName, TypeName, ArchiveName;
	try
	{
		while (SectionItr.hasMoreElements())
		{
			SectionName = SectionItr.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = SectionItr.getNext();
			for (Ogre::ConfigFile::SettingsMultiMap::iterator SettingItr = settings->begin() ; 
				 SettingItr!=settings->end() ; 
				 ++SettingItr)
			{
				TypeName	= SettingItr->first;
				ArchiveName = SettingItr->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ArchiveName, TypeName, SectionName);
			}
		}
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		return true;
	}
	catch (boost::exception* e)
	{
		// TODO:
	}

	return false;
}

void CNormalSimulation::_createSceneManager(void)
{
	m_SimulationSceneMgr = m_Root->createSceneManager(Ogre::ST_GENERIC);
	m_OverlaySystem		 = new Ogre::OverlaySystem();
	m_SimulationSceneMgr->addRenderQueueListener(m_OverlaySystem);
	m_SimulationSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	m_SimulationSceneMgr->setShadowTextureSize(SCENE_SHADOW_SIZE);
	m_SimulationSceneMgr->setShadowColour(Ogre::ColourValue::Blue);
	m_SimulationSceneMgr->setShadowFarDistance(SCENE_SHADOW_FAR_DISTANCE);
	m_SimulationSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
}

void CNormalSimulation::_createListener(void)
{
	m_InputDeviceModule = new CInputDeviceModule();
	_ASSERT(m_InputDeviceModule != nullptr);
	m_InputDeviceModule->initialize(m_RenderWindow, false);
	m_InputDeviceModule->setMouseEventCallback(this);
	m_InputDeviceModule->setKeryboardEventCallback(this);
	
	_ASSERT(m_RenderWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(m_RenderWindow, this);

	m_InputContext.mMouse    = m_InputDeviceModule->fetchMouseDevice();
	m_InputContext.mKeyboard = m_InputDeviceModule->fetchKeyboardDevice();
	m_SdkTrayMgr			 = new OgreBites::SdkTrayManager(typeid(*this).name(), m_RenderWindow, m_InputContext, this);

	m_Root->addFrameListener(this);
}

void CNormalSimulation::_createCamera(void)
{
	_ASSERT(m_SimulationSceneMgr);
	m_Camera = m_SimulationSceneMgr->createCamera(typeid(*this).name());
	m_Camera->setPosition(Ogre::Vector3(0.0, 60, -60));
	m_Camera->lookAt(Ogre::Vector3::NEGATIVE_UNIT_Z);
	m_Camera->setNearClipDistance(0.1);

	Ogre::Viewport* ViewPort = m_RenderWindow->addViewport(m_Camera);
	ViewPort->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	m_Camera->setAspectRatio(Ogre::Real(ViewPort->getActualWidth()) / Ogre::Real(ViewPort->getActualHeight()));

	m_SdkCameraMan = new OgreBites::SdkCameraMan(m_Camera);
	m_SdkCameraMan->setTopSpeed(20);
}

void CNormalSimulation::windowResized(Ogre::RenderWindow* vRenderWindow)
{
	size_t WindowWidth(0), WindowHeight(0), WindowDepth(0);
	int WindowLeftCorner(0), WindowTopCorner(0);
	vRenderWindow->getMetrics(WindowWidth, WindowHeight, WindowDepth, WindowLeftCorner, WindowTopCorner);
	const OIS::MouseState& CurMouseState(m_InputDeviceModule->getMouseDevice()->getMouseState());
	CurMouseState.width  = WindowWidth;
	CurMouseState.height = WindowHeight;
	m_InputDeviceModule->updateMouseExtents(WindowWidth, WindowHeight);
}

void CNormalSimulation::windowClosed(Ogre::RenderWindow* vRenderWindow)
{
	if (vRenderWindow == m_RenderWindow)
	{
		if (m_InputDeviceModule)
			m_InputDeviceModule->destroy();
	}
}

bool CNormalSimulation::mouseMoved(const OIS::MouseEvent& vMouseArg)
{
	m_SdkCameraMan->injectMouseMove(vMouseArg);

	return true;
}

bool CNormalSimulation::mousePressed(const OIS::MouseEvent& vMouseArg, OIS::MouseButtonID vID)
{
	m_SdkCameraMan->injectMouseDown(vMouseArg, vID);

	return true;
}

bool CNormalSimulation::mouseReleased(const OIS::MouseEvent& vMouseArg, OIS::MouseButtonID vID)
{
	m_SdkCameraMan->injectMouseUp(vMouseArg, vID);

	return true;
}

bool CNormalSimulation::keyPressed(const OIS::KeyEvent& vKeyArg)
{
	if (vKeyArg.key == OIS::KC_ESCAPE)
		m_IsShutDown = true;
	m_SdkCameraMan->injectKeyDown(vKeyArg);

	return true;
}

bool CNormalSimulation::keyReleased(const OIS::KeyEvent& vKeyArg)
{
	m_SdkCameraMan->injectKeyUp(vKeyArg);

	return true;
}

void CNormalSimulation::_drawMesh(void)
{
	const Ogre::Real MESH_DRAW_OFFSET = 0.5f;
	m_TestTriSceneNode  = m_SimulationSceneMgr->getRootSceneNode()->createChildSceneNode("TestTriangle");

	const std::vector<hiveCrowd::STriangle*>& TriangleList = m_NavMap->getTriList();
	unsigned int TriSize = TriangleList.size();
	_ASSERT(TriSize);

	Ogre::ColourValue* RegionColors = nullptr;
	RegionColors = new Ogre::ColourValue[TriSize];
	for (unsigned int i=0 ; i<TriSize ; ++i)
	{
		RegionColors[i] = Ogre::ColourValue(Ogre::Math::RangeRandom(0, 1), Ogre::Math::RangeRandom(0, 1), Ogre::Math::RangeRandom(0, 1), 1);
	}

	if (TriSize)
	{
		m_TestMOWalk = m_SimulationSceneMgr->createManualObject("TestMOWalk");
		m_TestMOWalk->begin("recastdebug", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		int Index = 0;
		for (unsigned int i=0 ; i<TriSize ; ++i)
		{
			hiveCrowd::STriangle* Triangle = TriangleList.at(i);
			Ogre::Vector3 PointA = m_NavMap->getNavPolyVertex(Triangle->PointA);
			m_TestMOWalk->position(PointA.x, PointA.y + MESH_DRAW_OFFSET, PointA.z);
			m_TestMOWalk->colour(RegionColors[i]);
			Ogre::Vector3 PointB = m_NavMap->getNavPolyVertex(Triangle->PointB);
			m_TestMOWalk->position(PointB.x, PointB.y + MESH_DRAW_OFFSET, PointB.z);
			m_TestMOWalk->colour(RegionColors[i]);
			Ogre::Vector3 PointC = m_NavMap->getNavPolyVertex(Triangle->PointC);
			m_TestMOWalk->position(PointC.x, PointC.y + MESH_DRAW_OFFSET, PointC.z);
			m_TestMOWalk->colour(RegionColors[i]);

			m_TestMOWalk->triangle(Index, Index+1, Index+2);
			Index += 3;
		}
		m_TestMOWalk->end();

		m_TestTriSceneNode->attachObject(m_TestMOWalk);
	}

	if (RegionColors)
		delete[] RegionColors;
}

void CNormalSimulation::_createScene(void)
{
	m_Scene = new CScene(typeid(*this).name(), m_SimulationSceneMgr);
	m_Scene->add("Maze3.mesh", typeid(CStaticSceneObject).name(), "TerrainNode");
	_drawMesh();
}

void CNormalSimulation::_createCrowd(void)
{
	const unsigned int CROWD_AGENT_SCALE = 10;
	for (unsigned int i=0 ; i<CROWD_AGENT_SCALE ; ++i)
	{
		m_Scene->add("NPC_ESF_TOWN_05.MESH", typeid(CAgent).name(), "Agent" + Ogre::StringConverter::toString(i));
	}

	CSceneObjectManager& SceneObjMgr = m_Scene->fetchSceneObjectManager(typeid(CAgent).name() + Ogre::String("MANAGER"));
	Ogre::map<Ogre::String, Common::CObjectPool<CSceneObject>*>::type& SceneObjGroup = SceneObjMgr.fetchGroup(typeid(CAgent).name());
	for (unsigned int i=0 ; i<CROWD_AGENT_SCALE ; ++i)
	{
		Common::CObjectPool<CSceneObject>& ScenePool = *(SceneObjGroup[boost::algorithm::to_upper_copy("AGENT" + Ogre::StringConverter::toString(i))]);
		CAgent* Agent = dynamic_cast<CAgent*>(&ScenePool[0]);
		Agent->loadBrain(m_BehaviorTree);
		AgentProperty& Property = Agent->fetchProperty();
		Property.NormalSpeed = 2.0;
		Property.SceneExit   = m_Exit;
		Property.CurPos      = m_Enter;
		Property.PathMemory  = m_PathMemory;
		CPath* Path = m_PathMemory->findPath(m_Enter, m_Exit);

		size_t Index = 0;
		size_t PathSize = Path->getPathSize();
		Ogre::Vector3 NewTargetPos = Path->getPathNode(0)->getNodePos();

		while (std::abs((NewTargetPos - Property.CurPos).length()) < 0.2f && Index < PathSize)
		{
			NewTargetPos = Path->getPathNode(Index ++)->getNodePos();
		}

		SAgentParam* AgentParam = new SAgentParam;
		AgentParam->MaxAcceleration		   = 20;
		AgentParam->Height				   = 5;
		AgentParam->Radius				   = 1;
		AgentParam->MaxSpeed			   = 3;
		AgentParam->CollosionQueryRange    = 18;
		AgentParam->SeparationWeight	   = 2.0f;
		AgentParam->NeighborSearchDistance = 12;
		AgentParam->PathOptimizationRange  = 15;
		AgentParam->ObstacleAvoidanceType  = 3.0f;
		AgentParam->UpdateFlags			   = 0;
		AgentParam->UpdateFlags = AgentParam->UpdateFlags | HIVE_CROWD_ANTICIPATE_TURNS | HIVE_CROWD_OPTIMIZE_TOPO |HIVE_CROWD_OPTIMIZE_VIS | HIVE_CROWD_OBSTACLE_AVOIDANCE;
		Property.AgentParam = AgentParam;

		Property.TargetPos = NewTargetPos;
		Ogre::Real CurDirLength = (Property.TargetPos - m_Enter).length();
		Property.CurLookAt = (Property.TargetPos - m_Enter)/CurDirLength;
		CNavPolygon* TargetPoly = m_NavMap->fetchNavPolygon(Property.TargetPos);
		unsigned int TargetPolyRef = m_NavMap->fetchPolyIndice(TargetPoly);
		Property.TargetPolyRef = TargetPolyRef;
		Agent->setProperty(Property);
		Agent->setLookAt(Property.CurLookAt);
		Agent->setPos(m_Enter);
		Agent->initialise();

		CPathCorridor* Corridor = new CPathCorridor(256);
		Corridor->initialize(Property.CurPos, Property.TargetPos, Path);
		Corridor->findCorners(Property.PathMemory);
		Property.Corridor = Corridor;

		CLocalBoundary* Boundary = new CLocalBoundary;
		Boundary->reset();
		Property.Boundary = Boundary;

		Property.TopologyOptTime = 0;
		Property.TargetReplanTime = 0;
		Property.DesiredVel   = Ogre::Vector3::ZERO;
		Property.NewVel		  = Ogre::Vector3::ZERO;
		Property.CurVel		  = Ogre::Vector3::ZERO;
		Property.DesiredSpeed = 0;
		Property.CurPolyState = HIVE_CROWDAGENT_STATE_WALKING;
		Property.TargetMoveRequestState = HIVE_CROWDAGENT_TARGET_NONE;
		Property.isActive = 1;

		m_PathMemory->reset();

		CAgentManager& AgentManager = dynamic_cast<CAgentManager&>(SceneObjMgr);
		//AgentManager.addAgent(Agent);
	}

	CAgentManager& AgentManager = dynamic_cast<CAgentManager&>(SceneObjMgr);
	AgentManager.setMoveTarget();
}

void CNormalSimulation::_createBehaviorTree(void)
{
	TreeNode* Root				   = new TreeNode;
	TreeNode* Seq_TurnAround	   = new TreeNode;
	TreeNode* Cond_TooCrowded	   = new TreeNode;
	TreeNode* Act_OppositeVelocity = new TreeNode;
	TreeNode* Seq_InsistOnPush	   = new TreeNode;
	TreeNode* Cond_NotCrowded	   = new TreeNode;
	TreeNode* Act_DecreaseVelocity = new TreeNode;

	CSelectNodeProperty    Root_Property;
	CSequenceNodeProperty  Seq_TurnAround_Property;
	CConditionNodeProperty Cond_TooCrowded_Property;
	CActionNodeProperty	   Act_OpppsiteVelocity_Property;
	CSequenceNodeProperty  Seq_InsistOnPush_Property;
	CConditionNodeProperty Cond_NotCrowded_Property;
	CActionNodeProperty	   Act_DecreaseVelocity_Property;

	CRandomSelectNodeController* Root_Controller				 = new CRandomSelectNodeController(*Root);
	CSequenceNodeController*	 Seq_TurnAround_Controller		 = new CSequenceNodeController(*Seq_TurnAround);
	CConditionNodeController*	 Cond_TooCrowded_Controller		 = new CConditionNodeController(*Cond_TooCrowded);
	CActionNodeController*		 Act_OppositeVelocity_Controller = new CActionNodeController(*Act_OppositeVelocity);
	CSequenceNodeController*	 Seq_InsistOnPush_Controller	 = new CSequenceNodeController(*Seq_InsistOnPush);
	CConditionNodeController*	 Cond_NotCrowded_Controller		 = new CConditionNodeController(*Cond_NotCrowded);
	CActionNodeController*		 Act_DecreaseVelocity_Controller = new CActionNodeController(*Act_DecreaseVelocity);

	Act_OppositeVelocity_Controller->setActionName(ACTION_SIG_SLOWDOWN);
	Act_DecreaseVelocity_Controller->setActionName(ACTION_SIG_SLOWDOWN);

	Root->Property					 = Root_Property;
	Root->Controller				 = Root_Controller;
	Seq_TurnAround->Property		 = Seq_TurnAround_Property;
	Seq_TurnAround->Controller		 = Seq_TurnAround_Controller;
	Cond_TooCrowded->Property		 = Cond_TooCrowded_Property;
	Cond_TooCrowded->Controller		 = Cond_TooCrowded_Controller;
	Act_OppositeVelocity->Property	 = Act_OpppsiteVelocity_Property;
	Act_OppositeVelocity->Controller = Act_OppositeVelocity_Controller;
	Seq_InsistOnPush->Property		 = Seq_InsistOnPush_Property;
	Seq_InsistOnPush->Controller	 = Seq_InsistOnPush_Controller;
	Cond_NotCrowded->Property		 = Cond_NotCrowded_Property;
	Cond_NotCrowded->Controller		 = Cond_NotCrowded_Controller;
	Act_DecreaseVelocity->Property   = Act_DecreaseVelocity_Property;
	Act_DecreaseVelocity->Controller = Act_DecreaseVelocity_Controller;

	m_BehaviorTree->initRoot(*Root);
	m_BehaviorTree->addChild(*Root, *Seq_TurnAround);
	m_BehaviorTree->addChild(*Root, *Seq_InsistOnPush);
	m_BehaviorTree->addChild(*Seq_TurnAround, *Cond_TooCrowded);
	m_BehaviorTree->addChild(*Seq_TurnAround, *Act_OppositeVelocity);
	m_BehaviorTree->addChild(*Seq_InsistOnPush, *Cond_NotCrowded);
	m_BehaviorTree->addChild(*Seq_InsistOnPush, *Act_DecreaseVelocity);

	m_BehaviorTree->initBegin();
	m_BehaviorTree->initEnd();
}
