#include <boost/exception/exception.hpp>
#include "ObjectFactory.h"
#include "Object.h"
using namespace hiveCrowd::Common;

CObjectFactoryContainer* CSingleton<CObjectFactoryContainer>::m_Singleton = nullptr;
boost::mutex CSingleton<CObjectFactoryContainer>::m_Mutex;

CObject* CObjectFactoryContainer::createProduct(const Ogre::String& vSignature)
{
	Ogre::String UpperSignature = boost::algorithm::to_upper_copy(vSignature);
	CObject* NewProduct			= nullptr;
	if (m_ObjectFactoryMap.count(UpperSignature) == 0)
		return NewProduct;

	try
	{
		NewProduct = m_ObjectFactoryMap.at(UpperSignature)->_createProductV(UpperSignature);
		return NewProduct;
	}
	catch (boost::exception* e)
	{
		// TODO: Insert exception information into LogSystem!!!
		SAFE_DELETE(NewProduct);

		return nullptr;
	}
}

CObject* CObjectFactoryContainer::createProducts(const Ogre::String& vSignature, size_t vSize)
{
	Ogre::String UpperSignature = boost::algorithm::to_upper_copy(vSignature);
	CObject* NewProducts		= nullptr;
	if (m_ObjectFactoryMap.count(UpperSignature) == 0)
		return NewProducts;

	try
	{
		NewProducts = m_ObjectFactoryMap.at(UpperSignature)->_createProductsV(UpperSignature, vSize);

		return NewProducts;
	}
	catch (boost::exception* e)
	{
		// TODO: Insert exception information into LogSystem!!!
		SAFE_DELETE_ARRAY(NewProducts);

		return nullptr;
	}
}

CObject* CObjectFactoryContainer::deserializeObject( const Ogre::String& vSignature, boost::archive::text_iarchive& vBinaryInputArchive )
{
	return m_ObjectFactoryMap.at(vSignature)->_deserializeProductV(vBinaryInputArchive);
}

CObject* CObjectFactoryContainer::deserializeObject( const Ogre::String& vSignature, boost::archive::binary_iarchive& vBinaryInputArchive )
{
	return m_ObjectFactoryMap.at(vSignature)->_deserializeProductV(vBinaryInputArchive);
}

bool CObjectFactoryContainer::serializeObject( CObject* vObject, boost::archive::text_oarchive& voTextOutputArchive )
{
	if (!vObject)
		return false;
	if (m_ObjectFactoryMap.count(vObject->getObjectCreatorName()) == 0)
		return false;

	return m_ObjectFactoryMap[vObject->getObjectCreatorName()]->_serializeProductV(vObject, voTextOutputArchive);
}

bool CObjectFactoryContainer::serializeObject( CObject* vObject, boost::archive::binary_oarchive& voBinaryOutputArchive )
{
	if (!vObject)
		return false;
	if (m_ObjectFactoryMap.count(vObject->getObjectCreatorName()) == 0)
		return false;

	return m_ObjectFactoryMap[vObject->getObjectCreatorName()]->_serializeProductV(vObject, voBinaryOutputArchive);
}

void CObjectFactoryContainer::registerObjectFactory( const Ogre::String& vSignature, const CObjectFactory<CObject>* vObjectFactory )
{
	m_ObjectFactoryMap[boost::algorithm::to_upper_copy(vSignature)] = vObjectFactory;
}
