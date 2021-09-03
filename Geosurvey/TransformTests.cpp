#include "pch.h"
#include "CppUnitTest.h"
#include "Transform.h"
#include "Comparisons.h"

#include "Transform.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX;
using namespace Geothermal;

namespace Geosurvey
{
	TEST_CLASS(TransformTests)
	{
	public:
		TEST_METHOD(TransformTest)
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
