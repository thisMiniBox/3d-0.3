#pragma once
#include<string>
#include<Windows.h>
#include <algorithm>
#include"Object.h"
#include <d3d11.h>
#include<d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
enum MainWindType
{
	MGDIWND,
	MD3DWND11,
	MOPENGL
};
class MainWind
{
public:
	MainWind():m_hWnd(nullptr),m_rect({0,0,600,400}), m_width(600), m_height(400), m_hdc(nullptr), m_hInstance(GetModuleHandle(NULL)) {}
	virtual ~MainWind();
	
	RECT GetRect();
	HWND GethWnd();
	int GetWidth();
	int GetHeight();
	std::wstring getClassName();
	virtual void SetRect(RECT NewRect);
	virtual HWND CreateWind(HWND Parent, int x = 0, int y = 0, int w = 600, int h = 400) = 0;
	virtual void Draw(const std::vector<Model*>& model,const Camera& camera) = 0;
	virtual int GetType() = 0;
protected:
	HWND m_hWnd;
	RECT m_rect;
	int m_width;
	int m_height;
	std::wstring WndClassName;
	HDC m_hdc;
	HINSTANCE m_hInstance;
};
using namespace DirectX;
class D3DWND11 : public MainWind
{

public:
	D3DWND11();
	~D3DWND11();
	static LRESULT CALLBACK WndProcD3D11(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual HWND CreateWind(HWND Parent, int x = 0, int y = 0, int w = 600, int h = 400) override;
	virtual void Draw(const std::vector<Model*>& models, const Camera& camera) override;
	virtual int GetType() override { return MD3DWND11; }

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_immediateContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	D3D_FEATURE_LEVEL m_featureLevel;
	void InitDeviceAndSwapChain(HWND hWnd);
	void ReleaseResources();
};

class GDIWND :public MainWind
{
public:
	GDIWND();
	~GDIWND();
	virtual HWND CreateWind(HWND Parent, int x = 0, int y = 0, int w = 600, int h = 400)override;
	virtual void Draw(const std::vector<Model*>& model, const Camera& camera)override;
	int GetType() { return MGDIWND; }
	static LRESULT CALLBACK WndProcGDI(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::vector<Outface> ProjectTriangles(const std::vector<FACE>&, const Camera&);
};
#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#pragma comment (lib,"opengl32.lib")
unsigned int loadTexture(char const* path);
class ModelManager {
public:
	ModelManager(Model* model, OpenGLShader* shader = nullptr)
		: m_Model(model), m_Shader(shader),m_ModelMatrix(glm::mat4(1.0))
	{
		// 创建VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// 创建VBO并存储顶点数据
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Model->GetVertices().size(),
			m_Model->GetVertices().data(), GL_STATIC_DRAW);

		// 指定顶点属性指针
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_DOUBLE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_DOUBLE, GL_DOUBLE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_DOUBLE, GL_DOUBLE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);

		// 解绑VAO和VBO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_DiffuseMap = loadTexture((m_Model->GetFileAddress().c_str() + m_Model->GetMaterial().map_Kd).c_str());
		m_MirrorMap= loadTexture((m_Model->GetFileAddress().c_str() + m_Model->GetMaterial().map_Ka).c_str());
		m_Shader->use();
		m_Shader->setInt("material.diffuse", 0);
		m_Shader->setInt("material.specular", 1);
	}

	~ModelManager()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteTextures(1, &m_DiffuseMap);
		glDeleteTextures(1, &m_MirrorMap);
	}

	void Draw()
	{
		if (m_Shader != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_MirrorMap);
			m_Shader->use();
			m_Shader->setMat4("model", m_ModelMatrix);
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, m_Model->GetVertices().size());
			glBindVertexArray(0);
		}
	}

	void SetModelMatrix(glm::mat4 model)
	{
		m_ModelMatrix = model;
	}

	void SetShader(OpenGLShader* shader)
	{
		m_Shader = shader;
	}

private:
	GLuint m_VAO, m_VBO;
	Model* m_Model;
	OpenGLShader* m_Shader;
	glm::mat4 m_ModelMatrix;
	unsigned int m_DiffuseMap;
	unsigned int m_MirrorMap;
};
#include"stb_image.h"
class OpenGLWnd : public MainWind
{
public:
	OpenGLWnd();
	~OpenGLWnd();
	virtual HWND CreateWind(HWND Parent, int x = 0, int y = 0, int w = 600, int h = 400) override;
	virtual void Draw(const std::vector<Model*>& models, const Camera& camera) override;
	virtual int GetType() override {
		return MOPENGL;
	}
	void SetModelShader(const char* vertexPath, const char* fragmentPath);
	void SetLightShader(const char* vertexPath, const char* fragmentPath);
	static LRESULT CALLBACK WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ResetOpenGLViewport();
private:
	HGLRC m_hglrc;
	std::unordered_map<Model*, ModelManager*>m_models;
	OpenGLShader* m_ModelShader;
	OpenGLShader* m_LightShader;

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int texture1;
	unsigned int texture2;
};