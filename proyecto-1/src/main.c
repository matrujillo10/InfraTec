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
	int i;
	unsigned char info[(int) ceil((arch->tamanho*8)/5)];
	for (i = 0; i < (int) ceil((arch->tamanho*8)/5); ++i) {
		info[i] = arch->informacion[0];
		info[i] >>= 3;
		char val = arch->informacion[0];
		(val >>= 7) == 1 ? info[i] | 32: info[i] | 64;
		*arch->informacion <<= 5;
	}
	resultado->informacion = info;
	resultado->tamanho = (int) sizeof(info)/sizeof(info[0]);
	// Por probar
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
}

/*
 * Procedimiento que recibe un byte con información en los 5 bits menos significativos, y realiza la codificación correspondiente en ASCII
 */
unsigned char codificar(unsigned char cinco)
{
	//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
}

/*
 * Procedimiento que convierte la informaci�n en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
 * como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuaci�n de los otros
 */
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado)
{
	//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
}

/*
 * Procedimiento para cargar un archivo de disco
 */
void cargarArchivo(ARCHIVO *data, char *nomArchivoEntrada)
{
	FILE *streamArchivo;
	int tam;
	unsigned char * aux;
	int i;

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
