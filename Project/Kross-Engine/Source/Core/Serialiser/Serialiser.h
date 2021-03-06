/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

namespace Kross
{
    template<typename Type>
    class KROSS_API Serialiser {};
}

#include "TextureSerialiser.h"
#include "ShaderSerialiser.h"
#include "AudioSourceSerialiser.h"
#include "TileSetSerialiser.h"
#include "SceneHierarchySerialiser.h"
#include "EditorSerialiser.h"
//#include "ObjectSerialiser.h"