// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#include <memory>
#include <assert.h>
#include <unknwn.h>
#include <windows.h>
#include <windowsx.h>

#include <d3d11_3.h>
#include <DirectXMath.h>
#include <d2d1_3.h>
#include <wrl.h>

#include <winrt\base.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"windowsapp.lib")

#undef min
#undef max

#include "GeothermalTypes.h"

// Disable warning messages 4244 and 4267 (possible lost of data on type cast)
#pragma warning( disable : 4244 4267)
