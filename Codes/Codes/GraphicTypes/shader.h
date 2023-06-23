#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <string>
#include <fstream>

class Texture;
class Vec3;
class Vec2;
class Color;

class Shader {
public:
    Shader();
    
    void init(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    void init(const std::string &shaderPath);
    unsigned int getShaderId() const;
    void useProgram() const;
    void setUniform(const std::string &uniformName, const glm::mat4 &mat) const;
    void setUniform(const std::string &uniformName, Vec3 vec) const;
    void setUniform(const std::string &uniformName, Vec2 vec) const;
    void setUniform(const std::string &uniformName, Color color) const;
    void setUniform(const std::string &uniformName, int num) const;
    void setUniform(const std::string &uniformName, bool b) const;
    void setUniform(const std::string &uniformName, const Texture &texture, int textureUniformIndex, bool arrayTexture = false) const;
    void setUniform(const std::string &uniformName, unsigned int textureId, int textureUniformIndex, bool arrayTexture = false) const;
    void release();

    Shader(const Shader&) = delete;
    Shader &operator = (const Shader&) = delete;
    
private:
    unsigned int shaderId;
    bool initialized = false;
    bool released = false;
};

#endif
