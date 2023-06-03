#pragma once
#include<string>
#include<Windows.h>
#include <algorithm>
#include"Object.h"
#include <d3d11.h>
#include<d3dcompiler.h>
#include"WndData.h"
#include"OpenGLData.h"
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

void GetChildModel(const std::vector<Model*>& Models, std::vector<Model*>& out);
ULONG64 GetTime();
RUNMODE GetRunMode();
enum MainWindType
{
	MGDIWND,
	MD3DWND11,
	MOPENGL
};
class MainWind
{
public:
	MainWind();
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
	std::vector<Outface> ProjectTriangles(const std::vector<FACE>&, const Camera&, COLORREF color = 0);
};
#include<glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#pragma comment (lib,"opengl32.lib")

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
	GLuint m_VAO, m_VBO;
	Model* m_Model;
	OpenGLShader* m_Shaders;
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
	HWND CreateWind(HWND Parent, int x = 0, int y = 0, int w = 600, int h = 400) override;
	void Draw(const std::vector<Model*>& models, const Camera& camera) override;
	int GetType() override {
		return MOPENGL;
	}
	void SetRect(RECT rect)override;
	bool AddModelToBuffer(Model*);
	void DeleteModelBuffer(Model*);

	int CreateShader(const char* vertexPath, const char* fragmentPath, int id);
	OpenGLShader* GetShader(int id) const;
	static LRESULT CALLBACK WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ResetOpenGLViewport();
private:
	void DrawModel(Model* model, const glm::mat4& view);
	HGLRC m_hglrc;
	std::unordered_map<Model*, OldModelBuffer*>m_models;
	std::unordered_map<Mesh*, Mesh_OpenGL*>m_Meshs;
	std::unordered_map<Picture*, Picture_OpenGL*>m_Textures;
	std::unordered_map<int, OpenGLShader*>m_Shaders;
	UINT skyboxVAO, skyboxVBO, skyCubemapTexture;
};