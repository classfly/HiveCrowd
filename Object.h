#pragma once
#include <boost/algorithm/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <OgreMemoryAllocatorConfig.h>
#include "CommonExport.h"
namespace hiveCrowd
{
	namespace Common
	{
		class COMMON_DLL_API CObject : public Ogre::GeneralAllocatedObject
		{
		public:
			CObject(void) : m_Type(typeid(*this).name())
			{}
			CObject(const Ogre::String& vName) : m_Type(typeid(*this).name()), m_Name(vName)
			{}
			virtual ~CObject(void){}

			virtual bool		operator==(const CObject& vRhs) const{ return true; }
			void				setObjectCreatorName(const Ogre::String& vCreatorName){ m_CreatorName = boost::algorithm::to_upper_copy(vCreatorName); }
			const Ogre::String& getObjectCreatorName(void) const{ return m_CreatorName; }
			const Ogre::String& getName(void) const{ return m_Name; }
			const Ogre::String& getType(void) const{ return m_Type; }
		private:
			template<class TArchive>
			void serialize(TArchive& vArchive, size_t vVersion)
			{
				vArchive & m_CreatorName;
				vArchive & m_Type;
				vArchive & m_Name;
			}
		protected:
			Ogre::String m_CreatorName, m_Type, m_Name;

			friend class boost::serialization::access;
		};
	}
}
