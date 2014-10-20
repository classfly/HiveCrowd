#pragma once
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
namespace hiveCrowd
{
	namespace Common
	{
		template<class T>
		class CSingleton
		{
		public:
			CSingleton(void)
			{
				_ASSERT(!m_Singleton);
			}
			~CSingleton(void)
			{
				_ASSERT(m_Singleton);
				m_Singleton = nullptr;
			}
			static T& getSingleton(void)
			{
				if (m_Singleton == nullptr)
				{
					boost::mutex::scoped_lock Lock(m_Mutex);
					if (m_Singleton == nullptr)
						m_Singleton = new T();
				}

				return (*m_Singleton);
			}
			static T* getSingletonPtr(void)
			{
				if (m_Singleton == nullptr)
				{
					boost::mutex::scoped_lock Lock(m_Mutex);
					if (m_Singleton == nullptr)
						m_Singleton = new T();
				}

				return m_Singleton;
			}
		protected:
			static T* m_Singleton;
			static boost::mutex m_Mutex;
		};
	}
}
