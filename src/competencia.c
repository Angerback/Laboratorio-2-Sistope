#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "lista.h"

/*
    Función que abre y lee un archivo con el formato dado en el enunciado.
    Recbie la ruta del archivo y retorna un arreglo con las listas extraídas
    del archivo.
*/
struct lista * abrir_archivo(char * ruta){
    FILE * archivo;
    char * buffer;
    char cursor;
    int i= 0;
    archivo = fopen(ruta, "r");
    int largo_lista = 0, numero = 0, n_listas = 0;
    if(archivo){
        cursor = getc(archivo);
        while(cursor != EOF){
            n_listas++;
            //Codigo para archivo valido
            buffer = (char *) malloc(sizeof(char));

            //Se lee elemento por elemento (es decir, cada espacio hay un elemento)
            //cursor = getc(archivo);
            while(cursor!=' '){
                buffer[i] = cursor;
                cursor = getc(archivo);
                    if(cursor != ' '){
                      i++;
                      buffer = realloc(buffer, (i+1)*sizeof(char));
                    }
            }
            //Una vez sacado el primer término, este se convierte a un entero
            largo_lista = atoi(buffer);
            printf("- Largo lista %d: %d\n", n_listas, largo_lista);
            /*
            Obtenido el largo de la lista, los siguientes valores son numeros de
            la lista
            */
            cursor = getc(archivo);
            lista list = crear_lista(largo_lista);
            while(cursor != '\n'){
                i = 0;
                free(buffer);
                buffer = (char *) malloc(sizeof(char));
                while(cursor!=' '){
                    if(cursor == '\n'){
                        break;
                    }
                    buffer[i] = cursor;
                    cursor = getc(archivo);
                    if(cursor != ' ' && cursor != '\n'){
                      i++;
                      buffer = realloc(buffer, (i+1)*sizeof(char));
                    }
                }

                // Luego de que se tiene un numero, se pasa a int y se agrega a la lista
                int num = atoi(buffer);
                printf("- Elemento de la lista: %d\n", num);
                add_lista(&list, num);
                // se itera para todos los numeros.

                if(cursor == '\n'){
                    cursor = getc(archivo);
                    break;
                }
                cursor = getc(archivo);
            }
            
        }
    }

    return 0;
}

int main(int argc, char * argv[]){

    int n_equipos = 0, n_hebras = 0;
    char * ruta = 0; //Relativa

    int arg_index = 1;

    while(arg_index < argc - 1){
        if(!strcmp(argv[arg_index], "-g")){ //Cantidad de equipos
            n_equipos = atoi(argv[arg_index + 1]);
        }
        else if(!strcmp(argv[arg_index], "-h")){ // Hebras por equipo
            n_hebras = atoi(argv[arg_index + 1]);
        }
        else if(!strcmp(argv[arg_index], "-i")){ // Ruta del archivo de entrada
            ruta = argv[arg_index + 1];
        }
        // Se aumenta de a dos ya que siempre se recibe el identificador del
        // parametro y luego el valor.
        arg_index += 2;
    }
    printf("Equipos: %d, Hebras: %d, Ruta: %s\n", n_equipos, n_hebras, ruta);

    // Luego de que se tienen los parametros, se debe buscar el archivo en
    // memoria y obtener las listas.

    lista * listas = abrir_archivo(ruta);

    return 0;
}
