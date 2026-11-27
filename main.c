#include "compressor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Línea para compilar: gcc main.c compressor.c -o main
// Línea para ejecutar: ./main

// size_t: tipo de entero sin signo para representar tamaños y cantidades de bytes
char *leer_entrada(void)
{
    size_t capacidad = 16;
    size_t longitud = 0;
    char *buffer = malloc(capacidad);
    if (buffer == NULL)
        return NULL;

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (longitud + 1 >= capacidad) // deja espacio para el '\0'
        {
            capacidad *= 2;
            char *temp = realloc(buffer, capacidad);
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
        *(buffer + longitud) = (char)c;
        longitud++;
    }
    *(buffer + longitud) = '\0';

    // Recorte: ajustar el bloque al tamaño exacto (caracteres + '\0')
    char *ajustado = realloc(buffer, longitud + 1);
    if (ajustado != NULL)
        buffer = ajustado;
        
    return buffer;
}

fr *crear_tabla_frecuencias(const char *oracion, int *total_out)
{
    int total = 0, capacidad = 4;
    fr *array = malloc(sizeof(fr) * capacidad);

    if (array == NULL)
    {
        if (total_out != NULL)
            *total_out = 0;
        return NULL;
    }

    for (int i = 0; *(oracion + i) != '\0'; i++)
    {
        char letra = *(oracion + i);

        int pos = -1;
        for (int j = 0; j < total; j++)
        {
            if ((array + j)->letra == letra)
            {
                pos = j;
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
                capacidad += 3;
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
            (array + total)->letra = letra;
            (array + total)->frecuencia = 1;
            (array + total)->codigo_binario = NULL;
            total++;
        }
    }

    if (total_out != NULL)
        *total_out = total;
    return array;
}

void ordenar_frecuencias(fr *array, int total)
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
    // ADAPTAR A TU RUTA 
    const char* ruta_comprimido = "Tu ruta.bin";
    const char* ruta_descomprimido = "Tu ruta.txt";

    printf("\nIngresa la oracion: ");
    const char *oracion = leer_entrada();
    if (oracion == NULL)
    {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    int total = 0;
    fr *tabla = crear_tabla_frecuencias(oracion, &total);

    if (tabla == NULL && total > 0)
    {
        printf("Error al asignar memoria.\n");
        free((char *)oracion);
        return -1;
    }

    printf("\n---------- Tabla de frecuencias desordenadas ----------\n");
    printf("\n");
    imprimir_tabla(tabla, total);

    printf("\n---------- Tabla de frecuencias ordenadas ----------\n");
    printf("\n");
    ordenar_frecuencias(tabla, total);
    imprimir_tabla(tabla, total);

    nodo *raiz = crear_arbol(tabla, total);

    printf("\n---------- Arbol de Huffman ----------\n");
    printf("\n");
    imprimir_arbol(raiz);

    generar_codigos(raiz, tabla, total);
    printf("\n---------- Generacion de codigos binarios ----------\n");
    printf("\n");
    imprimir_tabla(tabla, total);

    generar_archivo_binario(ruta_comprimido, oracion, tabla, total);
    printf("\n---------- Generacion de cadena binaria ----------\n");
    printf("\n");
    imprimir_archivo_binario(ruta_comprimido);
    printf("\n");

    descomprimir_archivo(ruta_comprimido, ruta_descomprimido, raiz);

    liberar_arbol(raiz);
    liberar_tabla(tabla, total);
    free((char *)oracion);

    return 0;
}