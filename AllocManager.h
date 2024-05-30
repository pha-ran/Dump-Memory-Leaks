#pragma once

void* operator new (size_t size, const char* file, int line);
void operator delete (void* ptr);

#define new new(__FILE__, __LINE__)
