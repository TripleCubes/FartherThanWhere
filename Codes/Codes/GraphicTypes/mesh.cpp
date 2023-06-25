#include <Codes/GraphicTypes/mesh.h>

#include <glad/glad.h>
#include <Codes/print.h>

Mesh::Mesh() {}

void Mesh::init3d(const std::vector<float> &verticies, bool drawLine) {
    if (initialized) {
        return;
    }

    set3d(verticies, drawLine);
}

void Mesh::init3d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine) {
    if (initialized) {
        return;
    }

    set3d(verticies, indicies, drawLine);
}

void Mesh::init2d(const std::vector<float> &verticies, bool drawLine) {
    if (initialized) {
        return;
    }

    set2d(verticies, drawLine);
}

void Mesh::init2d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine) {
    if (initialized) {
        return;
    }

    set2d(verticies, indicies, drawLine);
}

void Mesh::set3d(const std::vector<float> &verticies, bool drawLine) {
    if (released) {
        PRINTLN("cant set released mesh");
        return;
    }

    if (verticies.size() == 0) {
        if (VAOInitialized) {
            glDeleteVertexArrays(1, &VAO);
            VAOInitialized = false;
        }
        if (EBOInitialized) {
            glDeleteBuffers(1, &EBO);
            EBOInitialized = false;
        }

        emptyMesh = true;
        initialized = true;
        return;
    }

    this->drawLine = drawLine;
    
    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    } else {
        VAOInitialized = true;
    }

    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    } else {
        VBOInitialized = true;
    }

    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
        EBOInitialized = false;
    }

    numberOfVerticies = verticies.size() / 8;

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    emptyMesh = false;
    initialized = true;
}

void Mesh::set3d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine) {
    if (released) {
        PRINTLN("cant set released mesh");
        return;
    }

    if (verticies.size() == 0) {
        if (VAOInitialized) {
            glDeleteVertexArrays(1, &VAO);
            VAOInitialized = false;
        }
        if (EBOInitialized) {
            glDeleteBuffers(1, &EBO);
            EBOInitialized = false;
        }

        emptyMesh = true;
        initialized = true;
        return;
    }

    this->drawLine = drawLine;
    
    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    } else {
        VAOInitialized = true;
    }

    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    } else {
        VBOInitialized = true;
    }

    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
    } else {
        EBOInitialized = true;
    }

    numberOfIndicies = indicies.size();

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), &indicies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    emptyMesh = false;
    initialized = true;
}

void Mesh::set3dLayers(const std::vector<float> &verticies) {
    if (released) {
        PRINTLN("cant set released mesh");
        return;
    }

    if (verticies.size() == 0) {
        if (VAOInitialized) {
            glDeleteVertexArrays(1, &VAO);
            VAOInitialized = false;
        }
        if (EBOInitialized) {
            glDeleteBuffers(1, &EBO);
            EBOInitialized = false;
        }

        emptyMesh = true;
        initialized = true;
        return;
    }

    this->drawLine = false;
    
    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    } else {
        VAOInitialized = true;
    }

    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    } else {
        VBOInitialized = true;
    }

    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
        EBOInitialized = false;
    }

    numberOfVerticies = verticies.size() / 9;

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    emptyMesh = false;
    initialized = true;
}

void Mesh::set2d(const std::vector<float> &verticies, bool drawLine) {
    if (released) {
        PRINTLN("cant set released mesh");
        return;
    }

    if (verticies.size() == 0) {
        if (VAOInitialized) {
            glDeleteVertexArrays(1, &VAO);
            VAOInitialized = false;
        }
        if (EBOInitialized) {
            glDeleteBuffers(1, &EBO);
            EBOInitialized = false;
        }

        emptyMesh = true;
        initialized = true;
        return;
    }

    this->drawLine = drawLine;
    
    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    } else {
        VAOInitialized = true;
    }

    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    } else {
        VBOInitialized = true;
    }

    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
        EBOInitialized = false;
    }

    numberOfVerticies = verticies.size() / 2;

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    emptyMesh = false;
    initialized = true;
}

void Mesh::set2d(const std::vector<float> &verticies, const std::vector<unsigned int> &indicies, bool drawLine) {
    if (released) {
        PRINTLN("cant set released mesh");
        return;
    }

    if (verticies.size() == 0) {
        if (VAOInitialized) {
            glDeleteVertexArrays(1, &VAO);
            VAOInitialized = false;
        }
        if (EBOInitialized) {
            glDeleteBuffers(1, &EBO);
            EBOInitialized = false;
        }

        emptyMesh = true;
        initialized = true;
        return;
    }

    this->drawLine = drawLine;
    
    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    } else {
        VAOInitialized = true;
    }

    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    } else {
        VBOInitialized = true;
    }

    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
    } else {
        EBOInitialized = true;
    }

    numberOfIndicies = indicies.size();

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), &indicies[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    emptyMesh = false;
    initialized = true;
}

void Mesh::draw() const {
    if (!initialized) {
        PRINTLN("cant draw uninitialized mesh");
        return;
    }
    if (released) {
        PRINTLN("cant draw released mesh");
        return;
    }

    if (emptyMesh) {
        return;
    }

    glBindVertexArray(VAO);
    if (!EBOInitialized) {
        glDrawArrays(drawLine ? GL_LINES : GL_TRIANGLES, 0, numberOfVerticies);
    } else {
        glDrawElements(drawLine ? GL_LINES : GL_TRIANGLES, numberOfIndicies, GL_UNSIGNED_INT, 0);
    }
}

void Mesh::release() {
    if (released) {
        return;
    }

    if (!initialized) {
        PRINTLN("cant release uninitialized mesh");
        return;
    }

    if (VAOInitialized) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBOInitialized) {
        glDeleteBuffers(1, &VBO);
    }
    if (EBOInitialized) {
        glDeleteBuffers(1, &EBO);
    }

    released = true;
}
