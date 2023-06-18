#include "MainWind.h"
using namespace DirectX;
MainWind::MainWind() :m_hWnd(nullptr), m_rect({ 0,0,600,400 }), m_width(600), m_height(400), m_hdc(nullptr), m_hInstance(GetModuleHandle(NULL)) {}
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
        (HMENU)ChildWindSign::GdiWind,
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
void GDIWND::Draw(const std::vector<Model*>& Models, const Camera& camera)
{
    std::vector<Model*> model;
    GetChildModel(Models, model);
    // 获取设备上下文句柄
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    // 创建兼容DC和位图
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, m_width, m_height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    COLORREF color = RGB(200, 200, 100);
    // 准备绘制三角形
    std::vector<Outface> faces;
    for (const auto& m : model)
    {
        std::vector<FACE> triFaces = m->GetTriFace();
        std::vector<Outface> outfaces;
        if (m->IsSelected())
            outfaces = ProjectTriangles(triFaces, camera, color);
        else
            outfaces = ProjectTriangles(triFaces, camera);
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


std::vector<Outface> GDIWND::ProjectTriangles(const std::vector<FACE>& faces, const Camera& camera,COLORREF color)
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
        if (color == 0)
            outface.color = face.color;
        else
            outface.color = color;

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
        x, y, m_width, m_height, Parent, (HMENU)ChildWindSign::D3D11Wind, m_hInstance, NULL);

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
    : m_hglrc(nullptr)
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
    for (const auto p : m_models)
    {
        if (p.second)
            delete p.second;
    }
    for (const auto p : m_Meshs)
    {
        if (p.second)
            delete p.second;
    }
    for (const auto p : m_Shaders)
    {
        if (p.second)
            delete p.second;
    }
    //for (const auto p : m_Textures)
    //{
    //    if (p.second)
    //        delete p.second;
    //}
}
HWND OpenGLWnd::CreateWind(HWND Parent, int x, int y, int w, int h) 
{

    // 创建窗口
    m_hWnd = CreateWindowEx(0,
        WndClassName.c_str(), NULL,
        WS_CHILD | WS_VISIBLE,
        x, y, w, h,
        Parent,
        (HMENU)ChildWindSign::OpenGLWind, m_hInstance, NULL);

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // 设置视口大小
    glViewport(0, 0, w, h); // w和h分别是窗口的宽度和高度
    CreateShader("Shader/ModelShader.vs", "Shader/ModelShader.fs", SI_ModelShader);
    CreateShader("Shader/LightShader.vs", "Shader/LightShader.fs", SI_LightShader);
    CreateShader("Shader/ModelShader.vs", "Shader/Stroke.fs", SI_StrokeShader);
    CreateShader("Shader/SkyBox.vs", "Shader/SkyBox.fs", SI_SkyBoxShader);
    std::vector<std::string> faces
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    skyCubemapTexture = loadCubemap(faces);
    if (skyCubemapTexture)
        std::cout << "天空球加载成功" << std::endl;
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    OpenGLShader* skyboxShader = GetShader(SI_SkyBoxShader);
    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);
    skyboxShader = GetShader(SI_StrokeShader);
    skyboxShader->use();
    skyboxShader->setVec3("color", glm::vec3(0.7, 0.7, 0.4));
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    return m_hWnd;

}
int OpenGLWnd::CreateShader(const char* vertexPath, const char* fragmentPath,int id)
{
    OpenGLShader* Shader = new OpenGLShader(vertexPath, fragmentPath);
    if (!Shader)
        return SI_Empty;
    if (id == SI_Empty || m_Shaders.count(id) != 0)
    {
        id += SI_User;
        while (true)
        {
            if (m_Shaders.count(id) == 0)
                break;
            id++;
        }
    }
    m_Shaders.insert(std::make_pair(id, Shader));
    return id;
}
OpenGLShader* OpenGLWnd::GetShader(int id) const 
{
    auto it = m_Shaders.find(id);
    if (it != m_Shaders.end()) 
        return it->second;
    else 
        return nullptr;
}

bool OpenGLWnd::AddModelToBuffer(Model* model)
{
    Mesh* mesh = model->GetMesh();
    if (mesh)
    {
        //if (!m_Meshs[mesh])
        //    m_Meshs[mesh] = new Mesh_OpenGL(mesh->GetVertexData());
    }
    else
        return false;
    Material* material = model->GetMaterial();
    if(material)
    {
        Picture* picture = material->getMapKa();
        //if (picture)
        //    if (!m_Textures[picture])
        //        m_Textures[picture] = new Picture_OpenGL(*picture);
        //picture = material->getMapKd();
        //if (picture)
        //    if (!m_Textures[picture])
        //        m_Textures[picture] = new Picture_OpenGL(*picture);
        //picture = material->getMapKs();
        //if (picture)
        //    if (!m_Textures[picture])
        //        m_Textures[picture] = new Picture_OpenGL(*picture);
    }
    return true;
}
void OpenGLWnd::SetRect(RECT NewRect)
{
    m_rect = NewRect;
    m_width = m_rect.right - m_rect.left;
    m_height = m_rect.bottom - m_rect.top;
    if (m_hWnd)
        ResetOpenGLViewport();
}
void OpenGLWnd::DeleteModelBuffer(Model* model)
{

    auto it = m_models.find(model);
    if (it != m_models.end())
        m_models.erase(it);
    Material* material = model->GetMaterial();
    //auto om = m_Meshs.find(model->GetMesh());
    //if (om != m_Meshs.end())
    //{
    //    delete (*om).second;
    //    m_Meshs.erase(om);
    //}
    //auto op = m_Textures.find(material->getMapKa());
    //if (op != m_Textures.end())
    //{
    //    delete (*op).second;
    //    m_Textures.erase(op);
    //}
    //op = m_Textures.find(material->getMapKd());
    //if (op != m_Textures.end())
    //{
    //    delete (*op).second;
    //    m_Textures.erase(op);
    //}
    //op = m_Textures.find(material->getMapKs());
    //if (op != m_Textures.end())
    //{
    //    delete (*op).second;
    //    m_Textures.erase(op);
    //}
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
void OpenGLWnd::DrawModel(Model* model, const glm::mat4& view)
{

}
void OpenGLWnd::Draw(const std::vector<Model*>& Models, const Camera& camera)
{
    std::vector<Model*> models;
    GetChildModel(Models, models);
    for (auto model : models)
        AddModelToBuffer(model);
    PAINTSTRUCT ps;
    m_hdc = BeginPaint(m_hWnd, &ps);
    wglMakeCurrent(m_hdc, m_hglrc);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    OpenGLShader* CurrentShader = nullptr;
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFieldOfView()),camera.GetAspectRatio(), camera.GetNear(), camera.GetFar());
    glm::mat4 view = camera.GetGLMView();

    glDepthMask(GL_FALSE);
    CurrentShader = GetShader(SI_SkyBoxShader);
    CurrentShader->use();
    CurrentShader->setMat4("projection", projection);
    CurrentShader->setMat4("view", glm::mat4(glm::mat3(view)));
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);  

    CurrentShader = GetShader(SI_ModelShader);
    CurrentShader->use();

    CurrentShader->setVec3("viewPos", camera.GetPosition());

    CurrentShader->setMat4("projection", projection);
    CurrentShader->setMat4("view", view);
    glm::mat4 modelTransform = glm::mat4(1.0f);
    for (const auto& model : models)
    {
        modelTransform = model->GetGLTransform();
        CurrentShader->setMat4("model",modelTransform);
        Mesh* mesh = model->GetMesh();
        if (!mesh)
            continue;
        if (m_Meshs.find(mesh) == m_Meshs.end())
            m_Meshs[mesh] = new glMesh(mesh->GetVertexData(), mesh->GetIndices());
        Material* material = model->GetMaterial();
        if (material)
        {
            Picture* picture = material->getMapKa();
            if (picture)
            {
                if (m_Textures.find(picture) == m_Textures.end())
                    m_Textures[picture] = {picture->loadTexture(),"material.ambientTexture"};
            }
            picture = material->getMapKd();
            if (picture)
            {
                if (m_Textures.find(picture) == m_Textures.end())
                    m_Textures[picture] = { picture->loadTexture(),"material.diffuseTexture" };
            }
            picture = material->getMapKs();
            if (picture)
            {
                if (m_Textures.find(picture) == m_Textures.end())
                    m_Textures[picture] = { picture->loadTexture(),"material.specularTexture" };
            }
        }
        SetMaterialData(CurrentShader, material, camera);
        m_Meshs[mesh]->Draw(*CurrentShader);

        if (m_models[model] == nullptr)
            m_models[model] = new OldModelBuffer(model, CurrentShader);
        //if (GetRunMode_g() == RUNMODE::RM_EDIT)
        //    modelTransform = model->GetGLTransform();
        //else
        //modelTransform = model->GetTransform(GetTime());
        //m_models[model]->SetShader(CurrentShader);
        //m_models[model]->SetModelMatrix(modelTransform);
        //m_models[model]->Draw();
    }
    CurrentShader = GetShader(SI_StrokeShader);
    CurrentShader->use();
    CurrentShader->setMat4("projection", projection);
    CurrentShader->setMat4("view", view);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    for (const auto& model : models)
    {
        if (!model->GetMesh() || !model->IsSelected())
            continue;
        modelTransform = model->GetTransform(GetTime());
        m_models[model]->SetShader(CurrentShader);
        m_models[model]->SetModelMatrix(modelTransform);
        m_models[model]->Draw();
    }
    glDisable(GL_CULL_FACE);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    SwapBuffers(GetDC(m_hWnd));
    EndPaint(m_hWnd, &ps);
}

void OpenGLWnd::ResetOpenGLViewport()
{
    glViewport(0, 0, m_width, m_height);
}
void OpenGLWnd::SetMaterialData(OpenGLShader* shader, Material* material,const Camera& camera)
{
    if (!material)
        return;
    shader->use();
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    auto pointLight = GetAllPointLight();
    PointLight* pointL = pointLight[0];
    glm::vec3 lightPos;
    if (pointLight.empty())
        lightPos = glm::vec3(0, 0, -3);
    else
        lightPos = pointL->GetPosition();
    shader->setVec3("light.position", lightPos);
    shader->setVec3("viewPos", camera.GetPosition());

    shader->setVec3("material.ambient", material->getKa());
    shader->setVec3("material.diffuse", material->getKd());
    //shader->setVec3("material.specular", material->getKs());
    shader->setVec3("material.specular", glm::vec3(0.1f));
    //shader->setFloat("material.shininess", material->getNs());
    shader->setFloat("material.shininess", 128);
    lightPos = pointL->GetLightColor().Normalize();
    shader->setVec3("light.ambient", lightPos * glm::vec3(0.2));
    shader->setVec3("light.diffuse", lightPos * glm::vec3(0.7));
    shader->setVec3("light.specular", lightPos);
    float cons, line, quad;
    calculateAttenuationFactors(pointL->GetIntensity(), pointL->GetRange(), cons, line, quad);
    shader->setFloat("light.constant", cons);
    shader->setFloat("light.linear", line);
    shader->setFloat("light.quadratic", quad);
    Picture* texture = material->getMapKd();
    int num = 0;
    if(texture)
    {
        shader->setInt("material.diffuse", num);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Textures[texture].id);
        num++;
    }
}
void calculateAttenuationFactors(float intensity, float radius, float& constant, float& linear, float& quadratic)
{
    constant = 1.0f;
    linear = (1.0f - 0.0f) / radius;
    quadratic = (1.0f - 0.0f) / (radius * radius);
}

