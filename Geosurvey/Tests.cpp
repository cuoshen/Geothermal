#include "pch.h"
#include "CppUnitTest.h"
#include "../Geothermal/Transform.h"
#include "../Geothermal/Transform.cpp"
#include "Comparisons.h"

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
			XMFLOAT3 worldPos;
			Assert::IsTrue(AreEqual(transform.WorldPosition(), XMFLOAT3{ 0.0f, 0.0f, 0.0f }));
			Assert::IsTrue(AreEqual(transform.Front(), XMFLOAT3{ 0.0f, 0.0f, 1.0f }));

			transform.ApplyTransform(XMMatrixRotationZ(XM_PIDIV4));
			Assert::IsTrue(AreEqual(transform.WorldPosition(), XMFLOAT3{ 0.0f, 0.0f, 0.0f }));
			Assert::IsTrue(AreEqual(transform.Front(), XMFLOAT3{ 0.0f, 0.0f, 1.0f }));

			transform.ApplyTransform(XMMatrixTranslation(0.0f, 1.0f, 1.0f));
			Assert::IsTrue(AreEqual(transform.WorldPosition(), XMFLOAT3{ 0.0f, 1.0f, 1.0f }));
		}
	};
}
