/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../../Core.h"

#include "Character.h"

namespace Kross
{
	class KROSS_API Font
	{
	private:
		Font() : 
			m_Name			(""), 
			m_Texture		(nullptr), 
			m_Characters	(std::vector<Character*>()),
			m_Sprites		(std::vector<Sprite*>())
		{};
		~Font();

		std::string m_Name;
		Texture* m_Texture;
		std::vector<Character*> m_Characters;
		std::vector<Sprite*> m_Sprites;

	protected:
		friend class TextRenderer;
		friend class BatchRenderer;

		// Gets the Texture of the Font.
		Texture* GetTexture() const { return m_Texture; };

		// Sets the Texture of the Font.
		void SetTexture(Texture* texture) { m_Texture = texture; };

		// Adds a Character to the Font.
		void AttachCharacter(Character* character) { m_Characters.push_back(character); };

		// Gets a Character from the Font. (BY CHAR)
		Character* GetCharacter(unsigned char character) const;

		// Gets a Character from the Font. (BY STRING)
		Character* GetCharacter(const std::string& character) const;

		// Gets a Character from the Font. (BY INDEX)
		Character* GetCharacter(int index) const;

	public:
		// Creates a Font.
		static Font* OnCreate(Texture* texture, int width, int height, const std::string& name);

		// Deletes the specified Font.
		static void OnDestroy(Font* font);

		// Gets the Name of the Font.
		const std::string GetName() const { return m_Name; };

		// Sets the Name of the Font.
		void SetName(const std::string& name) { m_Name = name; };
	};
}