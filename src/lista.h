#include <stdlib.h>

typedef struct lista{
  int * dato;
  int largo;
  int cursor;
}lista;

lista crear_lista(int largo){
    int * li = ( int * ) malloc(sizeof( int ) * largo);
    lista l;
    l.dato = li;
    l.largo = largo;
    l.cursor = 0;
    return l;
}

void add_lista(lista * l, int num){
    (l->dato)[l->cursor] = num;
    l->cursor = (l->cursor)++;
    return;
}