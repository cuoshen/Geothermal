#pragma once
#include "pch.h"
#include "Transform.h"
#include "GameObject.h"
#include "ConstantBuffer.h"
#include "DeviceResources.h"

using namespace DirectX;

namespace Geothermal
{
	/// <summary>
	/// Camera parameters constant buffer structure layout
	/// </summary>
	struct CameraParameters
	{
		XMMATRIX World2ClipTransform;
		XMFLOAT3 CameraWorldPosition;
	};

	/// <summary>
	/// Camera object which describes how we view the scene.
	/// Always manage with raw pointers, never use smart pointers.
	/// </summary>
	class Camera : public GameObject
	{
	public:
		Camera(float aspectRatio, float nearZ, float farZ, std::shared_ptr<Graphics::DeviceResources> deviceResources);
		~Camera();
		void Update() override;

		/// <summary>
		/// We always have precisely one main camera in the scene
		/// </summary>
		static Camera* Main();
		/// <summary>
		/// Set the main camera reference to this camera
		/// </summary>
		void SetMainToThis();

		XMMATRIX GetWorld2ClipMatrix();
		void BindCamera2Pipeline();

		float Pitch() { return pitch; }
		float Yaw() { return yaw; }
		void Pitch(float pitch) { this->pitch = pitch; }
		void Yaw(float yaw) { this->yaw = yaw; }
	private:
		const float mouseSensitivity = 0.001f;
		const float speed = 2.0f;
		const float pitchLimit = XM_PIDIV2 - 0.01f;
		void HandleMovement();
		void HandleRotation();

		XMMATRIX perspective;
		float aspectRatio;
		float nearZ;
		float farZ;
		float pitch;
		float yaw;

		CameraParameters parameters;
		std::unique_ptr<Graphics::Bindables::VertexConstantBuffer<CameraParameters>> parametersBufferVS;
		std::unique_ptr<Graphics::Bindables::PixelConstantBuffer<CameraParameters>> parametersBufferPS;

		static Camera* main;
	};
}
