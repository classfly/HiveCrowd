#pragma once
namespace hiveCrowd
{
	namespace Common
	{
		class CEvent;
		class CSubject;
		class CObserver
		{
		public:
			explicit CObserver(void){}
			virtual	~CObserver(void){}

		protected:
			virtual void _triggerEvent(const Ogre::String& vEventSignature, const CEvent* vEvent, const CSubject* vSubject) = 0;

			friend class CSubject;
		};
	}
}
