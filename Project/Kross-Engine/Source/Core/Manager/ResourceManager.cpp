/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#include "ResourceManager.h"

namespace Kross
{
	ResourceManager*	ResourceManager::s_Instance =		nullptr;

	ResourceManager::~ResourceManager()
	{
		/* Clean up all the Geometry from memory. */
		for (int i = 0; i < m_Geometry.size(); i++)
		{
			delete m_Geometry[i];
			m_Geometry[i] = nullptr;
		}

		/* Clean up the list. */
		m_Geometry.clear();
		m_Geometry.~vector();

		/* Clean up all the Shaders from memory. */
		for (int i = 0; i < m_Shaders.size(); i++)
		{
			Shader::OnDestroy(m_Shaders[i]);
			m_Shaders[i] = nullptr;
		}

		/* Clean up the list. */
		m_Shaders.clear();
		m_Shaders.~vector();

		/* Clean up all the Sprites from memory. */
		for (int i = 0; i < m_Sprites.size(); i++)
		{
			Sprite::OnDestroy(m_Sprites[i]);
			m_Sprites[i] = nullptr;
		}

		/* Clean up the list. */
		m_Sprites.clear();
		m_Sprites.~vector();

		/* Clean up all the Textures from memory. */
		for (int i = 0; i < m_Textures.size(); i++)
		{
			Texture::OnDestroy(m_Textures[i]);
			m_Textures[i] = nullptr;
		}

		/* Clean up the list. */
		m_Textures.clear();
		m_Textures.~vector();

		/* Clean up all the Materials from memory. */
		for (int i = 0; i < m_Materials.size(); i++)
		{
			Material::OnDestroy(m_Materials[i]);
			m_Materials[i] = nullptr;
		}

		/* Clean up the list. */
		m_Materials.clear();
		m_Materials.~vector();

		/* Clean up all the Fonts from memory. */
		for (int i = 0; i < m_Fonts.size(); i++)
		{
			Font::OnDestroy(m_Fonts[i]);
			m_Fonts[i] = nullptr;
		}

		/* Clean up the list. */
		m_Fonts.clear();
		m_Fonts.~vector();

		/* Destroy the Atlas. */
		Atlas::OnDestroy(p_Atlas);
	}

	void ResourceManager::OnCreate()
	{
		if (!s_Instance)
			s_Instance = KROSS_NEW ResourceManager();
	}

	void ResourceManager::OnDestroy()
	{
		if (s_Instance)
			delete s_Instance;
	}
}