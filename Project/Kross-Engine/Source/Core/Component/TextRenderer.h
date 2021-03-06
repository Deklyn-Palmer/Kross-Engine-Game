/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

#include "Renderer.h"
#include "../Renderer/Text/Font.h"
#include "../Renderer/Text/TextAlignment.h"

namespace Kross
{
	class KROSS_API TextRenderer : public Renderer
	{
	private:
		std::string m_Text, m_PreviousText;

		Font* p_Font;
		Font* p_PreviousFont;
		std::vector<Character*> m_TextCharacters;

		TextAlignment m_Alignment;

		Colour m_Colour;
		float m_Size;

	protected:
		/* Friend classing Batch. */
		template<typename Type> friend class Batch;
		/* ^ I Hate this even more. */

		// Text Renderer Update Method.
		void OnUpdate() override;

	public:
		TextRenderer() :
			m_Colour(Colour(1.0f)),
			m_Size				(1.0f),
			p_Font				(nullptr),
			p_PreviousFont		(nullptr),
			m_Text				(std::string()),
			m_PreviousText		(std::string()),
			m_TextCharacters	(std::vector<Character*>()),
			m_Alignment			(TextAlignment::Center)
		{};
		~TextRenderer();

		// Gets the Current Text.
		const std::string GetText() const { return m_Text; };

		// Gets the Text Font.
		Font* GetFont() const { return p_Font; };

		// Gets the Text's Text Alignment.
		TextAlignment GetTextAlignment() const { return m_Alignment; };

		// Gets the Text Colour.
		Colour GetColour() const { return m_Colour; };

		// Gets the Text Size. (SIZE OF CHARACTERS IN TERMS OF WIDTH AND HEIGHT)
		const float GetTextSize() const { return m_Size; };

		// Sets the Text's Text to Render.
		void SetText(const std::string& text) { m_Text = text; };

		// Sets the Text Font.
		void SetFont(Font* font) { p_Font = font; };

		// Sets the Text's Text Alignment.
		void SetTextAlignment(TextAlignment alignment) { m_Alignment = alignment; };

		// Sets the Text Colour.
		void SetColour(Colour colour) { m_Colour = colour; };

		// Sets the Text Size. (SIZE OF CHARACTERS IN TERMS OF WIDTH AND HEIGHT)
		void SetTextSize(float size) { m_Size = size; };
	};
}