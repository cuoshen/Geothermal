#pragma once


namespace Geothermal
{
	class PoolAllocator
	{
	public:
		PoolAllocator(uint64 chunksPerBlock) : chunksPerBlock(chunksPerBlock) 
		{
		}

	private:
		uint64 chunksPerBlock;
	};
}
