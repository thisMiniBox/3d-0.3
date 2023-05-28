#pragma once
#include<vector>
#include"Object.h"
#include<glad/glad.h>
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int loadTexture(char const* path);



class Mesh_OpenGL
{
	size_t m_BufferSize;
	GLuint m_VAO, m_VBO;
public:
	Mesh_OpenGL();
	Mesh_OpenGL(const std::vector<Vertex>&);
	~Mesh_OpenGL();
	void SetVertex(const std::vector<Vertex>&);
	void Draw();
};

class Picture_OpenGL
{
	GLuint m_PictureID;
public:
	Picture_OpenGL();
	Picture_OpenGL(const Picture&);
	~Picture_OpenGL();
	GLuint GetID()const;
};