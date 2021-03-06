// Map each type of resource to a dedicated slot

#ifndef GEOTHERMAL_SLOT_CONFIG
#define GEOTHERMAL_SLOT_CONFIG

// Constant buffers

#define TRANSFORM_BUFFER_SLOT b0
#define VIEW_PARAMETERS_SLOT b1
#define PROPERTIES_SLOT b2

#define DEFERRED_PARAMETERS_SLOT b3

#define SHADOW_TRANSFORM_SLOT b5
#define LIGHTS_SLOT b7

// Textures

#define ALBEDO_MAP_SLOT t0
#define NORMAL_MAP_SLOT t1
#define SHADOW_MAP_SLOT t5

// GBuffers

#define GBUFFER_0_SLOT t0
#define GBUFFER_1_SLOT t1
#define GBUFFER_DEPTH_SLOT t2

#endif
