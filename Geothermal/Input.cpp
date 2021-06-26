#include "pch.h"
#include "Input.h"

using namespace Geothermal;

Input::Input()
{
	ResetStates();
}

void Input::Update()
{
	keysDownThisFrame.clear();
	mouseMovement.x = 0.0f;
	mouseMovement.y = 0.0f;
}

void Input::RegisterInput(MSG* msg)
{
	UINT message = msg->message;
	switch (message)
	{
	case WM_KEYDOWN:
		OnKeyDown(msg->wParam, msg->lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(msg->wParam, msg->lParam);
		break;
	case WM_MOUSEMOVE:
		OnMouseMoved();
		break;
	}
}

bool Input::GetMouseButton()
{
	return false;
}

bool Input::GetMouseButtonDown()
{
	return false;
}

bool Input::GetKey(char key) const
{
	if (keysDown.count(key) != 0)
	{
		return true;
	}
	return false;
}

bool Input::GetKeyDown(char key) const
{
	if (keysDownThisFrame.count(key) != 0)
	{
		return true;
	}
	return false;
}

void Input::ResetStates()
{
	mouseMovement.x = 0.0f;
	mouseMovement.y = 0.0f;
	keysDown.clear();
	keysDownThisFrame.clear();
}

void Input::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	// We should only record the first key down in case of a long key hold
	bool isRepeating = lParam >> 29;
	if (!isRepeating)
	{
		keysDown.insert(wParam);
		keysDownThisFrame.insert(wParam);
	}
}

void Input::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	keysDown.erase(wParam);
}

void Input::OnMouseMoved()
{
	/*MouseDelta delta = args.MouseDelta();
	mouseMovement.x = static_cast<float>(delta.X);
	mouseMovement.y = static_cast<float>(delta.Y);*/
}
