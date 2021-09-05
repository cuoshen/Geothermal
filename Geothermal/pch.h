#pragma once

#include <memory>
#include <assert.h>
#include <DDSTextureLoader.h>
#include <unknwn.h>
#include <windows.h>
#include <windowsx.h>
#include <wincodec.h>

#include <d3d11_3.h>
#include <DirectXMath.h>
#include <d2d1_3.h>

#include <winrt\base.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"windowsapp.lib")

#undef min
#undef max

#include "GeothermalTypes.h"

// Disable warning messages 4244 and 4267 (possible lost of data on type cast)
#pragma warning( disable : 4244 4267)
