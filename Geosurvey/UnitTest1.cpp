#include "pch.h"
#include "CppUnitTest.h"
#include "../Geothermal/Transform.h"
#include "../Geothermal/Transform.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
using namespace Geothermal;

namespace Geosurvey
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Transform transform;
			Assert::AreEqual(transform.GetObject2WorldMatrix(), XMMatrixIdentity());
		}
	};
}
