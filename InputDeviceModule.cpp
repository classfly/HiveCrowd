#include <OgreRenderWindow.h>
#include "InputDeviceModule.h"
using namespace hiveCrowd::Kernel;

CInputDeviceModule::CInputDeviceModule(void) : m_InputMgr(nullptr), 
											   m_MouseDevice(nullptr), 
											   m_KeyboardDevice(nullptr), 
											   m_RenderWindow(nullptr), 
											   m_IsMouseExclusive(false)
{

}

void CInputDeviceModule::initialize(Ogre::RenderWindow* vRenderWindow, bool vMouseClusiveFlag)
{
	if (m_InputMgr == nullptr)
	{
		m_RenderWindow   = vRenderWindow;
		m_IsMouseExclusive = vMouseClusiveFlag;

		OIS::ParamList IODeviceParamList;
		size_t WindowHandle = 0;
		m_RenderWindow->getCustomAttribute("WINDOW", &WindowHandle);
		std::ostringstream WindowHandleString;
		WindowHandleString << WindowHandle;
		IODeviceParamList.insert(std::make_pair(Ogre::String("WINDOW"), WindowHandleString.str()));
		if (m_IsMouseExclusive == false)
		{
			IODeviceParamList.insert(std::make_pair(Ogre::String("w32_mouse"), "DISCL_FOREGROUND"));
			IODeviceParamList.insert(std::make_pair(Ogre::String("w32_mouse"), "DISCL_NONEXCLUSIVE"));
			IODeviceParamList.insert(std::make_pair(std::string("w32_keyboard"), "DISCL_FOREGROUND"));
			IODeviceParamList.insert(std::make_pair(std::string("w32_keyboard"), "DISCL_NONEXCLUSIVE"));
		}
		m_InputMgr       = OIS::InputManager::createInputSystem(IODeviceParamList);
		m_MouseDevice    = static_cast<OIS::Mouse*>(m_InputMgr->createInputObject(OIS::OISMouse, true));
		m_KeyboardDevice = static_cast<OIS::Keyboard*>(m_InputMgr->createInputObject(OIS::OISKeyboard, true));

		updateMouseExtents(m_RenderWindow->getWidth(), m_RenderWindow->getHeight());
	}
}

void CInputDeviceModule::destroy(void)
{
	if (m_InputMgr != nullptr)
	{
		m_InputMgr->destroyInputObject(m_MouseDevice);
		m_InputMgr->destroyInputObject(m_KeyboardDevice);
		OIS::InputManager::destroyInputSystem(m_InputMgr);
		m_InputMgr = nullptr;
	}
}

void CInputDeviceModule::updateMouseExtents(Ogre::Real vRenderWindowWidth, Ogre::Real vRenderWindowHeight)
{
	_ASSERT(m_MouseDevice);
	const OIS::MouseState& MouseState(m_MouseDevice->getMouseState());
	MouseState.width  = vRenderWindowWidth;
	MouseState.height = vRenderWindowHeight;
}

void CInputDeviceModule::setMouseEventCallback(OIS::MouseListener* vMouseListener)
{
	_ASSERT(m_MouseDevice);
	m_MouseDevice->setEventCallback(vMouseListener);
}

void CInputDeviceModule::setKeryboardEventCallback(OIS::KeyListener* vKeyListener)
{
	_ASSERT(m_KeyboardDevice);
	m_KeyboardDevice->setEventCallback(vKeyListener);
}

void CInputDeviceModule::captureEvents(void)
{
	m_MouseDevice->capture();
	m_KeyboardDevice->capture();
}
