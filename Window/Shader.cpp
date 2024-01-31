#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"

// I NEED TO MODIFY ALL OF THIS TO ALLOW MULTIPLE SHADERS TO BE UTILIZED
// MAKE SMALL ADJUSTMENTS AND MAKE SURE THEY WORK BEFORE MAKING ANY SUBSTANTIAL CHANGES

Shader::Shader(const std::string& fragmentfilepath,
    ShaderType type,
    const std::string vertexfilepath)
	: m_VertexPath(vertexfilepath), shaderType(type), m_FragmentPath(fragmentfilepath), m_RendererID(0)
{
    // REFACTOR CODE

    // Default, Contains both shaders
    ShaderProgramSource source = ParseShader(m_VertexPath, m_FragmentPath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

// Contains two inputs. Need one that contains a single input
ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath) {

    std::ifstream Vert_Stream(vertexPath);
    std::ifstream Frag_Stream(fragmentPath);

    std::string line;
    std::stringstream ss[2];


    while (getline(Vert_Stream, line)) {
        if (line.find("#shader") != std::string::npos) {
                shaderType = ShaderType::VERTEX;
        }
        else {
            // Copies the line onto the stream
            ss[(int)shaderType] << line << '\n';
        }
    }

    while (getline(Frag_Stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            shaderType = ShaderType::FRAGMENT;
        }
        else {
            // Copies the line onto the stream
            ss[(int)shaderType] << line << '\n';
        }
    }

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
// This needs to be modified such that it can be parameterized
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());

    // The location of the shader program
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

void Shader::Bind() const
{
    // m_RendererID is just an integer, used to find the location of the program.
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float v0)
{
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1i(const std::string& name, int value){
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

// Able to set shader variables within source code
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    
    /*if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist." << std::endl;*/
    m_UniformLocationCache[name] = location;

    return location;
}
