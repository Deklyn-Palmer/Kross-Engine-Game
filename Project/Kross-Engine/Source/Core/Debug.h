/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 *		- Chris Deitch.
 */

#pragma once

#include "Core.h"
#include "Math/Math.h"

namespace Kross
{
	class KROSS_API Debug
	{
	private:
		static bool m_HasEnteredLog;

		static void StartLog() { std::cout << "[Log]"; Debug::m_HasEnteredLog = true; };
		static void StartWarning() { std::cout << "[Warning]"; Debug::m_HasEnteredLog = true; };
		static void StartError() { std::cout << "[Error]"; Debug::m_HasEnteredLog = true; };
		static void StartGLError() { std::cout << "[OpenGL Error]"; Debug::m_HasEnteredLog = true; };

	public:
		// Creates a Space.
		static void Spacing() 
		{ 
			#ifdef KROSS_RELEASE 
			return; 
			#endif

			std::cout << std::endl; 
		}

		// Logs a Message.
		static void Log(std::string message);

		// Logs a Message.
		static void Log(const char* message);

		// Logs a Message. (Bool)
		static void Log(bool value);

		// Logs a Message. (Float)
		static void Log(float value);

		// Logs a Message. (Int)
		static void Log(int value);

		// Logs a Message. (Vector1)
		static void Log(Vector1 vector);

		// Logs a Message. (Vector2)
		static void Log(Vector2 vector);

		// Logs a Message. (Vector3)
		static void Log(Vector3 vector);

		// Logs a Message. (Vector4)
		static void Log(Vector4 vector);

		// Logs a Message. (Matrix2)
		static void Log(Matrix2 matrix);

		// Logs a Message. (Matrix3)
		static void Log(Matrix3 matrix);

		// Logs a Message. (Matrix4)
		static void Log(Matrix4 matrix);

		// Logs a Message on a single line.
		static void LogLine(std::string message);

		// Logs a Message on a single line.
		static void LogLine(const char* message);

		// Logs a Message on a single line. (Bool)
		static void LogLine(bool value);

		// Logs a Message on a single line. (Float)
		static void LogLine(float value);

		// Logs a Message on a single line. (Int)
		static void LogLine(int value);

		// Logs a Message on a single line. (Vector1)
		static void LogLine(Vector1 vector);

		// Logs a Message on a single line. (Vector2)
		static void LogLine(Vector2 vector);

		// Logs a Message on a single line. (Vector3)
		static void LogLine(Vector3 vector);

		// Logs a Message on a single line. (Vector4)
		static void LogLine(Vector4 vector);

		// Logs a Message on a single line. (Matrix2)
		static void LogLine(Matrix2 matrix);

		// Logs a Message on a single line. (Matrix3)
		static void LogLine(Matrix3 matrix);

		// Logs a Message on a single line. (Matrix4)
		static void LogLine(Matrix4 matrix);

		// Logs Warning a Message.
		static void LogWarning(std::string message);

		// Logs a Warning Message on a single line.
		static void LogWarningLine(std::string message);

		// Logs Error a Message.
		static void LogError(std::string message);

		// Logs a Error Message on a single line.
		static void LogErrorLine(std::string message);

		// Logs a OpenGL Error.
		static void LogGLError(std::string message);

		// Logs a OpenGL Error on a single line.
		static void LogGLErrorLine(std::string message);

		// Ends the Log Line.
		static void EndLine() { std::cout << std::endl; Debug::m_HasEnteredLog = false; };
	};
}