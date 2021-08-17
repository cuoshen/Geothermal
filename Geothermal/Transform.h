#pragma once
#include "pch.h"

using namespace DirectX;

namespace Geothermal
{
	class Transform
	{
	public:
		/// <summary>
		/// Initialize the transform with identity
		/// </summary>
		Transform();
		/// <summary>
		/// Initialize transform with a 4x4 matrix
		/// </summary>
		Transform(XMMATRIX initialTransform);

		/// <summary>
		/// Apply transform matrix on current transform
		/// </summary>
		void ApplyTransform(XMMATRIX transform);
		/// <summary>
		/// Directly replace current transform with new transform
		/// </summary>
		void UpdateTransform(XMMATRIX newTransform);

		XMMATRIX Object2WorldMatrix() { return object2world; }

		XMVECTOR WorldPosition();
		XMVECTOR Front();
		XMVECTOR Up();

	private:

		// MAYBE: refactor this region into its own file
#pragma region TRANSFORM_CONVENTIONS
		/// <summary>
		/// Object space origin at (0,0,0,1)
		/// </summary>
		const XMVECTORF32 ORIGIN = { 0.0f, 0.0f, 0.0f, 1.0f };
		/// <summary>
		/// We define +z as front direction
		/// </summary>
		const XMVECTORF32 FRONT = { 0.0f, 0.0f, 1.0f, 0.0f };
		/// <summary>
		/// We define +y as up direction
		/// </summary>
		const XMVECTORF32 UP = { 0.0f, 1.0f, 0.0f, 0.0f };
		/// <summary>
		/// We define +x as left direction
		/// </summary>
		const XMVECTORF32 LEFT = { 1.0f, 0.0f, 0.0f, 0.0f };
#pragma endregion

		/// <summary>
		/// float4 worldPos = (matrix)object2world * (float4)objectPos
		/// </summary>
		XMMATRIX object2world;
	};
}
