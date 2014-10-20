#pragma once
#include <OgreString.h>
#include <boost/thread.hpp>
#include "CommonExport.h"
#include "SubjectSignature.h"
namespace hiveCrowd
{
	namespace Common
	{
		class CObserver;
		class CEvent;
		class COMMON_DLL_API CSubject
		{
		public:
			typedef Ogre::vector<CObserver*>::type				 ObserverGroup;
			typedef ObserverGroup::iterator						 ObserverIterator;
			typedef ObserverGroup::const_iterator				 ObserverConstIterator;
			typedef Ogre::map<Ogre::String, ObserverGroup>::type ObserverMap;
		public:
			explicit CSubject(void){}
			virtual ~CSubject(void);

			void registerObserver(const Ogre::String& vSignature, CObserver* vObserver);
			void triggerEvent(const Ogre::String& vSignature, const CEvent* vEvent);
			void unregisterObserver(const Ogre::String& vSignature, CObserver* vObserver);
			void unregisterAllObservers(const Ogre::String& vSignature);
		private:
			bool find(const Ogre::String& vSignature) const;
		protected:
			boost::mutex m_Mutex;
			ObserverMap  m_ObserverMap;
		};
	}
}
