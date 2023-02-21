#pragma once

namespace Geothermal
{
	class PoolAllocator
	{
	public:
		PoolAllocator(u64 chunksPerBlock, size_t chunkSize) : chunksPerBlock(chunksPerBlock), chunkSize(chunkSize)
		{
		}

		void* Allocate();
		void Deallocate(void* object);

	private:
		struct Chunk
		{
			Chunk* next;
		};
		u64 chunksPerBlock;
		size_t chunkSize;

		Chunk* nextAllocationPosition = nullptr;
		Chunk* AllocateBlock();
	};
}
