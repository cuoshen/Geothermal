#include "pch.h"
#include "PoolAllocator.h"

using namespace Geothermal;

void* PoolAllocator::Allocate()
{
	if (nextAllocationPosition == nullptr)
	{
		nextAllocationPosition = AllocateBlock();
	}

	Chunk* freeChunk = nextAllocationPosition;
	nextAllocationPosition = nextAllocationPosition->next;

	return freeChunk;
}

void PoolAllocator::Deallocate(void* object)
{
	reinterpret_cast<Chunk*>(object)->next = nextAllocationPosition;
	nextAllocationPosition = reinterpret_cast<Chunk*>(object);
}

PoolAllocator::Chunk* PoolAllocator::AllocateBlock()
{
	size_t blockSize = chunksPerBlock * chunkSize;
	Chunk* begin = reinterpret_cast<Chunk*>(malloc(blockSize));
	Chunk* chunk = begin;
	for (int i = 0; i < chunksPerBlock - 1; i++)
	{
		chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunkSize);
		chunk = chunk->next;
	}
	return begin;
}
