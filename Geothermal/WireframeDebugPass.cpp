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
	shared_ptr<DeviceResources> const& deviceResources, 
	vector<Texture2D*> const& source, 
	vector<Texture2D*> const& sink
) : 
	RenderPass(deviceResources, source, sink),
	camera(nullptr), states(nullptr)
{
	states = make_unique<CommonStates>(deviceResources->Device());
	// Initialize wireframe material
	// Use the WireframeVertexShader + Unlit combo
	wireframeMaterial = 
		make_unique<Material>(deviceResources, L"WireframeVertexShader", L"Unlit", VertexPLayout, size(VertexPLayout));
}

void WireframeDebugPass::SetResources(list<GameObject*> renderables, Camera* camera)
{
	this->camera = camera;
	this->renderables = renderables;
}

void WireframeDebugPass::operator()()
{
	SetUpPipelineStates();
	ID3D11RenderTargetView* target = sink[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, deviceResources->DepthStencilView());

	VisualizeBounds();
}

void WireframeDebugPass::SetUpPipelineStates()
{
	deviceResources->Context()->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	deviceResources->Context()->OMSetDepthStencilState(states->DepthNone(), 0);

	// Set rasterizer state to wireframe
	deviceResources->Context()->RSSetState(states->Wireframe());

	deviceResources->Context()->ClearRenderTargetView
	(
		sink[0]->UseAsRenderTarget().get(), deviceResources->ClearColor
	);
	deviceResources->Context()->ClearDepthStencilView
	(
		deviceResources->DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
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
