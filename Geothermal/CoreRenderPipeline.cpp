#include "pch.h"
#include <d3dcompiler.h>
#include "CoreRenderPipeline.h"
#include "Vertices.h"
#include "Shaders.h"
#include "GameMain.h"
#include "Scene.h"

#ifdef DEBUG_SHAPES
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "QuadMesh.h"
#include "ModelLoader.h"
using namespace Geothermal::Graphics::Meshes;
using namespace Geothermal;
#endif

using namespace Geothermal::Graphics;
using namespace Bindables;
using namespace Structures;
using namespace std;
using namespace DirectX;

CoreRenderPipeline::CoreRenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources):
	deviceResources(deviceResources), camera(nullptr)
{
	LoadAllShaders();
	camera = make_unique<Camera>(deviceResources->AspectRatio(), 0.1f, 100.0f, deviceResources);
#ifdef DEBUG_SHAPES
	LoadDebugMesh();
#endif
	OutputDebugString(L"Core Renderer ready \n");
}

// Load unlit shader for debug purposes
// TODO: refactor into (Shader Cache + Material) system in support of custom shader
void CoreRenderPipeline::LoadAllShaders()
{
	PixelShader unlitPixelShader(deviceResources, L"ForwardLit.cso");
	VertexShader defaultVertexShader(deviceResources, L"VertexShader.cso", VertexPNTLayout, (UINT)size(VertexPNTLayout));
	unlitPixelShader.Bind();
	defaultVertexShader.Bind();
}

void CoreRenderPipeline::Render()
{
	deviceResources->SetTargets();	// Always set target to current back buffer before drawing
	deviceResources->ClearView();		// Clear the view before we start drawing
	camera->Update();
	camera->BindCamera2Pipeline();

	// Simple forward rendering
	// For each object we render it in a single pass
	for (GameObject*& gameObject : Scene::Instance()->ObjectsInScene)
	{
		gameObject->Render();
	}

#ifdef DEBUG_SHAPES
	DrawDebugMesh();
#endif

	deviceResources->Present();
}

#ifdef DEBUG_SHAPES
void CoreRenderPipeline::DrawDebugTriangle()
{
	VertexPNT verts[] =
	{
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	};

	vector<VertexPNT> vertices(begin(verts), end(verts));
	VertexBuffer<VertexPNT> triangleVertex(deviceResources, vertices);
	triangleVertex.Bind();
	VertexConstantBuffer<XMMATRIX> transform(deviceResources, XMMatrixScaling(2.0f, 2.0f, 1.0f), 0u);
	transform.Bind();
	deviceResources->D3DDeviceContext()->Draw(triangleVertex.GetVertexCount(), 0);
}

void CoreRenderPipeline::DrawDebugQuad()
{
	QuadMesh quadMesh(deviceResources);
	quadMesh.vertices->Bind();
	float angle = GameMain::Instance()->GetTime();
	VertexConstantBuffer<XMMATRIX> transform(
		deviceResources, 
		XMMatrixTranspose(XMMatrixRotationY(angle) * XMMatrixTranslation(0.0f, 0.0f, 2.0f)),
		0u
	);
	transform.Bind();
	deviceResources->D3DDeviceContext()->DrawIndexed(quadMesh.vertices->GetIndexCount(), 0, 0);
}

void CoreRenderPipeline::LoadDebugMesh()
{
	ModelLoader loader;
	debugMesh = new Mesh();
	bool loaded =
		loader.LoadObj2Mesh(L"Assets\\stanford_dragon.obj", L"Assets\\stanford_dragon.mtl", debugMesh, deviceResources);
	assert(loaded);

	debugModelTransform = new VertexConstantBuffer<XMMATRIX>(deviceResources, 0u);
	shadingParameters = PhongAttributes
	{
		{0.0f, 0.0f, 0.0f, 0.0f},	// Ambient
		{1.0f, 1.0f, 1.0f, 1.0f},	// Base color
		1.5f,									// Diffuse
		1.0f,									// Specular
		40.0f,									// Smoothness
		0.0f										// Padding
	};
}

void CoreRenderPipeline::DrawDebugMesh()
{
	debugMesh->vertices->Bind();

	debugModelTransform->Update(
		XMMatrixTranspose
		(
			XMMatrixTranslation(0.0f, 0.0f, 4.0f)
		)
	);
	debugModelTransform->Bind();

	PixelConstantBuffer<PhongAttributes> unlitProperties(deviceResources, shadingParameters, 2u);
	unlitProperties.Bind();

	deviceResources->D3DDeviceContext()->Draw(debugMesh->vertices->GetVertexCount(), 0);
}
#endif
