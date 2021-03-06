/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../../Core.h"

#include "../../Math/Math.h"
#include "../Image/Sprite.h"

namespace Kross
{
	class KROSS_API Character
	{
	private:
		Character() :
			m_Sprite		(nullptr),
			m_Position		(Vector2(0.0f)),
			m_Scale			(Vector2(1.0f))
		{};
		~Character()
		{
			m_Sprite = nullptr;
		};

		Sprite* m_Sprite;
		Vector2 m_Position, m_Scale;

	protected:
		friend class Font;
		friend class TextRenderer;

		/* Friend classing Batch. */
		template<typename Type> friend class Batch;

		// Sets the position in World Space or Screen Space of the Character. (TEXT RENDERER USE)
		void SetPosition(Vector2 position) { m_Position = position; };

		// Sets the scale in World Space or Screen Space of the Character. (TEXT RENDERER USE)
		void SetScale(Vector2 scale) { m_Scale = scale; };

		// Sets the Sprite of the Character. (FONT USE)
		void SetSprite(Sprite* sprite) { m_Sprite = sprite; };

		// Gets the position of the Character. (BATCH USE)
		const Vector2 GetPosition() const { return m_Position; };

		// Gets the scale of the Character. (BATCH USE)
		const Vector2 GetScale() const { return m_Scale; };

		// Gets the Sprite of the Character. (FONT USE)
		Sprite* GetSprite() const { return m_Sprite; };
	public:

	};
}