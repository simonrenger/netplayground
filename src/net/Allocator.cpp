#include "net/Allocator.h"
#include <cstdlib>

bool eyos::net::Allocator::Init([[maybe_unused]]std::size_t bufferSize)
{
	inited = true;
	return true;
}


eyos::net::Allocator::~Allocator()
{
}

void* eyos::net::Allocator::Allocate(std::size_t size)
{
	return std::malloc(size); // fixme: add actual strategy
}


void eyos::net::Allocator::Free(void* ptr)
{
	std::free(ptr); // fixme: add actual strategy
}

eyos::net::Allocator& eyos::net::GetNetworkAllocator() {
    static eyos::net::Allocator allocator{};
    return allocator;
}
