#pragma once
#include <OgreSceneManager.h>
#include <OgreSphere.h>
#include "KernelExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		class CSceneTreeNode;
		class CSceneTree;
		class KERNEL_DLL_API CTreeSceneManager : public Ogre::SceneManager
		{
		public:
			CTreeSceneManager(const Ogre::String& vName){}
			virtual ~CTreeSceneManager(void){}

			virtual const Ogre::String& getTypeName(void) const;
			virtual Ogre::SceneNode*    createSceneNodeImpl(void) override;
			virtual Ogre::SceneNode*	createSceneNodeImpl(const Ogre::String& vName) override;
			virtual void				destroySceneNode(const Ogre::String& vName) override;
			virtual Ogre::Camera*		createCamera(const Ogre::String& vName) override;
			void						findNodesIn(const Ogre::AxisAlignedBox& vBox, Ogre::list<CSceneTreeNode*>::type& vList, const CSceneTreeNode* vExcludeNode = nullptr);
			void						findNodesIn(const Ogre::Sphere& vSphere, Ogre::list<CSceneTreeNode*>::type& vList, const CSceneTreeNode* vExcludeNode = nullptr);
			void						findNodesIn(const Ogre::PlaneBoundedVolume& vVolume, Ogre::list<CSceneTreeNode*>::type& vList, const CSceneTreeNode* vExcludeNode = nullptr);
			void						findNodesIn(const Ogre::Ray& vRay, Ogre::list<CSceneTreeNode*>::type& vList, const CSceneTreeNode* vExcludeNode);
		protected:
			void _updateSceneNode(CSceneTreeNode* vNode);
			void _addSceneNode(CSceneTreeNode* vNode, CSceneTree* vSceneTree, int vDepth = 0);
			void _removeSceneNode(CSceneTreeNode* vNode);
		};

		class KERNEL_DLL_API CTreeSceneManagerFactory : public Ogre::SceneManagerFactory
		{
		public:
			CTreeSceneManagerFactory(void){}
			virtual ~CTreeSceneManagerFactory(void){}

			virtual CTreeSceneManager* createInstance(const Ogre::String& vInstanceName) override;
			virtual	void			   destroyInstance(Ogre::SceneManager* vInstance) override;
		};
	}
}
