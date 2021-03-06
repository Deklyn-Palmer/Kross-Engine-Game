/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

/* --- Global Header --- */

/* --- README --- */
/*
 * This must be used everywhere in order for the Engine API to be put into the DLL for importation on the Game Project side of things
 * Eg. class KROSS_API Classname;
 */

/* Include Files */
/*
 * Eg. Standard Library files like vector and other things.
 * Do Not include API FILES HERE, bad times lie ahead if you do.
 */

/* Memory Leak detection. */
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdio.h>  
#include <crtdbg.h>
typedef _CrtMemState MemoryState;

#include <vector>

/* --- Random --- */
#include <random>
#include <time.h>
#include <stdlib.h> /* srand, rand */

/* -------------- */

#include <iostream>

#include <string>
#include <sstream>

#include <filesystem>

#include <unordered_map>
#include <fstream>

#define KROSS_ASSERT(x) if(!(x)) __debugbreak();

#if defined _DEBUG || defined KROSS_DEBUG
    #define KROSS_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    // allocations to be of _CLIENT_BLOCK type
#else
    #define KROSS_NEW new
#endif

/* --- DLL Export and Import Functionality --- */

#ifdef KROSS_PLATFORM_WINDOWS
    #ifdef KROSS_ENGINE
        #define KROSS_API __declspec(dllexport)
    #else
        #define KROSS_API __declspec(dllimport)
    #endif
#else
    #error Kross-Engine only supports Windows!
#endif

#include "Renderer/ErrorHandle.h"