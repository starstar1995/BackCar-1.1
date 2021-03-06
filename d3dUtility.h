#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include<Windows.h>
//XNA数学库相关头文件
#include<d3dcompiler.h>
#include<xnamath.h>

//DirectX11相关头文件
#include<D3D11.h>
#include<D3DX11.h>
#include<d3dx11effect.h>

//Direct11相关库
#pragma comment(lib,"Effects11")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

namespace d3d//定义一个d3d命名空间
{
	//初始化d3d
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		ID3D11RenderTargetView ** renderTargetView,//目标渲染视图窗口
		ID3D11DeviceContext ** immediateContext,//执行上下文接口
		IDXGISwapChain ** swapChain,//交换链接口
		ID3D11Device ** device);//设备用接口，每个D3D程序至少有一个设备
	//消息循环
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//回调函数
	LRESULT CALLBACK WndProc(
		HWND,
		UINT msg,
		WPARAM,
		LPARAM IParam);
}
#endif