/*
 *  Author: Deklyn Palmer.
 *  Editors:
 *      - Deklyn Palmer.
 */

#pragma once

#include "../../Core.h"

#include "ShaderType.h"

#include "../Image/Texture.h"
#include "../../Math/Math.h"

namespace Kross
{
	class KROSS_API Shader
	{
	private:
		Shader();
		~Shader();

		unsigned int m_ShaderID;
		// Used to store the Locations of the Uniforms for performace.
		std::unordered_map<std::string, int> m_UniformCache;

		std::string m_Name, m_VertexFilepath, m_FragmentFilepath, m_GeometryFilepath;
		ShaderType m_Type;

	protected:
		friend class ResourceManager;

		// Gets the Vertex Shader Filepath.
		const std::string GetVertexFilepath() const { return m_VertexFilepath; };

		// Gets the Fragment Shader Filepath.
		const std::string GetFragmentFilepath() const { return m_FragmentFilepath; };

		// Gets the Geometry Shader Filepath.
		const std::string GetGeometryFilepath() const { return m_GeometryFilepath; };

		// Gets the Shader ID.
		const unsigned int GetShaderID() const { return m_ShaderID; };

		// Complies a certain shader type.
		static unsigned int CompileShader(std::string source, int type);

		// Gets the Uniform Location.
		int GetUniformLocation(const std::string& variable);

		// Sets the Shader Name.
		void SetName(const std::string& name) { m_Name = name; };

		// Gets the Linking Status of the Linking of the Shader.
		void LinkShader();

		// Sets the Vertex Shader Filepath.
		void SetVertexFilepath(const std::string& filepath) { m_VertexFilepath = filepath; };

		// Sets the Fragment Shader Filepath.
		void SetFragmentFilepath(const std::string& filepath) { m_FragmentFilepath = filepath; };

		// Sets the Geometry Shader Filepath.
		void SetGeometryFilepath(const std::string& filepath) { m_GeometryFilepath = filepath; };

		// Combines the Vertex and Fragment Shaders.
		void AttachShaders(unsigned int vertex, unsigned int fragment);

		// Combines the Vertex, Fragment and Geometry Shaders.
		void AttachShaders(unsigned int vertex, unsigned int fragment, unsigned int geometry);

	public:
		// Binds the Shader.
		void Bind();

		// Un Binds all Shaders.
		static void UnBind();

		// Gets the Shader Name.
		const std::string GetName() const { return m_Name; };

		// Creates a Shader through file location.
		static Shader* OnCreate(const std::string& vertexFilepath, const std::string& fragmentFilepath, const std::string& name);

		// Creates a Shader through file location.
		static Shader* OnCreate(const std::string& vertexFilepath, const std::string& fragmentFilepath, const std::string& geometryFilepath, const std::string& name);

		// Reloads the Shader given.
		static Shader* OnReload(Shader* shader);

		// Destroys the Shader specified.
		static void OnDestroy(Shader* shader);

		// Sets the Shader Type.
		void SetType(ShaderType type) { m_Type = type; };

		// Gets the Shader Type.
		ShaderType GetType() const { return m_Type; };

		// Sets a Bool variable inside the Shader.
		void SetUniform(const std::string& variable, bool value);

		// Sets a Integer variable inside the Shader.
		void SetUniform(const std::string& variable, int value);

		// Sets a Float variable inside the Shader.
		void SetUniform(const std::string& variable, float value);

		// Sets a 2 Vector variable inside the Shader.
		void SetUniform(const std::string& variable, Vector2 vector);

		// Sets a 3 Vector variable inside the Shader.
		void SetUniform(const std::string& variable, Vector3 vector);

		// Sets a 4 Vector variable inside the Shader.
		void SetUniform(const std::string& variable, Vector4 vector);

		// Sets a 2x2 Matrix variable inside the Shader.
		void SetUniform(const std::string& variable, Matrix2 matrix);

		// Sets a 3x3 Matrix variable inside the Shader.
		void SetUniform(const std::string& variable, Matrix3 matrix);

		// Sets a 4x4 Matrix variable inside the Shader.
		void SetUniform(const std::string& variable, Matrix4 matrix);

		// Sets a Texture variable inside the Shader. (SAMPLER2D IN THIS CASE)
		void SetUniform(const std::string& variable, Texture* texture);
	};
}