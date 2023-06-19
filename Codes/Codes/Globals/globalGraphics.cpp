#include <Codes/GraphicTypes/shader.h>
#include <Codes/GraphicTypes/mesh.h>

namespace GlobalGraphics {
    Mesh mesh_windowRect;
    Shader shader_windowRect;

    void init() {
        std::vector<float> windowRectVerticies = {
            -1,  1,
            1,  1,
            -1, -1,
            
            1,  1,
            1, -1,
            -1, -1
        };
        mesh_windowRect.init2d(windowRectVerticies);
        shader_windowRect.init("Shaders/windowRect");
    }

    void release() {
        mesh_windowRect.release();
        shader_windowRect.release();
    }
}
