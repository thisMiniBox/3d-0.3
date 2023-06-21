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

#include"stb_image.h"
void calculateAttenuationFactors(float radius, float intensity,float& constant, float& linear, float& quadratic);
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

	int CreateShader(const char* vertexPath, const char* fragmentPath, int id);
	OpenGLShader* GetShader(int id) const;
	static LRESULT CALLBACK WndProcOpenGL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ResetOpenGLViewport();
	void DeleteModelBuffer(Model* model);
private:
	void SetMaterialData(OpenGLShader* shader, Material* material, const Camera& camera);
	HGLRC m_hglrc;
	std::map<int, OpenGLShader*>m_Shaders;
	std::map<Picture*, glTexture*>m_Textures;
	std::map<Mesh*, glMesh*>m_Meshs;

	//ÔÝÌæµÄÌì¿ÕºÐ
	UINT skyboxVAO, skyboxVBO, skyCubemapTexture;
};