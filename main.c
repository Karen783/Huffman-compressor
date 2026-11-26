#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>

// Línea para compilar: gcc main.c compressor.c -o main
// Línea para ejecutar: ./main

fr *calcularFrecuencias(int *total_out)
{
    int letra, total = 0, capacidad = 4;
    fr *array = malloc(sizeof(fr) * capacidad);

    if (array == NULL)
    {
        if (total_out != NULL)
            *total_out = 0;
        return NULL;
    }

    printf("Ingresa la oracion: ");
    while ((letra = getchar()) != '\n' && letra != EOF)
    {
        int pos = -1;
        for (int i = 0; i < total; i++)
        {
            if ((array + i)->letra == (char)letra)
            {
                pos = i;
                break;
            }
        }

        if (pos != -1)
        {
            (array + pos)->frecuencia++;
        }
        else
        {
            if (total == capacidad)
            {
                capacidad += 3; // Aumentar el tamaño en 3
                fr *temp = realloc(array, capacidad * sizeof(fr));
                if (temp == NULL)
                {
                    free(array);
                    if (total_out != NULL)
                        *total_out = 0;
                    return NULL;
                }
                array = temp;
            }
            (array + total)->letra = (char)letra;
            (array + total)->frecuencia = 1;
            total++;
        }
    }

    if (total_out != NULL)
    {
        *total_out = total;
    }

    return array;
}

void ordenarFrecuencias(fr *array, int total)
{
    for (int i = 0; i < total; i++)
    {
        int menor = i;
        for (int j = i + 1; j < total; j++)
        {
            if ((array + j)->frecuencia < (array + menor)->frecuencia)
            {
                menor = j;
            }
        }

        if (menor != i)
        {
            fr temp = *(array + i);
            *(array + i) = *(array + menor);
            *(array + menor) = temp;
        }
    }
}

int main()
{
    int total = 0;
    fr *tabla = calcularFrecuencias(&total);

    if (tabla == NULL && total > 0)
    {
        printf("Error al asignar memoria.\n");
        return -1;
    }

    ordenarFrecuencias(tabla, total);
    
    nodo *raiz = crear_arbol(tabla, total);
    printf("Recorrido preorden del arbol:\n");
    imprimir_arbol(raiz);
    printf("\n");

    liberar_arbol(raiz);
    free(tabla);
    return 0;
}