#include <stdlib.h>

typedef struct lista{
  int * dato;
  int largo;
  int cursor;
}lista;

lista * crear_lista(int largo){
    int * li = ( int * ) malloc(sizeof( int ) * largo);
    lista * l = ( lista * ) malloc(sizeof (struct lista));;
    l->dato = li;
    l->largo = largo;
    l->cursor = 0;
    return l;
}

void add_lista(lista * l, int num){
    int existe = 0;
    int i = 0;
    
    for (i = 0; i < l->largo; i++) {
        if((l->dato)[i] == num){
            existe = 1;
            break;
        }
    }

    if(!existe){
        if(l->cursor == l->largo){
            //Se debe pedir mas espacio
            l->largo = l->largo + 1;
            l -> dato = (int *) realloc((l->dato), sizeof(int) * (l->largo));
        }
        (l->dato)[l->cursor] = num;
        l->cursor = (l->cursor) + 1;
    }
    return;
}
/*
void agrandar_lista(lista * l, int nuevo_tamano){
    l -> largo = nuevo_tamano;
    l -> dato = (int *) realloc((l->dato), sizeof(int) * nuevo_tamano);
}*/
