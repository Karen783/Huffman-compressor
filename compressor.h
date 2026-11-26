#ifndef COMPRESSOR_H_
#define COMPRESSOR_H_

typedef struct pq_nodo nodo;

typedef struct min_heap heap;

typedef struct frecuencias
{
    int frecuencia;
    char letra;
} fr;

typedef struct diccionario dic;

int recorrer(nodo *n, char *codigo, int profundidad, dic *tabla, int cuenta);

nodo *crear_hoja(char letra, int frecuencia);

nodo *crear_padre(nodo *izq, nodo *der);

nodo *crear_arbol(fr *tabla, int total);

heap *crear_heap(int capacidad);

int insertar(heap *h, nodo *n);

void imprimir_heap(heap *h);

nodo *extraer_minimo(heap *h);

void imprimir_arbol(nodo *n);

void liberar_arbol(nodo *n);

#endif
