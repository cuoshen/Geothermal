#include "pch.h"
#include "RenderPass.h"

using namespace std;
using namespace Geothermal;
using namespace Graphics;

RenderPass::RenderPass
(
	shared_ptr<DeviceResources> const& deviceResources, 
	vector<ID3D11ShaderResourceView*> const& source, 
	vector<GameObject*> const& renderables, 
	std::vector<ID3D11RenderTargetView*> const& sink
):
	deviceResources(deviceResources)
{
}
