#pragma once
#include "Texture2D.h"

namespace Geothermal::Graphics
{
	/// <summary>
	/// A shadow map is a depth stencil | shader resource texture
	/// It should be null initialized since we typically write to it every frame
	/// </summary>
	class ShadowMap : public Texture2D
	{

	};
}