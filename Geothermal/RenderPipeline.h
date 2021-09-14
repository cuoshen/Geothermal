#pragma once
#include "GraphicResources.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// Abstract base class for a render pipeline
	/// that is in charge of scheduling and dispatching rendering tasks.
	/// </summary>
	class RenderPipeline
	{
	public:
		RenderPipeline(std::shared_ptr<DeviceResources> const& deviceResources) : 
			deviceResources(deviceResources)
		{
		}
		virtual void Render() = 0;

	protected:
		std::shared_ptr<DeviceResources> deviceResources;
	};
}