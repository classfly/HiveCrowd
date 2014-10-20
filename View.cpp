#include "View.h"
#include "SceneInfoSpaceManager.h"
using namespace hiveCrowd::Kernel;

CView::CView() : hiveCrowd::Common::CObject(typeid(*this).name())
{

}

CView::CView(size_t vSize) : hiveCrowd::Common::CObject(typeid(*this).name())
{
	m_InfoList.resize(vSize);
}

CView::~CView()
{
	m_InfoList.clear();
}

CView CView::operator+(const CView& vRhs)
{
	InfoListItr HeadItr = m_InfoList.begin();
	if (HeadItr == m_InfoList.end())
		return CView(*this);

	CView Result;
	for (InfoListConstItr itr=m_InfoList.begin() ; itr!=m_InfoList.end() ; ++itr)
		Result.m_InfoList.push_back(*itr);
	for (InfoListConstItr itr=vRhs.m_InfoList.begin() ; itr!=vRhs.m_InfoList.end() ; ++itr)
		Result.m_InfoList.push_back(*itr);

	HeadItr						= Result.m_InfoList.begin();
	InfoListConstItr CurrentItr	= HeadItr;
	InfoListConstItr RunItr		= HeadItr;
	InfoListConstItr RunNextItr	= HeadItr;
	InfoListConstItr TailItr	= m_InfoList.end();
	const size_t LIST_OFFSET	= (size_t)1;
	while (CurrentItr != TailItr)
	{
		RunItr						   = CurrentItr;
		RunNextItr					   = std::next(CurrentItr, LIST_OFFSET);
		const Common::CObject* DataPtr = (*CurrentItr);
		while (RunNextItr != TailItr)
		{
			if (DataPtr == (*RunItr))
			{
				RunItr = std::next(RunNextItr, LIST_OFFSET);
				RunNextItr = std::next(RunItr, LIST_OFFSET);
			}
			else
			{
				RunItr = std::next(RunItr, LIST_OFFSET);
				RunNextItr = std::next(RunNextItr, LIST_OFFSET);
			}
		}

		CurrentItr = std::next(CurrentItr, LIST_OFFSET);
	}

	return Result;
}

CView CView::operator-(const CView& vRhs)
{
	// TODO:


	return CView();
}

void CView::update(const CSceneInfoSpaceManager* vSceneInfoSpaceMgr)
{
	// TODO:




}

bool CView::isInSight(const Ogre::String& vSignature) const
{
	const SceneInfoSpace& Space(GET_SCENE_INFO(vSignature));
	const CSceneInfoModule& SceneInfoModule(GET_SCENE_INFO_MODULE(vSignature));
	SceneInfoBlockGroup voSceneInfoBlock;

	return SceneInfoModule.intersect(Space, voSceneInfoBlock);
}
