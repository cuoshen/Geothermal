#pragma once
#include "RenderPass.h"
#include "GameObject.h"
#include "Camera.h"

namespace Geothermal::Graphics::Passes
{
	/// <summary>
	/// Abstract base class for any pass that renders scene geometry from a single camera.
	/// </summary>
	class SceneGeometryPass : public RenderPass
	{
	public:
		SceneGeometryPass
		(
			std::shared_ptr<DeviceResources> const& deviceResources,
			std::unique_ptr<std::vector<Texture2D*>> source,
			std::unique_ptr<std::vector<Texture2D*>> sink
		);

		void SetSceneResources
		(
			std::list<GameObject*> renderables,
			Camera* camera
		);

	protected:
		/// <summary>
		/// Apply a frustum culling to renderables
		/// </summary>
		/// <returns>A list of game objects visible in the frustum</returns>
		std::list<GameObject*> Cull();

		std::list<GameObject*> renderables;
		Camera* camera;
	};
}
