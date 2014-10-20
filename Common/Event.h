#pragma once
#include "Object.h"
namespace hiveCrowd
{
	namespace Common
	{
		class CEvent : public CObject
		{
		public:
			explicit CEvent(void){}
			explicit CEvent(const Ogre::String& vName) : CObject(vName){}
			virtual ~CEvent(void){}

			// TODO:
			void		   set(const CObject* vObject){ m_Object = vObject; }
			const CObject* get(void) const{ return m_Object; }
		protected:
			const CObject* m_Object;
		};
	}
}
