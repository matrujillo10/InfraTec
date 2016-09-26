/*
PROYECTO 1 FUNDAMENTOS DE INFRAESTRUCTURA TECNOLÓGICA
 */
#define _CRT_SECURE_NO_DEPRECATE
#include "stdlib.h"
#include "stdio.h"
#include <math.h>

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
		 *** En esta parte se determina si el bit más significativo del grupo
		 *** de 5 es 1 ó 0 para poder colocar el encabezado de manera adecuada.
		 ***/
		unsigned char val = arch->informacion[0];
		val >>= 7;
		if(val == 1) {
			/***
			 *** Se usa el 32, pues es 100000 en binario. Es decir,
			 *** el bit 6 es 1 y así el OR con este número hará que
			 *** se de un 1 en el tercer bit del encabezado, el
			 *** bit 7 del byte i de informacion.
			 ***/
			informacion[i] |= 32;
		} else {
			/***
			 *** Se usa el 64, pues es 1000000 en binario. Es decir,
			 *** el bit 7 es 1 y así el OR con este número hará que
			 *** se de un 1 en el segundo bit del encabezado, el
			 *** bit 7 del byte i de informacion.
			 ***/
			informacion[i] |= 64;
		}

		unsigned char temporal;
		unsigned char temporal2;
		for (j = 0; j < arch->tamanho; ++j) {
			/***
			 *** Se desplazan 5 bits a la izquierda para eliminar los que ya
			 *** fueron procesados y guardados en información y dejar sólo los
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

/*
 *  Sólo se usa para debugear
 */
// TODO ¡¡¡¡¡¡BORRAR AL TERMINAR!!!!!!
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
}

/*
 * Procedimiento que recibe un byte con información en los 5 bits menos significativos, y realiza la codificación correspondiente en ASCII
 */
unsigned char codificar(unsigned char cinco)
{
	unsigned char rta = cinco;

	/***
	 *** Corre 3 bits a la izquierda para eliminar los bits sin información
	 *** y dejar en 0 los bits menos significativos, teniendo los datos entre
	 *** el bit 8 y el 4. Para quedar así: XXXXX000
	 ***/
	rta <<= 3;

	/***
	 *** Mueve 7 bits a la derecha los bits del byte cinco y guarda el valor
	 *** en la variable val. En teoría, debería quedar siete ceros a la
	 *** izquierda y el bit menos significativo representa el bit más significativo
	 *** de la información que trae cinco. Para quedar así: 0000000X
	 ***/
	unsigned char val = rta >> 7;

	/***
	 *** Se corre 3 bits a la derecha. Para quedar así: 000XXXXX
	 ***/
	rta >>= 3;

	/***
	 *** Si el valor de la variable val es 1, significa que el bit más significativo
	 *** de la información es 1. De lo contrario, será 0.
	 ***/
	if(val == 1) {
		/***
		 *** Se usa el 32, pues es 100000 en binario. Es decir,
		 *** el bit 6 es 1 y así el OR con este número hará que
		 *** se de un 1 en el tercer bit del encabezado.
		 ***/
		rta |= 32;
	} else {
		/***
		 *** Se usa el 64, pues es 1000000 en binario. Es decir,
		 *** el bit 7 es 1 y así el OR con este número hará que
		 *** se de un 1 en el segundo bit del encabezado.
		 ***/
		rta |= 64;
	}
	/*** Se retorna el byte cinco codificado ***/
	return rta;
}

/*
 * Procedimiento que convierte la información en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
 * como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuación de los otros
 */
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado)
{
	/*** Variables de los recorridos ***/
	int i;
	int j;

	/*** Se define el tamaño del nuevo arreglo de chars ***/
	int tamanho = (int) (ceil((data->tamanho*5)/8)+1);

	/*** Se quita el encabezado de cada byte. Queda así: XXXXX000 ***/
	for (i = 0; i < data->tamanho; ++i) {
		data->informacion[i] <<= 3;
	}

	unsigned char temporal;
	for (i = 0; i < data->tamanho; ++i) {
		for (j = i; j < data->tamanho; ++j) {
			/***
			 *** Deja en temporal los 3 bits más significativos del
			 *** byte j+1. Queda así: 00000YYY
			 ***/
			temporal = data->informacion[j+1] >> 5;
			/***
			 *** Realiza OR con el byte en la posición j y temporal.
			 *** XXXXX000 V 00000YYY = XXXXXYYY
			 ***/
			data->informacion[j] |= temporal;

			/***
			 *** Se aplica esta condición, pues el byte i en este punto
			 *** ya se encuentra decodificado, mientras los que le siguen
			 *** no lo están
			 ***/
			if (j > i) {
				/***
				 *** Cuando entra en este condicional, el byte sigue sin
				 *** ser el decodificado. Se entiende el byte debió dar sus
				 *** 3 bits más significativos al byte que le precede. Por
				 *** tanto, en este punto se le están quitando los 3 bits más
				 *** significativos para abrir espacio en los 3 bits menos
				 *** significativo y así permitir que en la próxima entrada en
				 *** el bucle, el byte pueda introducir los bits que le faltan.
				 *** Queda así XXYYY000.
				 ***/
				data->informacion[j] <<= 3;
			}
		}
	}

	/*** Se pasa la información codificada al archivo resultado ***/
	resultado->tamanho = tamanho;
	for (i = 0; i < tamanho; ++i) {
		resultado->informacion[i] = data->informacion[i];
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
