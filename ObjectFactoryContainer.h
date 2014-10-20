#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <OgrePrerequisites.h>
#include "CommonExport.h"
#include "Singleton.h"
namespace hiveCrowd
{
	namespace Common
	{
		class CObject;

		template<class TDerivedProduct>
		class CObjectFactory;

		class COMMON_DLL_API CObjectFactoryContainer : public CSingleton<CObjectFactoryContainer>
		{
		public:
			typedef Ogre::map<Ogre::String, const CObjectFactory<CObject>*>::type ObjectFactoryMap;

			CObjectFactoryContainer(){}
			~CObjectFactoryContainer(){}

			CObject* createProduct(const Ogre::String& vSignature);
			CObject* createProducts(const Ogre::String& vSignature, size_t vSize);
			CObject* deserializeObject(const Ogre::String& vSignature, boost::archive::text_iarchive& vTextInputArchive);
			CObject* deserializeObject(const Ogre::String& vSignature, boost::archive::binary_iarchive& vBinaryInputArchive);
			bool	 serializeObject(CObject* vObject, boost::archive::text_oarchive& voTextOutputArchive);
			bool	 serializeObject(CObject* vObject, boost::archive::binary_oarchive& voBinaryOutputArchive);
			void	 registerObjectFactory(const Ogre::String& vSignature, const CObjectFactory<CObject>* vObjectFactory);
		private:
			ObjectFactoryMap m_ObjectFactoryMap;
		};

		template class COMMON_DLL_API CSingleton<CObjectFactoryContainer>;

#define REGIST_FACTORY(vSignature, vFactoryPtr) hiveCrowd::Common::CObjectFactoryContainer::getSingletonPtr()->registerObjectFactory(vSignature, vFactoryPtr)
#define CREATE_OBJECT(vSignature) hiveCrowd::Common::CObjectFactoryContainer::getSingletonPtr()->createProduct(vSignature)
#define CREATE_OBJECT_ARRAY(vSignature, vSize) hiveCrowd::Common::CObjectFactoryContainer::getSingletonPtr()->createProducts(vSignature, vSize)
	}
}
