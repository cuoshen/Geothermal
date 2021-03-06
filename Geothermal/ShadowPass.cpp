#include "pch.h"
#include "ShadowPass.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace Geothermal;
using namespace SceneManagement;
using namespace Graphics;
using namespace Passes;
using namespace std;
using namespace DirectX;

ShadowPass::ShadowPass
(
	shared_ptr<DeviceResources> const& deviceResources
) : 
	RenderPass(deviceResources, nullptr, nullptr),
	shadowCaster(deviceResources, 30.0f, 30.0f, 0.0f, 1000.0f)
{
	shadowViewPort = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		shadowMapDimensions.x,
		shadowMapDimensions.y
	);
	mainShadowMap =
		make_shared<ShadowMap>(deviceResources, shadowMapDimensions.x, shadowMapDimensions.y);
}

XMMATRIX ShadowPass::UpdateWorld2Light(XMVECTOR castingOrigin, XMVECTOR lightDirection)
{
	XMFLOAT3 upFloat3 = XMFLOAT3{ 0.0f, 1.0f, 0.0f };
	XMVECTOR up = XMLoadFloat3(&upFloat3);

	world2light = XMMatrixLookToLH(castingOrigin, lightDirection, up);
	return world2light;
}

XMMATRIX ShadowPass::CasterPerspective()
{
	return shadowCaster.Perspective();
}

void ShadowPass::operator()()
{
	SetUpPipelineStates();

	// Render from the perspective of the main light
	// Assuming a call to UpdateWorld2Light earlier in the same frame
	shadowCaster.Bind(world2light);

	for (GameObject*& gameObject : SceneManager::Instance().GameObjectRegistry)
	{
		gameObject->Render();
	}
}

void ShadowPass::SetUpPipelineStates()
{
	deviceResources->ResetDefaultPipelineStates();
	// Render the depth to mainShadowMap
	deviceResources->Context()->ClearDepthStencilView
	(
		mainShadowMap->UseAsDepthStencil().get(), D3D11_CLEAR_DEPTH, 1.0f, 0
	);
	deviceResources->Context()->RSSetViewports(1, &shadowViewPort);
	deviceResources->SetTargets(0, nullptr, mainShadowMap->UseAsDepthStencil().get());
}
