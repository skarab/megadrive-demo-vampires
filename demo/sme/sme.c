#include "sme.h"

void sme_Initialize(int hard)
{
    if (hard==0)
    {
        // handle reset
    }
    
    smeVDP_Initialize();
    smeINPUTS_Initialize();
}

void sme_Finalize()
{
    smeVDP_Finalize();
}

void sme_Update()
{
    smeTIME_Update();
    smeINPUTS_Update();
}

void* MEM_realloc(void* ptr, u16 old_size, u16 new_size)
{
	if (new_size == old_size)
		return ptr;
	void* buffer = NULL;
	if (new_size>0)
		buffer = MEM_alloc(new_size);
	if (ptr != NULL)
	{
		if (new_size>0)
		{
			int size = new_size<old_size ? new_size : old_size;
			memcpy(buffer, ptr, size);
		}
		MEM_free(ptr);
	}
	return buffer;
}

void memcpy_bytes(void *to, const void *from, int len)
{
	const char* in = (const char*)from;
	char* out = (char*)to;
	int i;
	for (i = 0; i<len; ++i)
		*out++ = *in++;
}