#include "pch.h"
#include "ViewPoint.h"

using namespace std;
using namespace DirectX;
using namespace Geothermal;
using namespace Graphics;
using namespace Bindables;

ViewPoint::ViewPoint(shared_ptr<DeviceResources> const& deviceResources, float viewWidth, float viewHeight, float nearZ, float farZ)
{
	InitializeConstantBuffers(deviceResources);
	perspective = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
}

ViewPoint::ViewPoint(shared_ptr<DeviceResources> const& deviceResources, float aspectRatio, float nearZ, float farZ)
{
	InitializeConstantBuffers(deviceResources);
	perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, nearZ, farZ);
}

void ViewPoint::Bind(XMMATRIX world2View)
{
	// Transform view space origin into world space
	XMVECTOR worldPosition = 
		XMVector4Transform(XMVECTORF32{ 0.0f, 0.0f, 0.0f, 1.0f }, XMMatrixInverse(nullptr, world2View));

	XMStoreFloat3(&(parameters.CameraWorldPosition), worldPosition);
	parameters.World2ClipTransform = XMMatrixTranspose(world2View * perspective);

	parametersBufferVS->Update(parameters);
	parametersBufferPS->Update(parameters);
	parametersBufferVS->Bind();
	parametersBufferPS->Bind();
}

void ViewPoint::InitializeConstantBuffers(shared_ptr<DeviceResources> const& deviceResources)
{
	parametersBufferVS = make_unique<VertexConstantBuffer<ViewParameters>>(deviceResources, 1u);
	parametersBufferPS = make_unique<PixelConstantBuffer<ViewParameters>>(deviceResources, 1u);
}
