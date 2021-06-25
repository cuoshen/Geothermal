#include "pch.h"
#include "Camera.h"
#include "GameMain.h"
#include "Input.h"
#include <algorithm>

using namespace Geothermal;
using namespace Graphics::Bindables;
using namespace winrt::Windows::System;
using namespace std;

Camera* Camera::main;

Camera::Camera(float aspectRatio, float nearZ, float farZ, shared_ptr<DeviceResources> deviceResources):
	GameObject(),
	aspectRatio(aspectRatio), nearZ(nearZ), farZ(farZ), pitch(0.0f), yaw(0.0f), parametersBufferVS(nullptr)
{
	perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, nearZ, farZ);
	// The first camera created automatically becomes the main camera
	if (main == nullptr)
	{
		main = this;
	}
	parametersBufferVS = make_unique<VertexConstantBuffer<CameraParameters>>(deviceResources, 1u);
	parametersBufferPS = make_unique<PixelConstantBuffer<CameraParameters>>(deviceResources, 1u);
}

Camera::~Camera()
{
	// Unregister to main camera automatically if the camera is deleted
	if (main == this)
	{
		main = nullptr;
	}
}

void Camera::Update()
{
	// TODO: Add scripting layer to the engine so that we can factor out game code from engine code
	HandleMovement();
	HandleRotation();
}

void Camera::HandleMovement()
{
	float dt = GameMain::GetInstance()->GetDeltaTime();
	const float speed = 2.0f;
	float ds = dt * speed;
	Input* input = GameMain::GetInstance()->GetInput();
	// All movement is calculated relative to the object itself.
	XMVECTOR front = transform->Front();
	XMVECTOR up = transform->Up();
	XMVECTOR left = XMVector3Cross(front, up);

	if (input->GetKey(VirtualKey::W))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(ds * front));
	}
	else if (input->GetKey(VirtualKey::S))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-ds * front));
	}

	if (input->GetKey(VirtualKey::A))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(ds*left));
	}
	else if (input->GetKey(VirtualKey::D))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-ds * left));
	}

	if (input->GetKeyDown(VirtualKey::Space))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(1.0f * up));
	}
	else if (input->GetKeyDown(VirtualKey::Control))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-1.0f * up));
	}
}

void Camera::HandleRotation()
{
	Input* input = GameMain::GetInstance()->GetInput();
	if (input->GetKey(VirtualKey::X))
	{
		XMFLOAT2 rotation = input->GetMouseMovement();
		pitch += rotation.y * mouseSensitivity;
		pitch = clamp(pitch, -pitchLimit, pitchLimit);
		yaw += rotation.x * mouseSensitivity;
	}
}

Camera* Camera::Main()
{
	assert(main != nullptr);
	return main;
}

void Camera::SetMainToThis()
{
	if (main != this)
	{
		main = this;
	}
}

XMMATRIX Camera::GetWorld2ClipMatrix()
{
	XMVECTOR eye = transform->WorldPosition();
	XMVECTOR eyeDirection = transform->Front();
	eyeDirection = XMVector4Transform(eyeDirection, XMMatrixRotationX(pitch) * XMMatrixRotationY(yaw));
	XMMATRIX world2clip = XMMatrixLookToLH(eye, eyeDirection, transform->Up());
	world2clip *= perspective;
	return world2clip;
}

void Camera::BindCamera2Pipeline()
{
	 XMStoreFloat3(&(parameters.CameraWorldPosition), transform->WorldPosition());
	 parameters.World2ClipTransform = XMMatrixTranspose(GetWorld2ClipMatrix());

	 parametersBufferVS->Update(parameters);
	 parametersBufferPS->Update(parameters);
	 parametersBufferVS->Bind();
	 parametersBufferPS->Bind();
}
