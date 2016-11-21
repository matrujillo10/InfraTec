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

//NO MODIFICAR
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

/**
 * Procedimiento que convierte el argumento de entrada a la opción adecuada para saber si convertir a ASCII o a binario
 */
int convertir (char * p) {
    __asm {
        .code
            convertir proc
                push ebp
                mov ebp, esp
                mov ecx, [ebp+8]                ; ecx guarda el parametro
                mov ebx, 45                     ; ebx se igual al valor númerico del caracter ascii '-'
                cmp ebx, ecx                    ; se compara con el valor almacenado en ecx
                jne notEquals
                    mov esi, 0
                    jmp end
                notEquals:
                mov ebx, 97
                add ecx, 1
                cmp ebx, ecx
                jne notEqualsA
                    mov esi, 1
                    jmp end
                notEqualsA:
                mov ebx, 98
                cmp ebx, ecx
                jne notEqualsB
                    mov esi, 2
                    jmp end
                notEqualsB:
                mov esi, 0
                end:
                mov eax, esi
                pop ebp
                ret 4
            convertir endp
    }
}

/*
 * Procedimiento que convierte la informaci�n binaria de la estructura ARCHIVO apuntada por 'arch' (archivo original) en texto (codificacion en ASCII)
* y lo guarda en la estructura ARCHIVO apuntada por 'resultado'.
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. SE PERMITE USAR NOMBRES SIMBOLICOS
void conversionTexto(ARCHIVO *arch, ARCHIVO *resultado) {
    __asm {
        ; Se usan los registros ebx, ecx, esi y edi. El registro eax se usa para guardar los valores retornados por los métodos.
        .code
            conversionTexto proc
                push ebp
                mov ebp, esp
                mov ebx, [ebp+8]                ; ebx guarda el inicio de la estructura arch. Es decir, el tamaño.
                mov edx, [ebp+12]               ; edx guarda el inicio de la estructura resultado. Es decir, el tamaño.
                mov ecx, [edx+4]
                mov esi, 0
                Bwhile:	cmp esi, [edx]
                jge Ewhile                      ; i < resultado->tamanho
                    
                    push ebx                    ;----------------------------------------------------------------------
                    push edx                    ;
                    push ecx                    ; Salvando los registros
                    push esi                    ;----------------------------------------------------------------------
                        
                    push esi                    ; Parametros  sacar5bits
                    push [ebx]                  ; Parametros  sacar5bits
                    call sacar5bits             ; eax se usa para traer el valor retornado
                    push eax                    ; Parametros codificar
                        
                    call codificar              ; eax se usa para traer el valor retornado
                        
                    pop esi                     ;----------------------------------------------------------------------
                    pop ecx                     ;
                    pop edx                     ; Recuperando los registros
                    pop ebx                     ;----------------------------------------------------------------------
                
                    mov [edx+4], eax
                    inc esi
                jmp Bwhile
                Ewhile:
                pop ebp
                ret 8
            conversionTexto endp
    }
    resultado->informacion[i] = codificar(sacar5bits(arch, i));
}

/*
* Procedimiento que extrae el n-�simo grupo de 5 bits de la entrada (estructura ARCHIVO apuntada por 'arch') y retorna un char que los contiene en sus 5 bits menos significativos
* (los restantes bits deben estar en cero)
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. SE PERMITE USAR NOMBRES SIMBOLICOS
unsigned char sacar5bits(ARCHIVO *arch, int n)
{
	
}

/*
* Procedimiento que introduce los 5 bits menos significativos del byte 'bits' en el n-�simo grupo de 5 bits de la estructura ARCHIVO apuntada por 'arch'
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. NO SE PERMITE USAR NOMBRES SIMBOLICOS
void meter5bits(ARCHIVO *arch, int n, unsigned char bits)
{
	
}

/*
* Procedimiento que recibe un byte con informaci�n en los 5 bits menos significativos, y realiza la codificaci�n correspondiente en ASCII
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. SE PERMITE USAR NOMBRES SIMBOLICOS
unsigned char codificar(unsigned char cinco)
{
	
}

/*
* Procedimiento que convierte la informaci�n en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
* como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuaci�n de los otros
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. SE PERMITE USAR NOMBRES SIMBOLICOS
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado)
{
	
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