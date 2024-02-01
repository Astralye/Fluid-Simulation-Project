#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

// Retrieves shader and converts to string
ShaderProgramSource Shader::ParseShader(const std::string& filePath) {

    std::ifstream stream(filePath);
  
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        std::cout << line << std::endl;

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    std::cout << ss[0].str() << "\n" << std::endl;
    std::cout << ss[1].str() << std::endl;
    return { ss[0].str(),ss[1].str() };
}

// Takes in source code as string and type
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

    // Create shader of type parameter
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();

    // sets source code for 'id' to be array within 'src'
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    // If there was a problem with compiling shader
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

// Takes shader code and creates a program
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attaching is like putting them together, but are not bound.
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }
void Shader::Unbind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform1i(const std::string& name, int value) { GLCall(glUniform1i(GetUniformLocation(name), value)); }
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) { GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3)); }

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) { GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0])); }

int Shader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    //int samplers[2] = { 0,1 };
   /* if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist." << std::endl;*/
    m_UniformLocationCache[name] = location;
    //glUniform1iv(location, 2, samplers);
    return location;
}