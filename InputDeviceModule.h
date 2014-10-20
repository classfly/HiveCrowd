#pragma once
#include <OgrePrerequisites.h>
#include <OISInputManager.h>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class KERNEL_DLL_API CInputDeviceModule
		{
		public:
			explicit CInputDeviceModule(void);
			~CInputDeviceModule(void){}

			void					  initialize(Ogre::RenderWindow* vRenderWindow, bool vMouseClusiveFlag);
			void					  destroy(void);
			void					  setMouseEventCallback(OIS::MouseListener* vMouseListener);
			void					  setKeryboardEventCallback(OIS::KeyListener* vKeyListener);
			void					  updateMouseExtents(Ogre::Real vRenderWindowWidth, Ogre::Real vRenderWindowHeight);
			void					  captureEvents(void);
			bool					  isMouseExclusive(void) const{ return m_IsMouseExclusive; }
			const OIS::Mouse*		  getMouseDevice(void) const{ return m_MouseDevice; }
			OIS::Mouse*				  fetchMouseDevice(void){ return m_MouseDevice; }
			const OIS::Keyboard*	  getKeyboardDevice(void) const{ return m_KeyboardDevice; }
			OIS::Keyboard*			  fetchKeyboardDevice(void){ return m_KeyboardDevice; }
			const Ogre::RenderWindow* getRenderWindow(void) const;
		private:
			OIS::InputManager*  m_InputMgr;
			OIS::Mouse*		    m_MouseDevice;
			OIS::Keyboard*      m_KeyboardDevice;
			Ogre::RenderWindow* m_RenderWindow;
			bool				m_IsMouseExclusive;
		};
	}
}
