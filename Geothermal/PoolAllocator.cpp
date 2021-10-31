#include "pch.h"
#include "PoolAllocator.h"

using namespace Geothermal;

void* PoolAllocator::Allocate()
{
	return nullptr;
}

void PoolAllocator::Deallocate(void* object)
{
}

PoolAllocator::Chunk* PoolAllocator::AllocateBlock()
{
	return nullptr;
}
