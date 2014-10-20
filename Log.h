#pragma once
#include <boost/serialization/base_object.hpp>
#include <OgreString.h>
#include "Common/Object.h"
#include "LogExport.h"
namespace hiveCrowd
{
	namespace Kernel
	{
		template<class TDerivedProduct>
		class CObjectFactory;
	}

	namespace LogSystem
	{
		enum LogLevel
		{
			LOG_NORMAL_LEVEL, LOG_WARNING_LEVEL, LOG_ERROR_LEVEL, LOG_STOP_LEVEL
		};

		class CLogListener
		{
		public:
			CLogListener(){}
			virtual ~CLogListener(){}

			virtual void receiveMessage(const Ogre::String& vMessage, LogLevel vLevel, bool vDebugMask, Ogre::String& voLogName, bool& voSkipFlag) = 0;
		};

		class CStream;
		class LOG_DLL_API CLog : public Common::CObject, public boost::noncopyable
		{
		public:
			explicit CLog(void);
			explicit CLog(const Ogre::String& vName, bool vDebugOuput = false, bool vSupressFileOutput = false);
			virtual ~CLog(void);

			void				setDebugOutputEnabled(bool vDebugOutput){ m_DebugOutput = vDebugOutput; }
			void				setSupressFileOutput(bool vSupressFileOutput){ m_SupressFileOutput = vSupressFileOutput; }
			void				setTimeStampEnabled(bool vTimeStampOutput){ m_TimeStampOutput = vTimeStampOutput; }
			const Ogre::String& getName() const{ return m_LogName; }
			CStream				createStream(LogLevel vLevel = LOG_NORMAL_LEVEL, bool vDebugMask = false);
			bool				isDebugOutputEnabled() const{ return m_DebugOutput; }
			bool				isFileOutSupressed() const{ return m_SupressFileOutput; }
			bool				isTimeStampEnabled() const{ return m_TimeStampOutput; }
			void				registerListener(CLogListener* vListener);
			void				unregisterListener(CLogListener* vListener);

			virtual void		logMessage(const Ogre::String& vMessage, LogLevel vLevel, bool vDebugMask);
		protected:
			friend class boost::serialization::access;
			template<class TArchive>
			void serialize(TArchive& vArchive, size_t vVersion)
			{
				vArchive & boost::serialization::base_object<CObject>(*this);
			}

			friend class Kernel::CObjectFactory<CLog>;
		protected:
			LogLevel					m_Level;
			std::ofstream				m_LogStream;
			bool						m_DebugOutput, m_SupressFileOutput, m_TimeStampOutput;
			Ogre::String				m_LogName;
			std::vector<CLogListener*>	m_LogListeners;
		};

		class CStream
		{
		public:
			struct Flush{};

			CStream(CLog* vTarget, LogLevel vLevel, bool vDebugMask) : m_Target(vTarget), m_Level(vLevel), m_DebugMask(vDebugMask)
			{

			}
			CStream(const CStream& vRhs) : m_Target(vRhs.m_Target), m_Level(vRhs.m_Level), m_DebugMask(vRhs.m_DebugMask)
			{
				m_CacheStream.str(vRhs.m_CacheStream.str());
			}
			virtual ~CStream()
			{
				if (m_CacheStream.tellp() > 0)
					m_Target->logMessage(m_CacheStream.str(), m_Level, m_DebugMask);
			}

			template<typename T>
			CStream& operator<< (const T& vData)
			{
				m_CacheStream << vData;

				return *this;
			}

			CStream& operator<< (const Flush& vFlush)
			{
				(void)vFlush;
				m_Target->logMessage(m_CacheStream.str(), m_Level, m_DebugMask);
				m_CacheStream.str(Ogre::StringUtil::BLANK);

				return *this;
			}
		protected:
			CLog*							m_Target;
			LogLevel						m_Level;
			bool							m_DebugMask;
			Ogre::StringUtil::StrStreamType m_CacheStream;
		};
	}
}
