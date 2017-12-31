#include"d3dUtility.h"

//初始化 d3d
bool d3d::InitD3D(
	HINSTANCE hInstance,
	int width,
	int height,
	ID3D11RenderTargetView ** renderTargetView,//目标渲染视图窗口
	ID3D11DeviceContext ** immediateContext,//执行上下文接口
	IDXGISwapChain ** swapChain,//交换链接口
	ID3D11Device ** device)//设备用接口，每个D3D程序至少有一个设备
{
	//创建第一个窗口
	//步骤：1.设计一个窗口类2.注册窗口类3.创建窗口4.窗口显示和更新
	//1.设计窗口
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Direct3D11App";

	//2.注册窗口
	if (!RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	//3.创建窗口
	HWND hwnd = ::CreateWindow(L"Direct3D11App",
		L"D3D11",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		0,
		0,
		hInstance,
		0);
	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}

	//4.窗口显示和更新
	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);
	//创建窗口结束  开始初始化 D3D
	//初始化D3D步骤 1.描述交换链，即填充DXGI_SQAP_CHAIN_DESC结构
	//2.使用D3D11CreateDcviceAndSwapChain创建D3D设备(ID3D11Device) 执行上下文接口(ID3DeviceContext),交换 链接口(IDXGISwapChain)
	//3.创建目标渲染窗口(ID3D11RenderTargetView)
	//4.设置视口(View Port)

	//第一步
	DXGI_SWAP_CHAIN_DESC sd;//先声明DXGI_SWAP_CHAIN_DESC的对象sd
	ZeroMemory(&sd, sizeof(sd));//用ZeroMemory对sd进行初始化
	sd.BufferCount = 1;//交换链中后台缓存数量，通常为1
	sd.BufferDesc.Width = width;//缓存区的窗口宽
	sd.BufferDesc.Height = height;//缓存区的窗口高
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//表示红绿蓝Alpha各8位
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//第二步
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	//获取 D3D_FEATURE_LEVEL数组的元素个数
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	//调用D3D11CreateDeviceAndSwapChain创建交换链、设备和执行上下文
	//分别存入swapChain,devoce,immedicateContext
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		swapChain,
		device,
		NULL,
		immediateContext)))
	{
		::MessageBox(0, L"CreateDevice - FAILED", 0, 0);
		return false;
	}

	//第三步
	HRESULT hr = 0;
	ID3D11Texture2D * pBackBuffer = NULL;
	//调用GetBuffer()函数得到后台缓存对象，并村纳入&pBackBuffer中
	hr = (*swapChain)->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	//判断getbuffer是否调用成功
	if (FAILED(hr))
	{
		::MessageBox(0, L"GetBuffer - FAILED", 0, 0);
		return false;
	}

	//调用createredertargetview创建好渲染目标视图
	//创建后存入rendertargetview中
	hr = (*device)->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		renderTargetView);

	pBackBuffer->Release();
	//判断CreateRenderTargetView是否调用成功 
	if (FAILED(hr))
	{
		::MessageBox(0, L"GetRender - FAILED", 0, 0);
		return false;
	}

	//将渲染目标视图绑定到渲染管线
	(*immediateContext)->OMSetRenderTargets(1,
		renderTargetView,
		NULL);

	//第四步
	D3D11_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0,
		vp.TopLeftY = 0;

	//设置视口
	(*immediateContext)->RSSetViewports(
		1,
		&vp);
	return true;
}


//初始化 d3d 结束
//消息循环函数
int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//第二次获取当前时间
			float currTime = (float)timeGetTime();
			//获取两次事件之间的时间差
			float timeDelta = (currTime - lastTime) * 0.001f;
			//调用显示函数，
			ptr_display(timeDelta);
			lastTime = currTime;
		}
	}
	return msg.wParam;
}