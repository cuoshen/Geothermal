#pragma once
#include "DeviceResources.h"
#include "GameObject.h"

namespace Geothermal::Graphics
{
	class RenderPass
	{
	public:

		RenderPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<ID3D11ShaderResourceView*> const& source,
			std::vector<GameObject*> const& renderables,
			std::vector<ID3D11RenderTargetView*> const& sink
		);

		/// <summary>
		/// Execute the pass
		/// </summary>
		virtual void operator()() = 0;

	protected:
		std::shared_ptr<DeviceResources> deviceResources;
	};
}
