#pragma once

namespace hiveCrowd
{
	namespace LogSystem
	{
		template<class TLogThread, class TLog>
		class CLogThread{};

		template<class TLogThread, class TLog>
		class CLogThread<TLogThread*, TLog*>
		{
		public:
			typedef TLogThread	ThreadType;
			typedef TLog		LogType;
			typedef size_t		ThreadID;

			explicit CLogThread() : m_ID(0)
			{

			}
			explicit CLogThread(size_t vThreadID) : m_ID(vThreadID)
			{

			}
			virtual ~CLogThread(){}

			ThreadID getThreadID() const{ return m_ID; }
		private:
			ThreadID	m_ID;
		};
	}
}
