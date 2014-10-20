#pragma once
#include <OgreSceneManager.h>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class KERNEL_DLL_API COctreeIntersectionSceneQuery : public Ogre::DefaultIntersectionSceneQuery
		{
		public:
			explicit COctreeIntersectionSceneQuery(Ogre::SceneManager* vCreator);
			~COctreeIntersectionSceneQuery(void);

			void execute(Ogre::IntersectionSceneQueryListener* vListener) override;
		};

		class KERNEL_DLL_API COctreeRaySceneQuery : public Ogre::DefaultRaySceneQuery
		{
		public:
			explicit COctreeRaySceneQuery(Ogre::SceneManager* vCreator);
			~COctreeRaySceneQuery(void);

			void execute(Ogre::RaySceneQueryListener* vListener) override;
		};

		class KERNEL_DLL_API COctreeSphereQuery : public Ogre::DefaultSphereSceneQuery
		{
		public:
			explicit COctreeSphereQuery(Ogre::SceneManager* vCreator);
			~COctreeSphereQuery(void);

			void execute(Ogre::SceneQueryListener* vListener) override;
		};

		class KERNEL_DLL_API COctreeAxisAlignedBoxSceneQuery : public Ogre::DefaultAxisAlignedBoxSceneQuery
		{
		public:
			explicit COctreeAxisAlignedBoxSceneQuery(Ogre::SceneManager* vCreator);
			~COctreeAxisAlignedBoxSceneQuery(void);

			void execute(Ogre::SceneQueryListener* vListener) override;
		};
	}
}
