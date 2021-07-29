/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../../Core.h"

#include "../EditorWindow.h"
#include "AssetType.h"

namespace Kross
{
	class KROSS_API AssetPanel : public EditorWindow
	{
	private:
		AssetType m_Type;

	protected:

	public:
		AssetPanel() :
			m_Type(AssetType::None)
		{};
		~AssetPanel() {};

		// Sets the Asset Directory Window Flags.
		void SetFlags() override;

		// Initialises the Window.
		void OnStart() override;

		// Adds the Data to the Window. 
		void Attach() override;
	};
}