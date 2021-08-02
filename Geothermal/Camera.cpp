#include "pch.h"
#include "Camera.h"
#include "GameMain.h"
#include "Input.h"
#include <algorithm>

using namespace Geothermal;
using namespace Graphics;
using namespace Bindables;
using namespace std;

Camera* Camera::main;

Camera::Camera(float aspectRatio, float nearZ, float farZ, shared_ptr<DeviceResources> const& deviceResources) :
	GameObject(),
	ViewPoint(deviceResources, aspectRatio, nearZ, farZ),
	pitch(0.0f), yaw(0.0f)
{
	// The first camera created automatically becomes the main camera
	if (main == nullptr)
	{
		main = this;
	}
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
	float dt = GameMain::Instance()->DeltaTime();
	float ds = dt * speed;
	Input* input = GameMain::Instance()->GetInput();
	// All movement is calculated relative to the object itself.
	XMVECTOR front = XMVector4Transform(transform->Front(), XMMatrixRotationY(yaw));
	XMVECTOR up = transform->Up();
	XMVECTOR left = XMVector3Cross(front, up);

	if (input->GetKey('W'))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(ds * front));
	}
	else if (input->GetKey('S'))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-ds * front));
	}

	if (input->GetKey('A'))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(ds * left));
	}
	else if (input->GetKey('D'))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-ds * left));
	}

	if (input->GetKeyDown(VK_SPACE))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(0.5f * up));
	}
	else if (input->GetKeyDown(VK_CONTROL))
	{
		transform->ApplyTransform(XMMatrixTranslationFromVector(-0.5f * up));
	}
}

void Camera::HandleRotation()
{
	Input* input = GameMain::Instance()->GetInput();
	if (input->GetKey('X'))
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

XMMATRIX Camera::World2View()
{
	XMVECTOR eye = transform->WorldPosition();
	XMVECTOR eyeDirection = transform->Front();
	eyeDirection = XMVector4Transform(eyeDirection, XMMatrixRotationX(pitch) * XMMatrixRotationY(yaw));
	return XMMatrixLookToLH(eye, eyeDirection, transform->Up());
}

XMMATRIX Camera::World2Clip()
{
	XMMATRIX world2clip = World2View();
	world2clip *= Perspective();
	return world2clip;
}

void Camera::BindCamera2Pipeline()
{
	Bind(World2View());
}
