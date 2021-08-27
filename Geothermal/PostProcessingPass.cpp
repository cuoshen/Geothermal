#include "pch.h"
#include "PostProcessingPass.h"

using namespace std;
using namespace DirectX;
using namespace Geothermal;
using namespace Graphics;
using namespace Passes;

PostProcessingPass::PostProcessingPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	std::unique_ptr<std::vector<Texture2D*>> source,
	std::unique_ptr<std::vector<Texture2D*>> sink
) : 
	RenderPass(deviceResources, move(source), move(sink)), 
	useBloom(false), exposure(1.0f)
{
	basicPostProcess = make_unique<BasicPostProcess>(deviceResources->Device());
	dualPostProcess = make_unique<DualPostProcess>(deviceResources->Device());
	toneMapper = make_unique<ToneMapPostProcess>(deviceResources->Device());
}

void PostProcessingPass::operator()()
{
	ID3D11ShaderResourceView* sceneTarget =
		(*source)[0]->UseAsShaderResource().get();
	if (useBloom)
	{
		ApplyBloom();
		sceneTarget = (*sink)[2]->UseAsShaderResource().get();
	}

	// Clear back buffer and target it in OutputMerger
	deviceResources->ClearFrame();
	// Target the backbuffer to stream output onto the screen
	ID3D11RenderTargetView* target = deviceResources->BackBufferTargetView();
	deviceResources->SetTargets(1, &target, nullptr);

	toneMapper->SetOperator(ToneMapPostProcess::Reinhard);
	toneMapper->SetHDRSourceTexture(sceneTarget);
	toneMapper->SetExposure(exposure);
	toneMapper->SetTransferFunction(ToneMapPostProcess::Linear);
	toneMapper->Process(deviceResources->Context());
}


void PostProcessingPass::ApplyBloom()
{
	// Clear both bloom targets
	for (int i = 0; i < 2; i++)
	{
		deviceResources->Context()->ClearRenderTargetView
		(
			(*sink)[i]->UseAsRenderTarget().get(), deviceResources->ClearColor
		);
	}

	ID3D11RenderTargetView* target = (*sink)[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomExtract);
	basicPostProcess->SetBloomExtractParameter(bloomThreshold);
	basicPostProcess->SetSourceTexture((*source)[0]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// Send blur to the other bloom texture
	target = (*sink)[1]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	basicPostProcess->SetBloomBlurParameters(true, bloomSize, bloomBrightness);
	basicPostProcess->SetSourceTexture((*sink)[0]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// And then back
	target = (*sink)[0]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	basicPostProcess->SetEffect(BasicPostProcess::BloomBlur);
	basicPostProcess->SetBloomBlurParameters(false, bloomSize, bloomBrightness);
	basicPostProcess->SetSourceTexture((*sink)[1]->UseAsShaderResource().get());
	basicPostProcess->Process(deviceResources->Context());

	// Finally combine to scene render buffer
	target = (*sink)[2]->UseAsRenderTarget().get();
	deviceResources->SetTargets(1, &target, nullptr);

	dualPostProcess->SetEffect(DualPostProcess::BloomCombine);
	dualPostProcess->SetBloomCombineParameters(1.0f, 1.0f, 1.0f, 1.0f);
	dualPostProcess->SetSourceTexture((*source)[0]->UseAsShaderResource().get());
	dualPostProcess->SetSourceTexture2((*sink)[0]->UseAsShaderResource().get());
	dualPostProcess->Process(deviceResources->Context());
}

void PostProcessingPass::SetUpPipelineStates()
{
	// For now DirectXTK post process will set their own pipeline states
	// so no work needs to be done here
}
