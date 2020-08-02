#include "shader.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

namespace wlp::gl {

    Shader::Shader(const std::string &vshPath, const std::string &fshPath) {
        this->m_programId = this->load(vshPath, fshPath);
    }

    Shader::~Shader() {
        glDeleteProgram(this->m_programId);
    }

    void Shader::use() {
        this->m_uniformLocations.clear();

        glUseProgram(this->m_programId);
    }


    GLuint Shader::load(const std::string &vshPath, const std::string &fshPath) {
        // Create shader
        GLuint vshId = glCreateShader(GL_VERTEX_SHADER);
        GLuint fshId = glCreateShader(GL_FRAGMENT_SHADER);

        // Load vertex shader source
        std::string vshCode;
        std::ifstream vshFile(vshPath, std::ios::in);

        if(vshFile.is_open()){
            std::stringstream sstr;
            sstr << vshFile.rdbuf();
            vshCode = sstr.str();
            vshFile.close();
        } else return 0;

        // Load fragment shader source
        std::string fshCode;
        std::ifstream fshFile(fshPath, std::ios::in);
        if(fshFile.is_open()){
            std::stringstream sstr;
            sstr << fshFile.rdbuf();
            fshCode = sstr.str();
            fshFile.close();
        }

        GLint result = GL_FALSE;
        int logLength;

        // Compile vertex shader
        const char *vshCodePtr = vshCode.c_str();
        glShaderSource(vshId, 1, &vshCodePtr, nullptr);
        glCompileShader(vshId);

        glGetShaderiv(vshId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vshId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0){
            char *error = new char[logLength + 1];
            glGetShaderInfoLog(vshId, logLength, nullptr, error);
            printf("[*] %s\n", &error[0]);
            delete[] error;
        }

        // Compile fragment shader
        char const *fragCodePtr = fshCode.c_str();
        glShaderSource(fshId, 1, &fragCodePtr , nullptr);
        glCompileShader(fshId);

        glGetShaderiv(fshId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(fshId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0){
            char *error = new char[logLength + 1];
            glGetShaderInfoLog(fshId, logLength, nullptr, error);
            printf("[*] %s\n", error);
            delete[] error;
        }


        // Create program and attach shaders to it
        GLuint programId = glCreateProgram();
        glAttachShader(programId, vshId);
        glAttachShader(programId, fshId);
        glLinkProgram(programId);

        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0){
            char *error = new char[logLength + 1];
            glGetProgramInfoLog(programId, logLength, nullptr, error);
            printf("[*] %s\n", error);
            delete[] error;
        }

        glDetachShader(programId, vshId);
        glDetachShader(programId, fshId);

        glDeleteShader(vshId);
        glDeleteShader(fshId);

        return programId;
    }

    void Shader::uniforms(std::initializer_list<const char*> uniformNames) {
        for (auto name : uniformNames)
            this->m_uniformLocations.insert({ name, glGetUniformLocation(this->m_programId, name) });
    }

    void Shader::setUniform(const char *uniformName, GLfloat f1) {
        glUniform1f(this->m_uniformLocations[uniformName], f1);
    }
    void Shader::setUniform(const char *uniformName, GLint i1) {
        glUniform1i(this->m_uniformLocations[uniformName], i1);
    }
    void Shader::setUniform(const char *uniformName, GLfloat f1, GLfloat f2) {
        glUniform2f(this->m_uniformLocations[uniformName], f1, f2);
    }
    void Shader::setUniform(const char *uniformName, GLint i1, GLint i2) {
        glUniform2i(this->m_uniformLocations[uniformName], i1, i2);
    }
    void Shader::setUniform(const char *uniformName, GLfloat f1, GLfloat f2, GLfloat f3) {
        glUniform3f(this->m_uniformLocations[uniformName], f1, f2, f3);
    }
    void Shader::setUniform(const char *uniformName, GLint i1, GLint i2, GLint i3) {
        glUniform3i(this->m_uniformLocations[uniformName], i1, i2, i3);
    }
    void Shader::setUniform(const char *uniformName, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4) {
        glUniform4f(this->m_uniformLocations[uniformName], f1, f2, f3, f4);
    }
    void Shader::setUniform(const char *uniformName, GLint i1, GLint i2, GLint i3, GLint i4) {
        glUniform4i(this->m_uniformLocations[uniformName], i1, i2, i3, i4);
    }
}