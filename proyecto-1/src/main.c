/*
PROYECTO 1 FUNDAMENTOS DE INFRAESTRUCTURA TECNOLÓGICA
 */
#define _CRT_SECURE_NO_DEPRECATE
#include "stdlib.h"
#include "stdio.h"
#include <math.h>

//AQUI VAN LOS NOMBRES Y CODIGOS DE LOS INTEGRANTES DEL GRUPO
/**
 * Miller Andrés Trujillo - 201517402
 */

typedef struct archivo
{
	int tamanho;
	unsigned char *informacion;
}
ARCHIVO;

void cargarArchivo(ARCHIVO *info, char *nomArchivoEntrada);
void guardarArchivo(ARCHIVO *data, char *nomArchivoSalida);
void conversionTexto(ARCHIVO *data, ARCHIVO *resultado);
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado);
unsigned char codificar(unsigned char cinco);
unsigned char sacar5bits(ARCHIVO *arch, int n);
void meter5bits(ARCHIVO *arch, int n, unsigned char bits);
void impresion(unsigned char var);

int main(int argc, char* argv[])
{
	//Se reserva espacio para las variables ARCHIVO en las cuales se guarda el archivo de entrada y el archivo de salida
	ARCHIVO *arch = (ARCHIVO*)malloc(sizeof (ARCHIVO));//Archivo de entrada
	ARCHIVO *resultado = (ARCHIVO *)malloc(sizeof (ARCHIVO));//Archivo de salida

	char op;
	if (argc != 3)
	{
		printf("Faltan argumentos - Debe haber dos archivos: entrada y salida\n");
		system("pause");
		return -1;
	}

	printf("Archivo de entrada %s\n", argv[1]);
	printf("Archivo de salida %s\n", argv[2]);

	//Se cargan los datos del archivo de entrada en la variable arch de tipo ARCHIVO
	cargarArchivo(arch, argv[1]);



	printf("Indique la accion\n\t1) Codificar en ASCII:\n\t2) Decodificar:\n\n");//Se pregunta al usuario cu�l opci�n desea realizar

	scanf("%c", &op);

	if (op == '1')//Convertir a ASCII
	{

		//Se modifican los atributos del ARCHIVO resultado
		resultado->tamanho = (((arch->tamanho) * 8)+4) / 5;
		resultado->informacion = (unsigned char*)calloc(resultado->tamanho, 1);
		//Se convierte el ARCHIVO arch a texto y se guarda en el ARCHIVO resultado
		conversionTexto(arch, resultado);

		//Se guarda el archivo de texto en el destino dado como par�metro
		guardarArchivo(resultado, argv[2]);
	}
	else if (op == '2')//Convertir a binario
	{

		//Se modifican los atributos del ARCHIVO resultado
		resultado->tamanho = (((arch->tamanho) * 5) / 8);
		resultado->informacion = (unsigned char*)calloc(resultado->tamanho, 1);

		//Se convierte el ARCHIVO arch a binario y se guarda en el ARCHIVO resultado
		conversionBinario(arch, resultado);

		//Se guarda el archivo de texto en el destino dado como par�metro
		guardarArchivo(resultado, argv[2]);
	}
	else
	{
		printf("Opcion invalida\n");
	}

	system("pause");
	return 0;
}

/*
 * Procedimiento que convierte la información binaria de la estructura ARCHIVO apuntada por 'arch' (archivo original) en texto (codificacion en ASCII)
 * y lo guarda en la estructura ARCHIVO apuntada por 'resultado'.
 */
void conversionTexto(ARCHIVO *arch, ARCHIVO *resultado)
{
	/*** Variables de los recorridos ***/
	int i;
	int j;

	/*** Se define el tamaño del nuevo arreglo de chars ***/
	int tamanho = (int) (ceil((arch->tamanho*8)/5)+1);

	/*** Se inicializa del nuevo arreglo de chars ***/
	unsigned char informacion[tamanho];

	for (i = 0; i < tamanho; ++i) {

		/***
		 *** Se iguala el char de la posicion i de info con el char
		 *** en la primera posición de la información del archivo
		 ***/
		informacion[i] = arch->informacion[0];

		/***
		 *** Se hace el corrimiento de 3 birs a la derecha para poder
		 *** introducir el encabezado
		 ***/
		informacion[i] = informacion[i]>>3;

		/***
		 ***	En esta parte se determina si el bit más significativo del grupo
		 *** de 5 es 1 ó 0 para poder colocar el encabezado de manera adecuada.
		 ***/
		unsigned char val = arch->informacion[0];
		val >>= 7;
		if(val == 1) {
			/***
			 *** Se usa el 32, pues es 100000. Es decir, el bit 6
			 *** es 1 y así el or con este número hará que se de
			 *** un 1 en el tercer bit del encabezado
			 ***/
			informacion[i] |= 32;
		} else {
			/***
			 *** Se usa el 64, pues es 1000000. Es decir, el bit 7
			 *** es 1 y así el or con este número hará que se de
			 *** un 1 en el segundo bit del encabezado
			 ***/
			informacion[i] |= 64;
		}

		unsigned char temporal;
		unsigned char temporal2;
		for (j = 0; j < arch->tamanho; ++j) {
			/***
			 *** Se desplazan 5 bits a la izquierda para eliminar los que ya
			 ***	fueron procesados y guardados en información y dejar sólo los
			 *** pertenecientes al siguiente grupo de 5 que se almacenan en ese byte
			 ***/
			temporal = arch->informacion[j] << 5;

			/***
			 *** Se desplazan 3 bits a la derecha en el byte en la posición j+1
			 *** para dar el espacio de los 3 bits que estaban en el byte anterior y
			 *** 2 bits del byte que sigue al presente.
			 ***/
			temporal2 = arch->informacion[j+1] >> 3;

			/***
			 *** Se realiza el temporal or temporal2 y se almacena en el byte de la
			 *** posición j. Cabe resaltar que siempre se mueve el siguiente byte a
			 *** leer a la primera posición del arreglo de información del archivo
			 *** y se van completando los demás con 0. Así, si se tiene la necesidad
			 *** de usar bits de relleno, irán implicitos como si pertenecieran a la
			 *** información.
			 ***/
			arch->informacion[j] = temporal | temporal2;
		}
	}
	/*** Se pasa la información codificada al archivo resultado ***/
	resultado->tamanho = (int) sizeof(informacion)/sizeof(informacion[0]);
	for (i = 0; i < tamanho; ++i) {
		resultado->informacion[i] = informacion[i];
	}
}

void impresion(unsigned char var) {
	unsigned int contador, inicio = 128;
	for(contador = inicio; contador > 0; contador >>= 1) {
		if(contador & var) {
			printf("1");
		} else {
			printf("0");
		}
	}
	printf("\n");
}

/*
 * Procedimiento que extrae el n-ésimo grupo de 5 bits de la entrada (estructura ARCHIVO apuntada por 'arch') y retorna un char que los contiene en sus 5 bits menos significativos
 * (los restantes bits deben estar en cero)
 */
unsigned char sacar5bits(ARCHIVO *arch, int n)
{
	//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
}

/*
 * Procedimiento que introduce los 5 bits menos significativos del byte 'bits' en el n-ésimo grupo de 5 bits de la estructura ARCHIVO apuntada por 'arch'
 * sin afectar los restantes bits de la estructura
 */
void meter5bits(ARCHIVO *arch, int n, unsigned char bits)
{
	//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
	arch->informacion[n] >>= 5;
	arch->informacion[n] <<= 5;
	bits <<= 3;
	bits >>= 3;
	arch->informacion[n] |= bits;
}

/*
 * Procedimiento que recibe un byte con información en los 5 bits menos significativos, y realiza la codificación correspondiente en ASCII
 */
unsigned char codificar(unsigned char cinco)
{
	cinco <<= 3;
	unsigned char val = cinco;
	val >>= 7;
	cinco >>= 3;
	if(val == 1) {
		cinco |= 32;
	} else {
		cinco |= 64;
	}
	return cinco;
}

/*
 * Procedimiento que convierte la información en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
 * como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuación de los otros
 */
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado)
{
	//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
	int i;
	unsigned char temp;
	for (i = 0; i < data->tamanho; ++i) {
		data->informacion[i] <<= 3;
	}
}

/*
 * Procedimiento para cargar un archivo de disco
 */
void cargarArchivo(ARCHIVO *data, char *nomArchivoEntrada)
{
	FILE *streamArchivo;
	int tam;
	unsigned char * aux;

	if (!(streamArchivo = fopen(nomArchivoEntrada, "rb")))
	{
		printf("No se puede abrir el archivo: %s\n", nomArchivoEntrada);
		system("pause");
		exit(EXIT_FAILURE);
	}

	fseek(streamArchivo, 0, SEEK_END);
	tam = ftell(streamArchivo);
	fseek(streamArchivo, 0, SEEK_SET);

	aux = (unsigned char*)calloc(tam, 1);

	fread(aux, 1, tam, streamArchivo);

	data->tamanho = tam;
	data->informacion = aux;

	if (fclose(streamArchivo) != 0)
	{
		printf("Error cerrando el archivo");
	}
}

/*
 * Procedimiento para guardar un archivo en disco
 */
void guardarArchivo(ARCHIVO *data, char *nomArchivoSalida)
{
	FILE *streamArchivo;

	if (!(streamArchivo = fopen(nomArchivoSalida, "wb"))){
		printf("No se puede abrir el archivo: %s\n", nomArchivoSalida);
		system("pause");
		exit(EXIT_FAILURE);
	}

	fwrite((data->informacion), 1, (data->tamanho), streamArchivo);

	if (fclose(streamArchivo) != 0)
	{
		printf("Error cerrando el archivo");
	}
}
