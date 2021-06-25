#pragma once
#include "DeviceResources.h"
#include "pch.h"

namespace Geothermal::Graphics::Bindables
{
	/// <summary>
	/// Abstract class for any bindable graphic resources
	/// </summary>
	class Bindable
	{
	public:
		Bindable(std::shared_ptr<DeviceResources> const& deviceResources): deviceResources(deviceResources)
		{
		}

		/// <summary>
		/// Bind the resource to the rendering pipeline
		/// </summary>
		virtual void Bind() = 0;
	protected:
		std::shared_ptr<DeviceResources> deviceResources;
	};
}