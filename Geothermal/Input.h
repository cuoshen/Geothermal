#pragma once

#include<unordered_set>

namespace Geothermal
{
	enum class MouseButtons
	{
		LEFT = VK_LBUTTON,
		RIGHT = VK_RBUTTON,
		MIDDLE = VK_MBUTTON
	};

	/// <summary>
	/// Unified input manager providing handling functions for all inputs
	/// </summary>
	class Input
	{
	public:
		Input();
		void Update();
		void RegisterInput(MSG* msg);

		bool GetMouseButton(MouseButtons button);
		bool GetMouseButtonDown(MouseButtons button);
		/// <summary>
		/// Returns true while the key is being pressed down
		/// </summary>
		bool GetKey(char key) const;
		/// <summary>
		/// Returns if the key is STARTING to being pressed down at the same frame
		/// </summary>
		bool GetKeyDown(char key) const;

		DirectX::XMFLOAT2 GetMouseMovement() const { return mouseMovement; }

	private:
		void ResetStates();
		void OnKeyDown
		(
			WPARAM      wParam,
			LPARAM      lParam
		);
		void OnKeyUp
		(
			WPARAM      wParam,
			LPARAM      lParam
		);
		void OnMouseDown
		(
			WPARAM      wParam,
			LPARAM      lParam
		);
		void OnMouseUp
		(
			WPARAM      wParam,
			LPARAM      lParam
		);
		void OnMouseMoved(
			WPARAM      wParam,
			LPARAM      lParam
		);
		std::unordered_set<char> keysDown;
		std::unordered_set<char> keysDownThisFrame;
		DirectX::XMINT2 mousePosition;
		DirectX::XMFLOAT2 mouseMovement;
	};
}
