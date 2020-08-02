#pragma once

#include <unordered_map>
#include <glad/gl.h>
#include <string>

namespace wlp::gl {

    /**
     * A shader program consisting of a vertex and a fragment shader
     */
    class Shader {
    public:
        /**
         * Constructor
         * @param vshPath Vertex shader path
         * @param fshPath Fragment shader path
         */
        Shader(const std::string &vshPath, const std::string &fshPath);
        ~Shader();

        /**
         * Bind shader
         */
        void use();

        /**
         * Specify all uniforms used in this shader to get their locations
         * @param uniformNames Uniform names
         */
        void uniforms(std::initializer_list<const char*> uniformNames);

        void setUniform(const char *uniformName, GLfloat f1);
        void setUniform(const char *uniformName, GLint i1);
        void setUniform(const char *uniformName, GLfloat f1, GLfloat f2);
        void setUniform(const char *uniformName, GLint i1, GLint i2);
        void setUniform(const char *uniformName, GLfloat f1, GLfloat f2, GLfloat f3);
        void setUniform(const char *uniformName, GLint i1, GLint i2, GLint i3);
        void setUniform(const char *uniformName, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
        void setUniform(const char *uniformName, GLint i1, GLint i2, GLint i3, GLint i4);

        template<typename... T>
        void setUniformFloatArray(const char *uniformName, size_t size, T... data) {
            GLfloat array[] = { data... };
            glUniform1fv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformIntArray(const char *uniformName, size_t size, T... data) {
            GLint array[] = { data... };
            glUniform1iv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformVec2Array(const char *uniformName, size_t size, T... data) {
            GLfloat array[] = { data... };
            glUniform2fv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformIVec2Array(const char *uniformName, size_t size, T... data) {
            GLint array[] = { data... };
            glUniform2iv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformVec3Array(const char *uniformName, size_t size, T... data) {
            GLfloat array[] = { data... };
            glUniform3fv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformIVec3Array(const char *uniformName, size_t size, T... data) {
            GLint array[] = { data... };
            glUniform3iv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformVec4Array(const char *uniformName, size_t size, T... data) {
            GLfloat array[] = { data... };
            glUniform4fv(this->m_uniformLocations[uniformName], size, array);
        }
        template<typename... T>
        void setUniformIVec4Array(const char *uniformName, size_t size, T... data) {
            GLint array[] = { data... };
            glUniform4iv(this->m_uniformLocations[uniformName], size, array);
        }

    private:
        GLuint m_programId;
        std::unordered_map<std::string, GLuint> m_uniformLocations;

        GLuint load(const std::string &vshPath, const std::string &fshPath);
        void cacheUniformLocation(const char *uniformName);
    };

}