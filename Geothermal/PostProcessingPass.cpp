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
	vector<Texture2D*> const& source, 
	vector<Texture2D*> const& sink
) : 
	RenderPass(deviceResources, source, sink), useBloom(false), exposure(1.0f)
{
	basicPostProcess = make_unique<BasicPostProcess>(deviceResources->Device());
	dualPostProcess = make_unique<DualPostProcess>(deviceResources->Device());
	toneMapper = make_unique<ToneMapPostProcess>(deviceResources->Device());
}

void PostProcessingPass::ApplyBloom()
{
}

void PostProcessingPass::operator()()
{
	ID3D11ShaderResourceView* sceneTarget =
		sink[0]->UseAsShaderResource().get();
	if (useBloom)
	{
		ApplyBloom();
		sceneTarget = sink[1]->UseAsShaderResource().get();
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
