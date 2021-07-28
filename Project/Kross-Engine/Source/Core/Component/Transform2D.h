/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

#include "../Math/Math.h"

#include "Component.h"

namespace Kross
{
	class KROSS_API Transform2D : public Component
	{
	private:
	protected:
		friend class SpriteRenderer;
		friend class Rigidbody2D;

		/* Friend classing Batch. */
		template<typename Type> friend class Batch;
		/* ^ I Hate this even more. */

		// Transform2D Start Method.
		void OnStart() override;

		// Transform2D Update Method.
		void OnUpdate() override;

		// Gets the Model Matrix.
		const Matrix4 GetModelMatrix() const { return m_Model; };

		Matrix4 m_Model;

	public:
		Transform2D() :
			m_Rotation	(0.0f),
			m_Position	(Vector2(0.0f)),
			m_Model		(Matrix4(1.0f)),
			m_Scale		(Vector2(1.0f))
		{};
		~Transform2D() {};

		Vector2 m_Position;
		Vector2 m_Scale;

		float m_Rotation;
	};
}