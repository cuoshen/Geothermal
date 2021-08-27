#include "pch.h"
#include "DeferredLightingPass.h"

using namespace Geothermal;
using namespace Graphics;
using namespace Passes;
using namespace Materials;
using namespace Structures;
using namespace Bindables;
using namespace std;
using namespace DirectX;

DeferredLightingPass::DeferredLightingPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	unique_ptr<vector<Texture2D*>> source,
	unique_ptr<vector<Texture2D*>> sink
) : 
	RenderPass(deviceResources, move(source), move(sink))
{
	states = make_unique<CommonStates>(deviceResources->Device());
	deferredDirectionalLit = 
		make_unique<Material>(deviceResources, L"FullscreenQuad.cso", L"DeferredLit.cso", nullptr, 0);
	parameterBuffer = 
		make_unique<PixelConstantBuffer<DeferredParameters>>(deviceResources, 3u);
}

void DeferredLightingPass::SetDelegates(function<void(void)> uploadShadowResources)
{
	this->uploadShadowResources = uploadShadowResources;
}

void DeferredLightingPass::SetParameters(XMFLOAT3 ambience, DirectionalLight mainLight, Camera* camera)
{
	parameters.Ambience = ambience;
	parameters.MainLight = mainLight;
	assert(camera != nullptr);
	parameters.ViewParameters = camera->GenerateDeferredViewParameters();
}

void DeferredLightingPass::SetUpPipelineStates()
{
	auto context = deviceResources->Context();
	context->OMSetBlendState(states->Opaque(), nullptr, 0xffffffff);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());
	context->RSSetViewports(1, &(deviceResources->ScreenViewport()));
}

void DeferredLightingPass::UploadConstantBuffers()
{
	parameterBuffer->Update(parameters);
	parameterBuffer->Bind();
}

void DeferredLightingPass::operator()()
{
	auto context = deviceResources->Context();
	SetUpPipelineStates();

	deferredDirectionalLit->Bind();

	// Set HDR target
	ID3D11RenderTargetView* target = (*sink)[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, deviceResources->DepthStencilView());

	// Bind Gbuffers as shader resources
	for (auto& texture : *source)
	{
		ID3D11ShaderResourceView* srv = texture->UseAsShaderResource().get();
		context->PSSetShaderResources(texture->Slot(), 1, &srv);
	}

	UploadConstantBuffers();
	uploadShadowResources();

	context->Draw(3, 0);
}
