#pragma once

#include<unordered_set>

namespace Geothermal
{
	/// <summary>
	/// Unified input manager providing handling functions for all inputs
	/// </summary>
	class Input
	{
	public:
		Input();
		void Update();
		void RegisterInput(MSG* msg);

		bool GetMouseButton();
		bool GetMouseButtonDown();
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
