/*
PROYECTO 1 FUNDAMENTOS DE INFRAESTRUCTURA TECNOL�GICA
*/
#define _CRT_SECURE_NO_DEPRECATE
#include "stdlib.h" 
#include "stdio.h"

/**
 * Miller Andrés Trujillo - 201517402
 */

typedef struct archivo
{
	int tamanho;
	unsigned char *informacion;
} ARCHIVO;

void cargarArchivo(ARCHIVO *info, char *nomArchivoEntrada);
void guardarArchivo(ARCHIVO *data, char *nomArchivoSalida);
int convertir ( char * p );
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

	char * arg;
	if (argc != 4)
	{
		printf("Faltan argumentos - Debe haber dos archivos y un parametro de tipo\n");
		system("pause");
		return -1;
	}

	printf("Archivo de entrada %s\n", argv[1]);
	printf("Archivo de salida %s\n", argv[2]);

	//Se cargan los datos del archivo de entrada en la variable arch de tipo ARCHIVO
	cargarArchivo(arch, argv[1]);



	//printf("Indique la accion\n\t1) Codificar en ASCII:\n\t2) Decodificar:\n\n");//Se pregunta al usuario cu�l opci�n desea realizar

    arg = argv[3];
    
    int op = convertir(arg);
    
	if (op == 1)//Convertir a ASCII
	{

		//Se modifican los atributos del ARCHIVO resultado
		resultado->tamanho = (((arch->tamanho) * 8)+4) / 5;
		resultado->informacion = (unsigned char*)calloc(resultado->tamanho, 1);
		//Se convierte el ARCHIVO arch a texto y se guarda en el ARCHIVO resultado
		conversionTexto(arch, resultado);

		//Se guarda el archivo de texto en el destino dado como par�metro
		guardarArchivo(resultado, argv[2]);
	}
	else if (op == 2)//Convertir a binario
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
 *Procedimiento que convierte el argumento de entrada a la opci�n adecuada para saber si convertir a ASCII o a binario
 */
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
int convertir ( char * p )
{
    //no cumple con el formato
    if(*p!='-'){
        return 0;
    }
    p++;
    //convertir a ASCII
    if(*p == 'a')
    {
        return 1;
    }
    //convertir a binario
    else if(*p == 'b')
    {
        return 2;
    }
    //no cumple con el formato
    else
    {
        return 0;
    }
}

/*
* Procedimiento que convierte la informaci�n binaria de la estructura ARCHIVO apuntada por 'arch' (archivo original) en texto (codificacion en ASCII)
* y lo guarda en la estructura ARCHIVO apuntada por 'resultado'.
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void conversionTexto(ARCHIVO *arch, ARCHIVO *resultado)
{
	int i=0;
	while (i < (resultado->tamanho))
	{
		resultado->informacion[i] = codificar(sacar5bits(arch, i));
		
		i++;
	}
}

/*
* Procedimiento que extrae el n-�simo grupo de 5 bits de la entrada (estructura ARCHIVO apuntada por 'arch') y retorna un char que los contiene en sus 5 bits menos significativos
* (los restantes bits deben estar en cero)
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
unsigned char sacar5bits(ARCHIVO *arch, int n)
{
	int numL = n * 5 / 8;
	int numB = (n * 5) % 8;
	unsigned char resp, tmp;
	unsigned char letra = arch->informacion[numL];
	if (numB<4)
	{
		resp = (0x1F)&(letra >> (3 - numB));
	}
	else
	{
		if (numL == (arch->tamanho - 1))
		{
			resp = (0x1f)&(letra << (4 - (7 - numB)));
		}
		else
		{
			resp = (0x1F)&(letra << (4 - (7 - numB)));
			letra = arch->informacion[numL+1];
			tmp = letra >> (8 - (5 - (8 - numB)));
			resp = resp | tmp;
		}
	}
	return resp;
}

/*
* Procedimiento que introduce los 5 bits menos significativos del byte 'bits' en el n-�simo grupo de 5 bits de la estructura ARCHIVO apuntada por 'arch'
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void meter5bits(ARCHIVO *arch, int n, unsigned char bits)
{
	int numL = n * 5 / 8;
	int numB = (n * 5) % 8;
	unsigned char * pnt=arch->informacion+numL;
	unsigned char letra = *pnt;
	unsigned char tmp;
	bits = 0x1f & bits;
	tmp = bits;
	if (numB<4)
	{
		*pnt = letra|(bits << (3 - numB));
	}
	else
	{
		if (numL == (arch->tamanho - 1))
		{
			*pnt = letra|(bits>>(numB-3));
		}
		else
		{
			*pnt = letra | (tmp >> (numB - 3));
			*(pnt + 1) = *(pnt + 1)|bits << (8 - (5 - (8 - numB)));
		}
	}
}

/*
* Procedimiento que recibe un byte con informaci�n en los 5 bits menos significativos, y realiza la codificaci�n correspondiente en ASCII
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
unsigned char codificar(unsigned char cinco)
{
	char resp;
	if (cinco >= 0x00 && cinco<=0x0f)
	{
		resp = 0x40 | cinco;
	}
	else
	{
		resp = 0x20 | cinco;
	}
	return resp;
}

/*
* Procedimiento que convierte la informaci�n en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
* como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuaci�n de los otros
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado)
{
	int i = 0;
	unsigned char bits;
	while (i<data->tamanho)
	{
		bits = data->informacion[i];
		meter5bits(resultado, i, bits);
		i++;
	}
}

/*
* Procedimiento para cargar un archivo de disco
*/
//NO MODIFICAR
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
//NO MODIFICAR
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


