#pragma warning (disable : 6387)
#include "AllocManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#undef new
#define _MAX_INFO_COUNT	1000
#define _MAX_LINE_LEN	400

struct AllocInfo
{
	int _state;
	int _line;
	void* _ptr;
	size_t _size;
	char _file[_MAX_PATH];
};

class AllocManager
{
public:
	void Add(int state, void* ptr, size_t size, const char* file, int line);
	bool Remove(void* ptr);
	~AllocManager();

private:
	AllocInfo _info[_MAX_INFO_COUNT];

};

static AllocManager allocManager;

void* operator new(size_t size, const char* file, int line)
{
	void* ptr = malloc(size);

	allocManager.Add(1, ptr, size, file, line);

	return ptr;
}

void operator delete(void* ptr)
{
	if (allocManager.Remove(ptr))
		free(ptr);
	else
		allocManager.Add(2, ptr, 0, "\0", 0);
}

void AllocManager::Add(int state, void* ptr, size_t size, const char* file, int line)
{
	int index;

	for (index = 0; index < _MAX_INFO_COUNT; ++index)
	{
		if (_info[index]._state)
			continue;

		_info[index]._state = state;
		_info[index]._ptr = ptr;
		_info[index]._size = size;
		strcpy_s(_info[index]._file, _MAX_PATH, file);
		_info[index]._line = line;

		break;
	}
}

bool AllocManager::Remove(void* ptr)
{
	int index;

	for (index = 0; index < _MAX_INFO_COUNT; ++index)
	{
		if (_info[index]._state == 0)
			continue;

		if (_info[index]._ptr != ptr)
			continue;

		_info[index]._state = 0;

		return true;
	}

	return false;
}

AllocManager::~AllocManager()
{
	__time64_t time;
	struct tm Tm;
	FILE* file;
	char name[28];
	int index;
	char buf[_MAX_LINE_LEN];
	size_t size;

	_time64(&time);
	_localtime64_s(&Tm, &time);

	sprintf_s(
		name,
		28,
		"Alloc_%04d%02d%02d_%02d%02d%02d.txt",
		Tm.tm_year + 1900,
		Tm.tm_mon + 1,
		Tm.tm_mday,
		Tm.tm_hour,
		Tm.tm_min,
		Tm.tm_sec
	);

	fopen_s(&file, name, "w");

	for (index = 0; index < _MAX_INFO_COUNT; ++index)
	{
		if (_info[index]._state == 0)
			continue;

		if (_info[index]._state == 2)
			sprintf_s(buf, _MAX_LINE_LEN, "NOALLOC\t0x%p\n", _info[index]._ptr);
		else
			sprintf_s(
				buf,
				_MAX_LINE_LEN,
				"LEAK\t0x%p (%d byte) -> %s (line %d)\n",
				_info[index]._ptr,
				_info[index]._size,
				_info[index]._file,
				_info[index]._line
			);

		size = strnlen_s(buf, _MAX_LINE_LEN);

		fwrite(buf, 1, size, file);
	}

	fclose(file);
}
