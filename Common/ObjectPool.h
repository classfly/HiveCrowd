#pragma once
#include <vector>
#include <map>
#include "CommonMirco.h"
namespace hiveCrowd
{
	namespace Common
	{
		class CObject;

		template<class TObject>
		class CObjectPool
		{
		public:
			typedef TObject value_type;
			typedef std::vector<CObject*>::iterator		  ObjectIterator;
			typedef std::vector<CObject*>::const_iterator ObjectConstIterator;

			CObjectPool(){}
			CObjectPool(const CObjectPool& vRhs)
			{
				_ASSERT(this != &vRhs);

				try
				{
					SAFE_DELETE_ARRAY(m_Objects[0]);
				}
				catch(std::exception* vException)
				{

				}
			}
			~CObjectPool()
			{
				clear();
			}

			TObject& operator[](size_t vIndex)
			{
				_ASSERT(vIndex < m_Objects.size());

				return dynamic_cast<TObject&>(*(m_Objects[vIndex]));
			}

			ObjectIterator begin(void)
			{
				return m_Objects.begin();
			}

			ObjectIterator end(void)
			{
				return m_Objects.end();
			}

			const TObject& at(size_t vIndex) const
			{
				_ASSERT(vIndex < m_Objects.size());

				return dynamic_cast<const TObject&>(*(m_Objects.at(vIndex)));
			}

			ObjectIterator find(const TObject& vObject)
			{
				return std::find(m_Objects.begin(), m_Objects.end(), &vObject);
			}

			CObjectPool& operator=(const CObjectPool& vRhs)
			{
				try
				{
					if (this != &vRhs)
					{
						clear();
						size_t PoolSize = vRhs.m_Objects.size();
						m_Objects.resize(PoolSize);
						for (size_t i=0 ; i<PoolSize ; ++i)
						{
							m_Objects[i] = new TObject();
							memmove(m_Objects[i], vRhs.m_Objects.at(i), sizeof(TObject));
						}
					}

					return *this;
				}
				catch(std::exception* vException)
				{

				}

			}

			size_t size(void) const
			{
				return m_Objects.size();
			}

			void push(TObject* vObject)
			{
				if (!vObject)
					return ;

				m_Objects.push_back(vObject);
			}

			ObjectIterator erase(ObjectConstIterator vItr)
			{
				return m_Objects.erase(vItr);
			}

			void clear()
			{
				size_t NumObject = m_Objects.size();
				for (size_t i=0 ; i<NumObject ; ++i)
					SAFE_DELETE(m_Objects[i]);
			}
		private:
			std::vector<CObject*> m_Objects;
		};
	}
}
