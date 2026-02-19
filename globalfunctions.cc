#include "globalfunctions.h"
#include <stdlib.h>

void* GLO::AllocateMemory(int num_of_elements, size_t size_of_elements){
    const int kScreenWidth = 512;
    const int kScreenHeight = 384;

    return malloc(num_of_elements * size_of_elements);
}