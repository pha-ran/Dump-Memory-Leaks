#include "AllocManager.h"
#include <stdio.h>
#include <stdlib.h>

#undef new

struct AllocInfo
{
	void* ptr;
	size_t size;
	char file[_MAX_PATH];
	int line;
};

class AllocManager
{
public:
	~AllocManager();
};

AllocManager allocManager;

void* operator new(size_t size, const char* file, int line)
{
	printf("new %d\n", size);
	return malloc(size);
}

void operator delete(void* ptr)
{
	printf("delete %p\n", ptr);
}

AllocManager::~AllocManager()
{
	;
}
