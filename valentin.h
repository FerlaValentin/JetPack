#pragma once

namespace VFL{
    /*Crea un bloque de memoria de tipo void para después castearlo a cualquier tipo de puntero
        param num_of_elements Cuántos elementos debe tener el bloque de memoria
        param size_of_elements Un sizeof del tipo de dato que va a guardar el puntero
    */
    void* AllocateMemory(int num_of_elements, size_t size_of_elements);
}