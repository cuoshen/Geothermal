#ifndef GEOTHERMAL_SHADOW
#define GEOTHERMAL_SHADOW

Texture2D ShadowMap : register(SHADOW_MAP_SLOT);

#define SHADOW_BIAS 0.0001f

bool IsInShadow(float4 lightSpacePosition, SamplerState samplerState)
{
	// Project into Shadow map coordinates
	float2 projectedPosition;
	projectedPosition.x = (lightSpacePosition.x / lightSpacePosition.w / 2.0f) + 0.5f;
	projectedPosition.y = (-lightSpacePosition.y / lightSpacePosition.w / 2.0f) + 0.5f;

	// Check whether both x and y falls within (0,1), if so it may fall in shadow
	if
		(
			(saturate(projectedPosition.x) == projectedPosition.x) &&
			(saturate(projectedPosition.y) == projectedPosition.y)
			)
	{
		float depthReachedByLight = ShadowMap.Sample(samplerState, projectedPosition).r;
		depthReachedByLight += SHADOW_BIAS;
		float depthOfPoint = lightSpacePosition.z / lightSpacePosition.w;

		return (depthOfPoint > depthReachedByLight);
	}

	return false;		// Unaffected by this shadow casting light
}

#endif