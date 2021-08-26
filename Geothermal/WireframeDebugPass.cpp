#include "pch.h"
#include "WireframeDebugPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Materials;
using namespace Passes;
using namespace Structures;
using namespace std;
using namespace DirectX;

WireframeDebugPass::WireframeDebugPass
(
	shared_ptr<DeviceResources> const& deviceResources
) : 
	SceneGeometryPass(deviceResources, nullptr, nullptr),
	states(nullptr)
{
	states = make_unique<CommonStates>(deviceResources->Device());
	// Initialize wireframe material
	// Use the WireframeVertexShader + Unlit combo
	wireframeMaterial = 
		make_unique<Material>(deviceResources, L"WireframeVertexShader.cso", L"Unlit.cso", VertexPLayout, size(VertexPLayout));
}

void WireframeDebugPass::operator()()
{
	SetUpPipelineStates();
	// Draw directly on the screen
	ID3D11RenderTargetView* target = deviceResources->BackBufferTargetView();
	deviceResources->SetTargets(1, &target, nullptr);

	VisualizeBounds();
}

void WireframeDebugPass::SetUpPipelineStates()
{
	deviceResources->Context()->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	deviceResources->Context()->OMSetDepthStencilState(states->DepthNone(), 0);
	deviceResources->Context()->RSSetState(states->CullNone());
	deviceResources->Context()->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void WireframeDebugPass::VisualizeBounds()
{
	camera->BindCamera2Pipeline();		// Render from the perspective of the main camera
	wireframeMaterial->Bind();

	for (GameObject*& renderable : renderables)
	{
		renderable->Renderer().Bounds().DrawWireFrame
		(
			deviceResources, renderable->GetTransform().Object2WorldMatrix()
		);
	}
}
