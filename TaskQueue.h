#pragma once
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <OgrePrerequisites.h>
namespace hiveCrowd
{
	namespace Common
	{
		template<class TTaskProcess, class TTask>
		class CTaskQueue
		{
		public:
			typedef size_t TaskThreadID;
			typedef Ogre::vector<boost::thread*>::type TaskThreadGroup;
			typedef TaskThreadGroup::iterator		   TaskThreadIterator;
			typedef TaskThreadGroup::const_iterator	   TaskThreadConstIterator;

			explicit CTaskQueue(void){}
			explicit CTaskQueue(size_t vSize){}
			virtual ~CTaskQueue(void){}
			
			void bind(boost::function<void(TTaskProcess*, TTask& vTask)> vBindFunc)
			{
				m_BindFunc = vBindFunc;
			}

			void commitTask(TTask& vTask, TTaskProcess* vTaskProcess)
			{
				boost::thread& NewThread(boost::thread(boost::bind(m_BindFunc, vTaskProcess, vTask)));
// 				HANDLE ThreadHandle = NewThread.native_handle();
// 
// 				return GetThreadId(ThreadHandle);
			}
		private:
			boost::function<void(TTaskProcess*, TTask& vTask)> m_BindFunc;
		};
	}
}
