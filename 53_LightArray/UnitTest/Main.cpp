#include "stdafx.h"
#include "Main.h"
#include "Systems/Window.h"

#include "VertexDemo.h"
#include "RectDemo.h"
#include "WorldDemo.h"
#include "IndexDemo.h"
#include "GridDemo.h"
#include "TerrainDemo.h"
#include "MeshDemo.h"
#include "TextureDemo.h"
#include "GetHeightDemo.h"
#include "VerticalPickDemo.h"
#include "ProjectionDemo.h"
#include "RaycastDemo.h"
#include "FrameworkDemo.h"
#include "SkyCubeDemo.h"
#include "TerrainBrushDemo.h"
#include "TerrainLineDemo.h"
#include "TerrainSplattingDemo.h"
#include "Objects/ColliderDemo.h"
//#include "CpuBillboardDemo.h"
#include "WeatherDemo.h"
#include "GeometryBillboardDemo.h"
#include "InstancingDemo.h"
#include "InstanceFrameworkDemo.h"
#include "ThreadDemo.h"
#include "ByteAddressDemo.h"
#include "InstanceColliderDemo.h"
#include "SpecularDemo.h"
#include "NormalMapDemo.h"
#include "LightingDemo.h"
#include "RtvDemo.h"
#include "MrtDemo.h"
#include "ViewportDemo.h"
#include "ProjectorDemo.h"
#include "Lighting/ShadowDemo.h"
#include "Objects/DynamicCubeMapDemo.h"
#include "Deffered/DefferedDemo.h"
#include "Tessellation/TriangleDemo.h"
#include "Tessellation/QuadDemo.h"
#include "Tessellation/TessDistanceDemo.h"
#include "Objects/FrustumDemo.h"
#include "Deffered/DefferedPointLightsDemo.h"
#include "Deffered/DefferedPointLightArrayDemo.h"

void Main::Initialize()
{
	Push(new DefferedPointLightArrayDemo());
}

void Main::Ready()
{

}

void Main::Destroy()
{
	for (IExecute* exe : executes)
	{
		exe->Destroy();
		SafeDelete(exe);
	}
}

void Main::Update()
{
	for (IExecute* exe : executes)
		exe->Update();
}

void Main::PreRender()
{
	for (IExecute* exe : executes)
		exe->PreRender();
}

void Main::Render()
{
	for (IExecute* exe : executes)
		exe->Render();
}

void Main::PostRender()
{
	for (IExecute* exe : executes)
		exe->PostRender();
}

void Main::ResizeScreen()
{
	for (IExecute* exe : executes)
		exe->ResizeScreen();
}

void Main::Push(IExecute * execute)
{
	executes.push_back(execute);

	execute->Initialize();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.AppName = L"D3D Game";
	desc.Instance = instance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	desc.Background = Color(0.3f, 0.3f, 0.3f, 1.0f);	
	D3D::SetDesc(desc);

	Main* main = new Main();
	WPARAM wParam = Window::Run(main);

	SafeDelete(main);

	return wParam;
}