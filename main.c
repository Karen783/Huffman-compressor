#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>

// Línea para compilar: gcc main.c compressor.c -o main
// Línea para ejecutar: ./main

int main()
{
    int total = 0;
    fr *tabla = calcularFrecuencias(&total);

    if (tabla == NULL && total > 0)
    {
        fprintf(stderr, "Error al asignar memoria.\n");
        return 1;
    }

    for (int i = 0; i < total; i++)
    {
        printf("%c: %d\n", tabla[i].letra, tabla[i].frecuencia);
    }

    free(tabla);
    return 0;
}
