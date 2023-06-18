#pragma once
#include<vector>
#include"Object.h"
#include<glad/glad.h>
#include"Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadTexture(char const* path);
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class OldModelBuffer {
public:
    OldModelBuffer(Model* model, OpenGLShader* shader = nullptr);

    ~OldModelBuffer();

    void Draw();

    void SetModelMatrix(glm::mat4 model)
    {
        m_ModelMatrix = model;
    }

    void SetShader(OpenGLShader* shader)
    {
        m_Shaders = shader;
    }

private:
    GLuint m_VAO, m_VBO, m_EBO;
    Model* m_Model;
    OpenGLShader* m_Shaders;
    glm::mat4 m_ModelMatrix;
    unsigned int m_DiffuseMap;
    unsigned int m_MirrorMap;
};
struct glTexture {
	unsigned int id;
	std::string type;
};
struct glVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class glMesh {
public:
    glMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~glMesh();
    void Draw(OpenGLShader shader);
private:
    size_t m_IndicesSize;
    unsigned int m_VAO, m_VBO, m_EBO;
};
