#pragma once
#include <OgreString.h>
#include "CommonMirco.h"
#include "ObjectFactoryContainer.h"
namespace hiveCrowd
{
	namespace Common
	{
		class CObject;

		template<class TDerivedProduct>
		class CObjectFactory
		{
		public:
			CObjectFactory(void){}
			CObjectFactory(const Ogre::String& vName);
			CObjectFactory(const char* vName);
			~CObjectFactory(){}
		protected:
			virtual CObject* _createProductImpl(const Ogre::String& vName) const;
			virtual CObject* _createProductImpl(const char* vName) const;
			virtual CObject* _createProductV() const;
			virtual CObject* _createProductV(const Ogre::String& vName) const;
			virtual CObject* _createProductV(const char* vName) const;
			virtual CObject* _createProductsV(size_t vSize) const;
			virtual CObject* _createProductsV(const Ogre::String& vName, size_t vSize) const;
			virtual CObject* _createProductsV(const char* vName, size_t vSize) const;
			virtual CObject* _deserializeProductV(boost::archive::text_iarchive& vTextInputArchieve) const;
			virtual CObject* _deserializeProductV(boost::archive::binary_iarchive& vBinaryInputArchieve) const;
			virtual bool	 _serializeProductV(CObject* vProduct, boost::archive::text_oarchive& voTextOutputArchieve) const;
			virtual bool	 _serializeProductV(CObject* vProduct, boost::archive::binary_oarchive& voBinaryOutputArchieve) const;

			friend class CObjectFactoryContainer;
		};

		template class COMMON_DLL_API CObjectFactory<CObject>;

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductImpl(const Ogre::String& vName) const
		{
			return nullptr;
		}

		template<class TDerivedProduct>
		CObject* hiveCrowd::Common::CObjectFactory<TDerivedProduct>::_createProductImpl(const char* vName) const
		{
			return nullptr;
		}

		template<class TDerivedProduct>
		CObjectFactory<TDerivedProduct>::CObjectFactory(const Ogre::String& vName)
		{
			REGIST_FACTORY(vName, reinterpret_cast<const CObjectFactory<CObject>*>(this));
		}

		template<class TDerivedProduct>
		CObjectFactory<TDerivedProduct>::CObjectFactory(const char* vName)
		{
			REGIST_FACTORY(vName, reinterpret_cast<const CObjectFactory<CObject>*>(this));
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductV() const
		{
			return new TDerivedProduct;
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductV(const Ogre::String& vName) const
		{
			return new TDerivedProduct(vName);
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductV(const char* vName) const
		{
			return new TDerivedProduct(vName);
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductsV(size_t vSize) const
		{
			return new TDerivedProduct[vSize];
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductsV(const Ogre::String& vName, size_t vSize) const
		{
			return new TDerivedProduct[vSize];
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_createProductsV(const char* vName, size_t vSize) const
		{
			return new TDerivedProduct[vSize];
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_deserializeProductV(boost::archive::text_iarchive& vTextInputArchieve) const
		{
			TDerivedProduct* Product = nullptr;
			try
			{
				vTextInputArchieve >> Product;
			}
			catch(std::exception* vException)
			{
				SAFE_DELETE(Product);
			}

			return Product;
		}

		template<class TDerivedProduct>
		CObject* CObjectFactory<TDerivedProduct>::_deserializeProductV(boost::archive::binary_iarchive& vBinaryInputArchieve) const
		{
			TDerivedProduct* Product = nullptr;
			try
			{
				vBinaryInputArchieve >> Product;
			}

			catch(std::exception* vException)
			{
				SAFE_DELETE(Product);
			}

			return Product;
		}

		template<class TDerivedProduct>
		bool CObjectFactory<TDerivedProduct>::_serializeProductV(CObject* vProduct, boost::archive::text_oarchive& voTextOutputArchieve) const
		{
			_ASSERT(vProduct);

			try
			{
				TDerivedProduct* SubProduct = dynamic_cast<TDerivedProduct*>(vProduct);
				if (!SubProduct)
					return false;
				voTextOutputArchieve << SubProduct;

				return true;
			}
			catch(std::exception* vException)
			{
				// TODO:


				return false;
			}
		}

		template<class TDerivedProduct>
		bool CObjectFactory<TDerivedProduct>::_serializeProductV(CObject* vProduct, boost::archive::binary_oarchive& voBinaryOutputArchieve) const
		{
			_ASSERT(vProduct);

			try
			{
				TDerivedProduct* SubProduct = dynamic_cast<TDerivedProduct*>(vProduct);
				if (!SubProduct)
					return false;
				voBinaryOutputArchieve << SubProduct;

				return true;
			}

			catch(std::exception* vException)
			{
				// TODO:


				return false;
			}
		}
	}
}
