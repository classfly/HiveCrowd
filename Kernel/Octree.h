#pragma once
#include <OgreAxisAlignedBox.h>
#include <tuple>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class COctreeNode;
		class KERNEL_DLL_API COctree
		{
		public:
			typedef Ogre::list<COctreeNode*>::type NodeList;

			explicit COctree(void);
			~COctree(void);

			void addNode(COctreeNode* vNode);
			void removeNode(COctreeNode* vNode);
			std::tuple<size_t, size_t, size_t> getChildIndex(const Ogre::AxisAlignedBox& vBox) const;
		private:
			inline void __increaseOverallNodeCount(void)
			{
				m_NumNodes++;
				if (m_Parent)
					m_Parent->__increaseOverallNodeCount();
			}
			inline void __decreaseOverallNodeCount(void)
			{
				m_NumNodes--;
				if (m_Parent)
					m_Parent->__decreaseOverallNodeCount();
			}
			bool __isTwiceSize(const Ogre::AxisAlignedBox& vBox) const;
		private:
			size_t		  m_NumNodes;
			COctree*	  m_Parent;
			COctree*	  m_Children[2][2][2];
			NodeList	  m_NodeList;
			Ogre::Vector3 m_HalfSize;
		};
	}
}
