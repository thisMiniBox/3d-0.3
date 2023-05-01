#include "MainWind.h"
using namespace DirectX;
MainWind::~MainWind()
{
    // 销毁窗口
    DestroyWindow(m_hWnd);
    // 注销窗口类
    UnregisterClass(WndClassName.c_str(), m_hInstance);
}
HWND MainWind::GethWnd()
{
	return m_hWnd;
}
void MainWind::SetRect(RECT NewRect)
{
    m_rect = NewRect;
    m_width = m_rect.right - m_rect.left;
    m_height = m_rect.bottom - m_rect.top;
}
RECT MainWind::GetRect()
{
    return m_rect;
}
std::wstring MainWind::getClassName()
{
    return WndClassName;
}
GDIWND::GDIWND()
{
    m_hWnd = nullptr;
    m_width = 400;
    m_height = 400;
    m_hdc = nullptr;
    m_rect = {};
    WndClassName = L"GDIMainWnd";
    // 定义窗口类描述结构
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcGDI;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WndClassName.c_str();
    // 注册窗口类
    RegisterClassEx(&wcex);
}
GDIWND::~GDIWND()
{
    
}
HWND GDIWND::CreateWind(HWND Parent, int x, int y, int w, int h)
{
    DestroyWindow(m_hWnd);
    m_hWnd = CreateWindowW( //创建编辑框
        WndClassName.c_str(),
        0,
        WS_CHILD | WS_BORDER | WS_VISIBLE,
        x, y, w, h,
        Parent,
        (HMENU)3,
        m_hInstance,
        nullptr);
    if (m_hWnd)
    {
        GetClientRect(m_hWnd, &m_rect);
        m_width = m_rect.right - m_rect.left;
        m_height = m_rect.bottom - m_rect.top;
    }
    return m_hWnd;
}
int MainWind::GetWidth()
{
    return m_width;
}
int MainWind::GetHeight()
{
    return m_height;
}
bool compareFace(const Outface& p1, const Outface& p2) {
    return p1.distance > p2.distance;
}
void GDIWND::Draw(const std::vector<Model*>& model, const Camera& camera)
{
    // 获取设备上下文句柄
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    // 创建兼容DC和位图
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, m_width, m_height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // 准备绘制三角形
    std::vector<Outface> faces;
    for (const auto& m : model)
    {
        // 计算模型投影后的所有三角形面
        std::vector<FACE> triFaces = m->GetTriFace();
        std::vector<Outface> outfaces = ProjectTriangles(triFaces, camera);

        // 将三角形面添加到待绘制列表中
        faces.insert(faces.end(), outfaces.begin(), outfaces.end());
    }
    std::sort(faces.begin(), faces.end(), compareFace);
    // 逐个绘制三角形面
    for (const auto& face : faces)
    {
        // 创建画刷并设置颜色
        HBRUSH brush = CreateSolidBrush(face.color);
        SelectObject(memDC, brush);

        Polygon(memDC, face.point, 3);

        // 释放画刷
        DeleteObject(brush);
    }

    // 将绘制的位图拷贝到设备上下文中
    BitBlt(hdc, 0, 0, m_width, m_height, memDC, 0, 0, SRCCOPY);

    // 释放位图和设备上下文资源
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    // 结束绘制
    EndPaint(m_hWnd, &ps);
}


std::vector<Outface> GDIWND::ProjectTriangles(const std::vector<FACE>& faces, const Camera& camera)
{
    // 计算视图矩阵和投影矩阵
    Matrix4 viewMat = camera.GetView();
    Matrix4 projMat = camera.GetProjection();

    // 对于每个面，将其三个顶点进行投影，并将结果存储到输出列表中
    std::vector<Outface> outfaces;
    Vector dir;
    Outface outface;
    Vector CRight = camera.GetRight();
    Vector CUp = camera.GetUp();
    Vector CPos = camera.GetPosition();
    float di = 0;
    float fov = (m_width+m_height) * (2-2*vec::DegToRad(camera.GetFieldOfView()));
    for (const auto& face : faces)
    {
        dir = face.vertexA - CPos;
        if (dir * face.normalA > 0 || dir * camera.GetDirection() < 0)continue;
        outface.distance = dir.Length();
        di = outface.distance;
        if (di > camera.GetFar())continue;
        outface.point[0].x = (dir * CRight) / CRight.Length() * fov / di + (float)m_width / 2;
        outface.point[0].y = -(dir * CUp) / CUp.Length() * fov / di + (float)m_height / 2;

        dir = face.vertexB - CPos;
        di = dir.Length();
        outface.point[1].x = (dir * CRight) / CRight.Length() * fov / di + (float)m_width / 2;
        outface.point[1].y = -(dir * CUp) / CUp.Length() * fov / di + (float)m_height / 2;

        dir = face.vertexC - CPos;
        di = dir.Length();
        outface.point[2].x = (dir * CRight) / CRight.Length() * fov / di + (float)m_width / 2;
        outface.point[2].y = -(dir * CUp) / CUp.Length() * fov / di + (float)m_height / 2;
        outface.color = face.color;

        // 将投影后的面添加到输出列表中
        outfaces.push_back(outface);
    }

    return outfaces;
}
D3DWND11::D3DWND11()
{
    m_device = nullptr;
    m_immediateContext = nullptr;
    m_swapChain = nullptr;
    m_renderTargetView = nullptr;
    m_featureLevel = D3D_FEATURE_LEVEL_11_0;
}

HWND D3DWND11::CreateWind(HWND Parent, int x, int y, int w, int h)
{
    // 创建窗口

    WndClassName = L"D3D11WindowClass";

    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcD3D11;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WndClassName.c_str();

    RegisterClassEx(&wcex);

    m_rect = { x, y, x + w, y + h };
    AdjustWindowRect(&m_rect, WS_CHILD, FALSE);
    m_width = m_rect.right - m_rect.left;
    m_height = m_rect.bottom - m_rect.top;

    m_hWnd = CreateWindowEx(0, WndClassName.c_str(), L"D3D11 Window", WS_CHILD | WS_VISIBLE,
        x, y, m_width, m_height, Parent, NULL, m_hInstance, NULL);

    if (!m_hWnd)
    {
        MessageBox(NULL, L"Failed to create window.", L"Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    // 初始化Direct3D 11设备和交换链

    InitDeviceAndSwapChain(m_hWnd);

    return m_hWnd;
}

void D3DWND11::Draw(const std::vector<Model*>& models, const Camera& camera)
{
    // 清空背景
    float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    m_immediateContext->ClearRenderTargetView(m_renderTargetView, clearColor);

    // 设置渲染状态

    // TODO: 设置顶点和像素着色器

    // TODO: 设置缓冲区、纹理等

    // 设置视口
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)m_width;
    vp.Height = (FLOAT)m_height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    m_immediateContext->RSSetViewports(1, &vp);

    // 执行绘制调用

    // TODO: 调用ID3D11DeviceContext::DrawIndexed()或Draw()等函数来执行绘制调用

    // 结束渲染
    m_swapChain->Present(0, 0);
}
void D3DWND11::InitDeviceAndSwapChain(HWND hWnd)
{
    // 创建设备和交换链

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = m_width;
    scd.BufferDesc.Height = m_height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_swapChain,
        &m_device, &m_featureLevel, &m_immediateContext);

    if (FAILED(hr))
    {
        MessageBox(NULL, L"Failed to create device and swap chain.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // 创建渲染目标视图

    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

    if (FAILED(hr))
    {
        MessageBox(NULL, L"Failed to get back buffer.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    hr = m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_renderTargetView);
    pBackBuffer->Release();

    if (FAILED(hr))
    {
        MessageBox(NULL, L"Failed to create render target view.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

    // 其他初始化工作

    // TODO: 初始化其他资源，例如常量缓冲区、纹理等
}

void D3DWND11::ReleaseResources()
{
    if (m_immediateContext)
    {
        m_immediateContext->ClearState();
        m_immediateContext->Flush();
    }

    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    if (m_immediateContext)
    {
        m_immediateContext->Release();
        m_immediateContext = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }
}
D3DWND11::~D3DWND11()
{
    ReleaseResources();
}
OpenGLWnd::OpenGLWnd()
    : m_hglrc(nullptr), m_ModelShader(nullptr), m_LightShader(nullptr)
{
    WndClassName = L"OpenGL_WindowClass";
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcOpenGL;
    wcex.hInstance = m_hInstance;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = WndClassName.c_str();

    RegisterClassEx(&wcex);
    
}
OpenGLWnd::~OpenGLWnd()
{
    ReleaseDC(m_hWnd, m_hdc);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(m_hglrc);
    if (m_ModelShader)delete m_ModelShader;
    if (m_LightShader)delete m_LightShader;
    for (const auto p : m_models)
        if (p.second)
            delete p.second;
    m_models.clear();
}
HWND OpenGLWnd::CreateWind(HWND Parent, int x, int y, int w, int h) 
{

    // 创建窗口
    m_hWnd = CreateWindowEx(0,
        WndClassName.c_str(), NULL,
        WS_CHILD | WS_VISIBLE,
        x, y, w, h,
        Parent,
        NULL, m_hInstance, NULL);

    if (!m_hWnd) {
        return NULL;
    }

    m_width = w;
    m_height = h;

    // 获取设备上下文句柄
    HDC hdc = GetDC(m_hWnd);

    // 设置像素格式
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    int format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, format, &pfd);//

    // 创建OpenGL上下文并使其成为当前上下文
    m_hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, m_hglrc);//

    // 加载所有的OpenGL函数指针
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    // 设置视口大小
    glViewport(0, 0, w, h); // w和h分别是窗口的宽度和高度
    m_ModelShader = new OpenGLShader("Shader/ModelShader.vs", "Shader/ModelShader.fs");
    m_LightShader = new OpenGLShader("Shader/LightShader.vs", "Shader/LightShader.fs");
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return m_hWnd;

}
bool OpenGLWnd::AddModelToBuffer(Model* model)
{
    m_models[model] = new OldModelBuffer(model, m_ModelShader);
    if (m_models[model])
        return true;
    return false;
}
void OpenGLWnd::DeleteModelBuffer(Model* model)
{
    auto it = m_models.find(model);
    if (it != m_models.end())
    {
        delete it->second;
        m_models.erase(it);
    }

    if (!model->GetChildModel().empty())
        for (auto& cMod : model->GetChildModel())
            DeleteModelBuffer(cMod);
}
void GetChildModel(const std::vector<Model*>& Models, std::vector<Model*>& out)
{
    for (const auto& i : Models)
    {
        if (i->GetMesh())
            out.push_back(i);
        GetChildModel(i->GetChildModel(), out);
    }
}
void OpenGLWnd::Draw(const std::vector<Model*>& Models, const Camera& camera)
{
    std::vector<Model*> models;
    GetChildModel(Models, models);
    PAINTSTRUCT ps;
    m_hdc = BeginPaint(m_hWnd, &ps);
    wglMakeCurrent(m_hdc, m_hglrc);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_ModelShader->use();

    m_ModelShader->setVec3("viewPos", camera.GetPosition());

    m_ModelShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    m_ModelShader->setVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
    m_ModelShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    m_ModelShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    m_ModelShader->setVec3("pointLights[0].position", camera.GetPosition());
    m_ModelShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    m_ModelShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    m_ModelShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    m_ModelShader->setFloat("pointLights[0].constant", 1.0f);
    m_ModelShader->setFloat("pointLights[0].linear", 0.09f);
    m_ModelShader->setFloat("pointLights[0].quadratic", 0.032f);

    // material properties
    m_ModelShader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    m_ModelShader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
    m_ModelShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    m_ModelShader->setFloat("material.shininess", 32.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFieldOfView()),camera.GetAspectRatio(), camera.GetNear(), camera.GetFar());
    glm::mat4 view = camera.GetGLMView();
    m_ModelShader->setMat4("projection", projection);
    m_ModelShader->setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    m_ModelShader->setMat4("model", model);

    glm::mat4 modelP = glm::mat4(1.0f);
    for (const auto& model : models)
    {
        if (!model->GetMesh())continue;
        if (m_models[model] == nullptr)
            m_models[model] = new OldModelBuffer(model, m_ModelShader);
        modelP = glm::mat4(1.0f);
        
        modelP = glm::translate(modelP, (glm::vec3)model->GetWorldPosition());
        modelP = glm::scale(modelP, (glm::vec3)model->getScale());
        modelP = glm::rotate(modelP, (float)(model->GetRotate().angle), (glm::vec3)(model->GetRotate().axis.Normalize()));
        m_models[model]->SetModelMatrix(modelP);
        m_models[model]->Draw();
    }
    SwapBuffers(GetDC(m_hWnd));
    EndPaint(m_hWnd, &ps);
}


void OpenGLWnd::SetModelShader(const char* vertexPath, const char* fragmentPath)
{
    if (m_ModelShader)
    {
        delete m_ModelShader;
        m_ModelShader = nullptr;
    }
    m_ModelShader = new OpenGLShader(vertexPath, fragmentPath);
}
void OpenGLWnd::SetLightShader(const char* vertexPath, const char* fragmentPath)
{
    if (m_LightShader)
    {
        delete m_LightShader;
        m_LightShader = nullptr;
    }
    m_LightShader = new OpenGLShader(vertexPath, fragmentPath);
}
void OpenGLWnd::ResetOpenGLViewport()
{
    glViewport(0, 0, m_width, m_height);
}
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}