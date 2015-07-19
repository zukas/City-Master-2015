#include "glprogram.h"
#include "../Memory/utils.h"

void check_alignment()
{
	for(int i = 0; i < 512; ++i)
	{
		printf("Value: %d, aligned 16: %lu, aligned 32: %lu, aligned 64: %lu\n", i, align_block_16(i),align_block_32(i),align_block_64(i));
	}
}

void check_alloc()
{

}


int main()
{
//	check_alignment();
	glProgram p;
	p.exec();


    return 0;
}
