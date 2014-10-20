#pragma once
#include "KernelExport.h"
#include "Common/Object.h"
namespace hiveCrowd
{
	namespace Geometry{ class CGeometricShape; }
	namespace Kernel
	{
		struct SceneInfo;
		class  CSceneInfoSpaceManager;
		class KERNEL_DLL_API CView : public Common::CObject
		{
		public:
			typedef Ogre::list<const Common::CObject*>::type InfoList;
			typedef InfoList::iterator						 InfoListItr;
			typedef InfoList::const_iterator				 InfoListConstItr;

			explicit CView(void);
			explicit CView(size_t vSize);
			~CView();

			CView operator+(const CView& vRhs);
			CView operator-(const CView& vRhs);

			bool   isInSight(const Ogre::String& vSignature) const;
			size_t getSize(void) const{ return m_InfoList.size(); }
			void   update(const CSceneInfoSpaceManager* vSceneInfoSpaceMgr);
		private:
			InfoList				   m_InfoList;
			Geometry::CGeometricShape* m_Frustum;
		};
	}
}
