#include <Codes/GraphicTypes/shader.h>

#include <Codes/print.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <Codes/GraphicTypes/texture.h>
#include <Codes/Types/vec3.h>
#include <Codes/Types/vec2.h>
#include <Codes/Types/color.h>

unsigned int compileShader(const std::string &path, bool isFragmentShader) {
    std::ifstream file(path);
    if (file.fail()) {
        PRINT(path);
        PRINTLN("not found");
        return -1;
    }
    std::string shaderCodeString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char *shaderCode = shaderCodeString.c_str();
    unsigned int shader;

    shader = glCreateShader(isFragmentShader ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        PRINTLN("shader compile error");
        PRINTLN(path);
        PRINTLN(infoLog);
        return -1;
    }

    return shader;
}

void linkShaderProgram(unsigned int shaderProgram, const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    unsigned int vertexShader = compileShader(vertexShaderPath, false);
    unsigned int fragmentShader = compileShader(fragmentShaderPath, true);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        PRINTLN("shader linking error");
        PRINTLN(vertexShaderPath);
        PRINTLN(fragmentShaderPath);
        PRINTLN(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader() {}

void Shader::init(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    if (initialized) {
        return;
    }

    shaderId = glCreateProgram();
    linkShaderProgram(shaderId, vertexShaderPath, fragmentShaderPath);
    initialized = true;
}

void Shader::init(const std::string &shaderPath) {
    if (initialized) {
        return;
    }

    shaderId = glCreateProgram();
    linkShaderProgram(shaderId, shaderPath + "_vertex.glsl", shaderPath + "_fragment.glsl");
    initialized = true;
}

unsigned int Shader::getShaderId() const {
    if (!initialized) {
        PRINTLN("cant get shaderId of uninitialized shader");
        return 0;
    }
    if (released) {
        PRINTLN("cant get shaderId of released shader");
        return 0;
    }
    return shaderId;
}

void Shader::useProgram() const {
    if (!initialized) {
        PRINTLN("cant use uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant use released shader");
        return;
    }
    glUseProgram(shaderId);
}

void Shader::setUniform(const std::string &uniformName, const glm::mat4 &mat) const {
    if (!initialized) {
        PRINTLN("cant set mat4 uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set mat4 uniform of released shader");
        return;
    }
    glUniformMatrix4fv(glGetUniformLocation(shaderId, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(const std::string &uniformName, Vec3 vec) const {
    if (!initialized) {
        PRINTLN("cant set vec3 uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set vec3 uniform of released shader");
        return;
    }
    glUniform3f(glGetUniformLocation(shaderId, uniformName.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setUniform(const std::string &uniformName, Vec2 vec) const {
    if (!initialized) {
        PRINTLN("cant set vec2 uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set vec2 uniform of released shader");
        return;
    }
    glUniform2f(glGetUniformLocation(shaderId, uniformName.c_str()), vec.x, vec.y);
}

void Shader::setUniform(const std::string &uniformName, Color color) const {
    if (!initialized) {
        PRINTLN("cant set color uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set color uniform of released shader");
        return;
    }
    glUniform4f(glGetUniformLocation(shaderId, uniformName.c_str()), color.r, color.g, color.b, color.a);
}

void Shader::setUniform(const std::string &uniformName, int num) const {
    if (!initialized) {
        PRINTLN("cant set int uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set int uniform of released shader");
        return;
    }
    glUniform1i(glGetUniformLocation(shaderId, uniformName.c_str()), num);
}

void Shader::setUniform(const std::string &uniformName, bool b) const {
    if (!initialized) {
        PRINTLN("cant set bool uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set bool uniform of released shader");
        return;
    }
    glUniform1i(glGetUniformLocation(shaderId, uniformName.c_str()), b);
}

void Shader::setUniform(const std::string &uniformName, const Texture &texture, int textureUniformIndex, bool arrayTexture) const {
    if (!initialized) {
        PRINTLN("cant set texture uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set texture uniform of released shader");
        return;
    }
    setUniform(uniformName, texture.getTextureId(), textureUniformIndex, arrayTexture);
}

void Shader::setUniform(const std::string &uniformName, unsigned int textureId, int textureUniformIndex, bool arrayTexture) const {
    if (!initialized) {
        PRINTLN("cant set texture uniform of uninitialized shader");
        return;
    }
    if (released) {
        PRINTLN("cant set texture uniform of released shader");
        return;
    }
    glUniform1i(glGetUniformLocation(shaderId, uniformName.c_str()), textureUniformIndex);
    glActiveTexture(GL_TEXTURE0 + textureUniformIndex);
    glBindTexture(arrayTexture? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D, textureId);
}

void Shader::release() {
    if (released) {
        return;
    }
    
    if (!initialized) {
        PRINTLN("cant release uninitialized shader");
        return;
    }

    glDeleteProgram(shaderId);

    released = true;
}
