#ifndef MESH_H
#define MESH_H

#include <vector>

class Mesh {
public:
    Mesh();

    void init3d(const std::vector<float> &verticies, bool drawLine = false);
    void init3d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine = false);
    void init2d(const std::vector<float> &verticies, bool drawLine = false);
    void init2d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine = false);

    void set3d(const std::vector<float> &verticies, bool drawLine = false);
    void set3d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine = false);
    void set3dLayers(const std::vector<float> &verticies);
    void set2d(const std::vector<float> &verticies, bool drawLine = false);
    void set2d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine = false);

    void draw() const;

    void release();

    Mesh(const Mesh&) = delete;
    Mesh &operator = (const Mesh&) = delete;
    
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    bool initialized = false;
    bool VAOInitialized = false;
    bool VBOInitialized = false;
    bool EBOInitialized = false;
    int numberOfVerticies = 0;
    int numberOfIndicies = 0;
    bool drawLine = false;
    bool emptyMesh = true;
    bool released = false;
};

#endif
