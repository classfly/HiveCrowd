#pragma once
#include "KernelExport.h"
#include "HiveCrowdPrerequisites.h"
#include "CrowdSimulation.h"
#include "BehaviorTree.h"
#include "SceneObjectManager.h"
#include "StaticSceneObjectManager.h"
#include "PathEngine/NavMap.h"
#include "PathEngine/AStarPathfindingInterface.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class CInputDeviceModule;
		class CScene;
		class KERNEL_DLL_API CNormalSimulation : public CCrowdSimulation
		{
		public:
			explicit CNormalSimulation(void);
			~CNormalSimulation(void);

			void					  startSimulation(void)		  override;
			const Ogre::RenderWindow* getRenderWindow(void) const override{ return m_RenderWindow; }
			bool					  frameRenderingQueued(const Ogre::FrameEvent& vEvent) override;
			void					  windowResized(Ogre::RenderWindow* vRenderWindow) override;
			void					  windowClosed(Ogre::RenderWindow* vRenderWindow) override;
			bool					  mouseMoved(const OIS::MouseEvent& vMouseArg) override;
			bool					  mousePressed(const OIS::MouseEvent& vMouseArg, OIS::MouseButtonID vID) override;
			bool					  mouseReleased(const OIS::MouseEvent& vMouseArg, OIS::MouseButtonID vID) override;
			bool					  keyPressed(const OIS::KeyEvent& vKeyArg) override;
			bool					  keyReleased(const OIS::KeyEvent& vKeyArg) override;
		private:
			void _createSceneManager(void) override;
			void _createListener(void)	   override;
			void _createCamera(void)	   override;
			void _createScene(void)		   override;
			bool _setup(void)			   override;
			bool _configure(void)	       override;
			bool _loadResources(void)	   override;

			void _createBehaviorTree(void);
			void _createCrowd(void);
			void _drawMesh(void);
		private:
			bool					   m_IsShutDown;
			Ogre::Root*				   m_Root;
			Ogre::Camera*			   m_Camera;
			Ogre::RenderWindow*		   m_RenderWindow;
			Ogre::OverlaySystem*	   m_OverlaySystem;
			Ogre::SceneManager*		   m_SimulationSceneMgr;
			CInputDeviceModule*		   m_InputDeviceModule;
			OgreBites::InputContext    m_InputContext;
			OgreBites::SdkTrayManager* m_SdkTrayMgr;
			OgreBites::SdkCameraMan*   m_SdkCameraMan;
			CScene*					   m_Scene;

			Ogre::SceneNode*    m_TestLineSceneNode;
			Ogre::SceneNode*	m_TestTriSceneNode;
			Ogre::ManualObject* m_TestMOWalk;
			Ogre::ManualObject* m_TestMOPath;

			Ogre::Vector3 m_Enter;
			Ogre::Vector3 m_Exit;
			hiveCrowd::PathEngine::CNavMap*					   m_NavMap;
			hiveCrowd::PathEngine::CAStarPathfindingInterface* m_PathMemory;
			CBehaviorTree*									   m_BehaviorTree;
		};
	}
}
