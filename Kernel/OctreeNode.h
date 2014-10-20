#pragma once
#include <OgreSceneNode.h>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class COctree;
		class KERNEL_DLL_API COctreeNode : public Ogre::SceneNode
		{
		public:
			explicit COctreeNode(Ogre::SceneManager* vCreator);
			explicit COctreeNode(Ogre::SceneManager* vCreator, const Ogre::String& vName);
			~COctreeNode(void);

			Ogre::Node* removeChild(Ogre::Node* vChild)		   override;
			Ogre::Node* removeChild(unsigned short vIndex)	   override;
			Ogre::Node* removeChild(const Ogre::String& vName) override;
			void		removeAllChildren(void)				   override;
			void		setOctant(COctree* vOctant){ m_Octant = vOctant; }
			COctree*	getOctant(void) const	   { return m_Octant; }

			static size_t PREALLOCATED_CORNER_COUNT;
		private:
			void __updateBounds(void);
			void __removeNodeAndChildren(void);
			bool __isInBox(const Ogre::AxisAlignedBox& vBox);
		private:
			COctree* m_Octant;
			Ogre::AxisAlignedBox m_LocalAABB;
			Ogre::Real m_Corners[24];
		};
	}
}
