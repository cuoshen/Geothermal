#pragma once
#include "GraphicResources.h"
#include "RenderPass.h"
#include "GameObject.h"
#include "Camera.h"

namespace Geothermal::Graphics::Passes
{
	class SimpleForwardPass : public RenderPass
	{
	public:
		SimpleForwardPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::vector<Texture2D*> const& source,
			std::vector<Texture2D*> const& sink
		);

		void AddResources
		(
			std::list<GameObject*> renderables,
			Camera* camera
		);

		void operator()() override;

	protected:
		std::list<GameObject*> renderables;
		Camera* camera;
	};
}
