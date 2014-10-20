#include <boost/algorithm/string.hpp>
#include "Subject.h"
#include "Event.h"
#include "Observer.h"
using namespace hiveCrowd::Common;

CSubject::~CSubject( void )
{
	// TODO:




}

void CSubject::registerObserver( const Ogre::String& vSignature, CObserver* vObserver )
{
	if (vObserver == nullptr || !find(vSignature))
		return ;

	boost::mutex::scoped_lock Lock(m_Mutex);
	ObserverGroup& Group(m_ObserverMap[boost::algorithm::to_upper_copy(vSignature)]);
	if (std::find(Group.begin(), Group.end(), vObserver) == Group.end())
		Group.push_back(vObserver);
}

void CSubject::triggerEvent( const Ogre::String& vSignature, const CEvent* vEvent )
{
	const Ogre::String UpperSignature(boost::algorithm::to_upper_copy(vSignature));
	if (m_ObserverMap.count(UpperSignature) > 0)
	{
		ObserverGroup& Group(m_ObserverMap[UpperSignature]);
		const size_t GroupSize = Group.size();
		for (size_t i=0 ; i<GroupSize ; ++i)
			Group[i]->_triggerEvent(UpperSignature, vEvent, this);
	}
}

void CSubject::unregisterObserver( const Ogre::String& vSignature, CObserver* vObserver )
{
	ObserverGroup& Group(m_ObserverMap[boost::algorithm::to_upper_copy(vSignature)]);
	ObserverIterator& itr(std::find(Group.begin(), Group.end(), vObserver));
	if (itr == Group.end())
		return ;

	Group.erase(itr);
}

void CSubject::unregisterAllObservers( const Ogre::String& vSignature )
{
	ObserverGroup& Group(m_ObserverMap[boost::algorithm::to_upper_copy(vSignature)]);
	Group.clear();
}

bool CSubject::find( const Ogre::String& vSignature ) const
{
	return (m_ObserverMap.count(boost::algorithm::to_upper_copy(vSignature)) > 0);
}
