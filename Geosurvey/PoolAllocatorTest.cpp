#include "pch.h"
#include "CppUnitTest.h"
#include "PoolAllocator.h"

#include "PoolAllocator.cpp"

using namespace Geothermal;

namespace Geosurvey
{
	constexpr uint64 chunksInBlock = 4;

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
		/// and verify that they are indeed adjacent to each other in underlying memory
		/// </summary>
		TEST_METHOD(MemoryAdjacencyTest)
		{
			PoolAllocatedObject* collection[chunksInBlock];
			std::wstringstream allocationSizeStr;
			allocationSizeStr << L"About to allocate " << chunksInBlock << L" objects \n";
			OutputDebugString(allocationSizeStr.str().c_str());

			std::wstringstream memoryLocationStr;

			for (uint64 i = 0; i < chunksInBlock; i++)
			{
				collection[i] = new PoolAllocatedObject();
				memoryLocationStr << L"Memory location: [" << collection[i] << L"] \n";
			}

			OutputDebugString(memoryLocationStr.str().c_str());

			for (uint64 i = 0; i < chunksInBlock; i++)
			{
				delete collection[i];
			}
		}
	};

	PoolAllocator PoolAllocatedObject::allocator(chunksInBlock, sizeof(PoolAllocatedObject));
}
