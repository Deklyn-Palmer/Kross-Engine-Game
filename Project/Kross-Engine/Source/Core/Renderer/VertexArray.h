/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../Core.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Kross
{
	class KROSS_API VertexArray
	{
	private:
		unsigned int m_VertexArrayID;

	protected:
		// Gets the ID of the Vertex Array.
		unsigned int GetVertexArrayID() const { return m_VertexArrayID; };

	public:
		VertexArray();
		~VertexArray();

		// Binds the Vertex Array for using.
		void Bind();

		// Un Binds the all VertexArrays. 
		static void UnBind();

		// Configures a VertexBuffer to a Layout.
		void AttachVertexBufferToLayout(VertexBuffer& vertexBuffer, VertexBufferLayout* layout);
	};
}