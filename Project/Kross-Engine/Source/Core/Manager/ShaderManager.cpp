/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#include "ShaderManager.h"

namespace Kross
{
	List<Shader*>		ShaderManager::s_Shaders =		List<Shader*>();
	ShaderManager*		ShaderManager::s_Instance =		nullptr;

	ShaderManager::~ShaderManager()
	{
		/* Destroy all the Shaders. */
		for (int i = 0; i < s_Shaders.size(); i++)
		{
			Shader::OnDestroy(s_Shaders[i]);
			s_Shaders[i] = nullptr;
		}

		/* Clean up Memory. */
		s_Shaders.clear();
		s_Shaders.~vector();
	}

	void ShaderManager::OnCreate()
	{
		if (!s_Instance)
			s_Instance = KROSS_NEW ShaderManager();
	}

	void ShaderManager::OnDestroy()
	{
		if (s_Instance)
			delete s_Instance;
	}

	void ShaderManager::AttachShader(Shader* shader)
	{
		/* Incase for some reason the Shader doesn't exist. Early out. */
		if (!shader)
			return;

		/* Check for duplicates. */
		for (int i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i] == shader)
				return;
		}

		/* If no duplicate was found, add it. */
		s_Shaders.push_back(shader);
	}

	void ShaderManager::DetachShader(Shader* shader)
	{
		/* Incase for some reason the Shader doesn't exist. Early out. */
		if (!shader)
			return;

		/* Check for Shader. */
		for (int i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i] == shader)
			{
				/* Remove the Shader. */
				Shader::OnDestroy(shader);
				s_Shaders.erase(s_Shaders.begin() + i);
				return;
			}
		}
	}

	void ShaderManager::OnReloadShader(Shader* shader)
	{
		/* Reload the Shader. */
		shader = Shader::OnReload(shader);
	}

	void ShaderManager::OnUpdateShaderVPMatrix(Matrix4 viewMatrix, Matrix4 projectionMatrix)
	{
		/* Update all Shaders View and Projection Matrix. */
		for (int i = 0; i < s_Shaders.size(); i++)
		{
			if (s_Shaders[i]->GetFlag() == ShaderFlag::None)
			{
				s_Shaders[i]->SetUniform("u_View", viewMatrix);
				s_Shaders[i]->SetUniform("u_Projection", projectionMatrix);
			}
		}
	}
}