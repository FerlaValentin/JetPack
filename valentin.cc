#include "valentin.h"
#include <stdlib.h>

void* VFL::AllocateMemory(int num_of_elements, size_t size_of_elements){
    return malloc(num_of_elements * size_of_elements);
}