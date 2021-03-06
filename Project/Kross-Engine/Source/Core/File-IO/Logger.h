/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 *      - Chris Deitch.
 */

#pragma once

#include "../Core.h"
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "user32.lib")

//   #include <sysinfoapi.h> NO target arcitecture error on include. Why?

namespace Kross
{
	class KROSS_API Logger
	{
	private:
		std::vector<std::string> m_LogLines;

	protected:
		friend class Manifest;
		template<typename Type> friend class Serialiser;

		/*!
			Writes all Log Lines to a Output File.
		*/
		void Dump(const std::string& title, const std::string& filepath);

		/*!
			Gets the Time the Log was Recorded.
		*/
		std::string GetLogTime();

	public:
		Logger() :
			m_LogLines	(std::vector<std::string>())
		{};
		~Logger() { m_LogLines.clear(); };

		/*!
			Adds line to the Logger to Output.
		*/
		void Write(const std::string& line) { m_LogLines.push_back(line); };

		/*!
			Adds Log line to the Logger to Output.
		*/
		void WriteLog(const std::string& line);

		/*!
			Adds Warning line to the Logger to Output.
		*/
		void WriteWarning(const std::string& line);

		/*!
			Adds Error line to the Logger to Output.
		*/
		void WriteError(const std::string& line);

		/*!
			Adds Spacing to the Logger to Output.
		*/
		void WriteSpace() { m_LogLines.push_back(""); };


		/*!
			Adds system info to the logger to output.
		*/
		void WriteSystemInfo();


	};
}