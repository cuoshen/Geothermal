#include "pch.h"
#include "CppUnitTest.h"
#include "PoolAllocator.h"

#include "PoolAllocator.cpp"

using namespace Geothermal;

namespace Geosurvey
{
	constexpr uint64 chunksInBlock = 10;

	struct PoolAllocatedObject
	{
		PoolAllocatedObject() : data()
		{
		}

		// 16 Bytes of data
		float data[4];

		static PoolAllocator allocator;

		static void* operator new(size_t size)
		{
			OutputDebugString(L"Override operator new called \n");
			assert(size == sizeof(PoolAllocatedObject));
			return allocator.Allocate();
		}

		static void operator delete(void* pointerToObject, size_t size)
		{
			OutputDebugString(L"Override operator delete called \n");
			assert(size == sizeof(PoolAllocatedObject));
			allocator.Deallocate(pointerToObject);
		}
	};

	struct PlainObject
	{
		PlainObject() : data()
		{
		}

		// 16 Bytes of data
		float data[4];
	};

	TEST_CLASS(PoolAllocatorTests)
	{
	public:
		/// <summary>
		/// Test if the custom operator new and delete for PoolAllocatedObject is working properly
		/// </summary>
		TEST_METHOD(AllocateSingleObject)
		{
			// Verify that the object is of desired size
			std::wstringstream testVector4SizeStr;
			testVector4SizeStr << L"sizeof(TestVector4) = " << sizeof(PoolAllocatedObject) << L"\n";
			OutputDebugString(testVector4SizeStr.str().c_str());
			PoolAllocatedObject* testVector = new PoolAllocatedObject();
			delete testVector;
		}

		/// <summary>
		/// We allocate #chunksInBlock chunks with the custom new operator, 
		/// and verify that they are indeed adjacent to each other in underlying memory.
		/// Compare results with plain objects with default new operator.
		/// </summary>
		TEST_METHOD(MemoryAdjacencyTest)
		{
			// The last element should be in a new block, thus may not be adjacent in pool allocated case
			constexpr uint64 allocationCount = chunksInBlock + 1;
			PlainObject* plainCollection[allocationCount];
			PoolAllocatedObject* poolCollection[allocationCount];

			std::wstringstream allocationSizeStr;
			allocationSizeStr << L"About to allocate " << allocationCount << L" objects \n";
			OutputDebugString(allocationSizeStr.str().c_str());

			std::wstringstream plainMemoryLocationStr;
			plainMemoryLocationStr << L"Plain object memory locations : \n";
			std::wstringstream poolMemoryLocationStr;
			poolMemoryLocationStr << L"Pool allocated object memory locations : \n";

			for (uint64 i = 0; i < allocationCount; i++)
			{
				plainCollection[i] = new PlainObject();
				poolCollection[i] = new PoolAllocatedObject();
				plainMemoryLocationStr << L"[" << plainCollection[i] << L"] \n";
				poolMemoryLocationStr << L"[" << poolCollection[i] << L"] \n";
			}

			OutputDebugString(plainMemoryLocationStr.str().c_str());
			OutputDebugString(poolMemoryLocationStr.str().c_str());

			for (uint64 i = 0; i < allocationCount; i++)
			{
				delete plainCollection[i];
				delete poolCollection[i];
			}
		}
	};

	PoolAllocator PoolAllocatedObject::allocator(chunksInBlock, sizeof(PoolAllocatedObject));
}
