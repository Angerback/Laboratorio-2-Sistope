# README #

### Laboratorio 2 SISTOPE Usach. Hebras y concurrencia ###

* Laboratorio escrito en C para realizar competencia entre equipos de hebras que intersectan listas de numeros.
* Version 1.0
* Este programa usa [Pthread](https://computing.llnl.gov/tutorials/pthreads/)
* Autores: Sebastián Meneses y Matías Calderon

### Instrucciones ###

* Ir al directorio raíz del proyecto (la carpeta donde se encuentra este README).
* En una terminal linux, ejecutar el comando

```
#!c

make
```

* Para ejecutar el programa, dirigirse a la carpeta "build" recién creada:


```
#!bash

cd ./build
```

* Colocar un archivo de lista en esta carpeta, conforme al formato especificado en el enunciado.
* Ejecutar el archivo "competencia":


```
#!bash

./competencia -g 4 -h 4 -i lista.dat
```

El comando anterior ejecutará el programa con 4 equipos de 4 hebras para el archivo "lista.dat"

Si desea ejecutar otro comando, variando el numero de equipos o hebras, siga el siguiente esquema:

```
#!bash

./competencia -g NG -h NH -i Ruta
```

donde:

      NG: número de grupos 
      NH: número de hebras por grupo
      Ruta: ruta relativa del archivo .dat 


### Detalles encontrados ###

1- Si el archivo .dat no contiene un salto de linea final, el programa se queda pegado en un bucle while en la linea 92.

2- Número de hebras no puede ser mayor al tamaño de la lista mas corta, de lo contrario, el programa devuelve como respuesta una intersección vacía.

3- Si en el archivo .dat no se encuentra un número para interceptar, genera una lista vacía.