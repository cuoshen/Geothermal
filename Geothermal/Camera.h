#pragma once
#include "pch.h"
#include "Transform.h"
#include "GameObject.h"
#include "GraphicResources.h"
#include "ViewPoint.h"
#include "ShadingAttributes.h"

namespace Geothermal
{
	/// <summary>
	/// Camera object which describes how we view the scene.
	/// </summary>
	class Camera : public Graphics::ViewPoint, public GameObject
	{
	public:
		Camera
		(
			float aspectRatio, float nearZ, float farZ, 
			std::shared_ptr<Graphics::DeviceResources> const& deviceResources
		);

		void Update() override;

		DirectX::XMMATRIX World2View();
		DirectX::XMMATRIX World2Clip();
		void BindCamera2Pipeline();
		Graphics::Structures::DeferredViewParameters GenerateDeferredViewParameters();

		float Pitch() { return pitch; }
		float Yaw() { return yaw; }
		void Pitch(float pitch) { this->pitch = pitch; }
		void Yaw(float yaw) { this->yaw = yaw; }

		static void* operator new(size_t size);
		static void operator delete(void* pointerToObject, size_t size);

	private:

		const float mouseSensitivity = 0.001f;
		const float speed = 2.0f;
		const float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;
		void HandleMovement();
		void HandleRotation();

		float pitch;
		float yaw;
	};
}
