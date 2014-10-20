#pragma once
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
#include <OISMouse.h>
#include <OISKeyboard.h>
namespace hiveCrowd
{
	namespace Kernel
	{
		class CCrowdSimulation : public Ogre::WindowEventListener, 
								 public Ogre::FrameListener, 
								 public OIS::MouseListener,
								 public OIS::KeyListener, 
								 public OgreBites::SdkTrayListener
		{
		public:
			explicit CCrowdSimulation(void){}
			virtual ~CCrowdSimulation(void){}

			virtual void					  startSimulation(void)		  = 0;
			virtual const Ogre::RenderWindow* getRenderWindow(void) const = 0;
		protected:
			virtual void _createSceneManager(void) = 0;
			virtual void _createListener(void)	   = 0;
			virtual void _createCamera(void)	   = 0;
			virtual void _createScene(void)		   = 0;
			virtual bool _setup(void)			   = 0;
			virtual bool _configure(void)		   = 0;
			virtual bool _loadResources(void)	   = 0;
		};
	}
}
