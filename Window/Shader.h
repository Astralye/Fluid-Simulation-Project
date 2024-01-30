	#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

// Struct, function can return multiple values
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_VertexPath;
	std::string m_FragmentPath;

	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	Shader(const std::string &fragmentfilepath, 
		ShaderType type = ShaderType::NONE,
		std::string vertexfilepath = "res/shaders/Vertex.shader");
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms

	// Integers
	void SetUniform1i(const std::string& name, int value);

	// Floats
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 &matrix);

private: 
	ShaderType shaderType;

	ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name) const;
};