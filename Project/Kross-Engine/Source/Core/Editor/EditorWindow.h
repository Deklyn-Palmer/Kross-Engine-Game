/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

#include "../Scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace Kross
{
	class KROSS_API EditorWindow
	{
	protected:
		friend class Editor;

		Scene* p_Scene;

		std::string m_Title;

		int m_WindowFlags;

		// Sets the Window Flags.
		virtual void SetFlags() = 0;

		// Initialises basic Variables. 
		virtual void OnStart() = 0;

		// Attaches Render Data of the Window.
		virtual void Attach() = 0;

	public:
		EditorWindow() :
			p_Scene			(nullptr),
			m_Title			(std::string()),
			m_WindowFlags	(ImGuiWindowFlags_None)
		{};
		virtual ~EditorWindow() {};
	};
}

#include "Windows/SceneHierarchy.h"
#include "Windows/ObjectEditor.h"
#include "Windows/AssetPreview.h"
#include "Windows/MainMenu.h"
#include "Windows/AssetDirectory.h"
#include "Windows/AssetPanel.h"