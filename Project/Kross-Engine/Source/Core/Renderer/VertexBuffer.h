/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

namespace Kross
{
	class KROSS_API VertexBuffer
	{
	private:
		unsigned int m_VertexBufferID;

	protected:
		// Returns the ID of the Vertex Buffer.
		unsigned int GetVertexBufferID() const { return m_VertexBufferID; };

	public:
		VertexBuffer();
		~VertexBuffer();

		// Binds the Vertex Buffer.
		void Bind();

		// Un Binds the all Vertex Buffers.
		static void UnBind();

		// Adds data to the Vertex Buffer.
		void SetVertexData(const void* data, unsigned int size);
	};
}