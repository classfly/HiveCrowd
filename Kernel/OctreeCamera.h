#pragma once
#include <OgreCamera.h>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class KERNEL_DLL_API COctreeCamera : public Ogre::Camera
		{
		public:
			enum VISIBILITY_FLAG{ NONE, PARTIAL, FULL };

			explicit COctreeCamera(const Ogre::String& vName, Ogre::SceneManager* vCreator);
			~COctreeCamera(void);

			VISIBILITY_FLAG getVisibility(const Ogre::AxisAlignedBox& vBox) const;
		};
	}
}
