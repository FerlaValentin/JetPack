#include "globalfunctions.h"
#include <stdlib.h>

namespace GLO {
    void* AllocateMemory(int num_of_elements, size_t size_of_elements) {
        return malloc(num_of_elements * size_of_elements);
    }
}