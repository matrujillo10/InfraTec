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
int main(int argc, char* argv[]) {
    //Se reserva espacio para las variables ARCHIVO en las cuales se guarda el archivo de entrada y el archivo de salida
    ARCHIVO *arch = (ARCHIVO*)malloc(sizeof (ARCHIVO));//Archivo de entrada
    ARCHIVO *resultado = (ARCHIVO *)malloc(sizeof (ARCHIVO));//Archivo de salida

	int op;    
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
    
    op = convertir(arg);
    
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
	int ans;
    __asm {
        mov ebx, p[0]
        cmp [ebx], ['-']
        je Equals
            mov esi, 0
            jmp end
		Equals:
        cmp [ebx+1], ['a']
        jne notEqualsA
            mov esi, 1
            jmp end
        notEqualsA:
        cmp [ebx+1], ['b']
        jne notEqualsB
            mov esi, 2
            jmp end
        notEqualsB:
        mov esi, 0
        end:
        mov ans, esi
    }
	return ans;
}

/**
 * Procedimiento que convierte la información binaria de la estructura ARCHIVO apuntada por 'arch' (archivo original)
 * en texto (codificacion en ASCII) y lo guarda en la estructura ARCHIVO apuntada por 'resultado'.
 */
void conversionTexto(ARCHIVO *arch, ARCHIVO *resultado) {
    __asm {
		mov ebx, arch
		mov edx, resultado
        mov esi, 0
		mov ecx, [edx+4]
        Bwhile:	cmp esi, [edx]
        jge Ewhile                      ; i < resultado->tamanho

            push ebx                    ;----------------------------------------------------------------------
            push edx                    ; Salvando los registros
            push esi                    ;----------------------------------------------------------------------
			push ecx

            push esi                    ; Parametros  sacar5bits - i
            push arch                   ; Parametros  sacar5bits - arch
            call sacar5bits             ; eax se usa para traer el valor retornado
			add esp, 8

            push eax                    ; Parametros codificar
            call codificar              ; eax se usa para traer el valor retornado
			add esp, 4

			pop ecx
            pop esi                     ;----------------------------------------------------------------------
            pop edx                     ; Recuperando los registros
            pop ebx                     ;----------------------------------------------------------------------

			cmp esi, 0
			je nonzero
				add ecx, 1
			nonzero:
            mov [ecx], al
            inc esi
        jmp Bwhile
        Ewhile:
    }
}

/**
* Procedimiento que extrae el n-ésimo grupo de 5 bits de la entrada (estructura ARCHIVO apuntada por 'arch') y 
* retorna un char que los contiene * en sus 5 bits menos significativos (los restantes bits deben estar en cero)
*/
unsigned char sacar5bits(ARCHIVO *arch, int n) {
	int numL, numB;
    __asm {
        mov edi, arch
        mov ecx, n
        mov eax, n
		mov ebx, 8
        imul eax, 5
		mov edx, 0
        idiv ebx
        mov numL, eax
        mov numB, edx

		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0
		
        mov edx, [edi+4]
        add edx, numL
        mov al, [edx]				; letra
        cmp numB, 4
        jge Else1
			mov ecx, 0
            mov cl, 3
            sub ecx, numB
			shr al, cl
            and al, 0x1F ;31
            jmp fin
        Else1:
			mov ebx, [edi]
            dec ebx
            cmp ebx, numL
            jne Else2
				mov ecx, 0
				mov cl, -3
				add ecx, numB
				shl al, cl
				and al, 0x1F ;31
                jmp fin
            Else2:
					mov ecx, 0
					mov cl, -3
					add ecx, numB
					shl al, cl
					and al, 0x1F ;31
					mov ebx, 0
                    mov bl, [edx+1]
					mov ecx, 0
                    mov cl, 11
                    sub ecx, numB
					shr bl, cl
                    or al, bl
			fin:
    }
}

/*
* Procedimiento que introduce los 5 bits menos significativos del byte 'bits' en el n-�simo grupo de 5 bits de la estructura ARCHIVO apuntada por 'arch'
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION. NO SE PERMITE USAR NOMBRES SIMBOLICOS
void meter5bits(ARCHIVO *arch, int n, unsigned char bits) {
	__asm {
		push ebp
		mov ebp, esp
		/* mov ebx, [ebp + 8]				;se mueve *arch a ebx
		mov eax, [ebp + 12]				;se mueve n a eax
		mov ecx, [ebp + 16]				;se mueve bits a cl */
		mov eax, [ebp + 12]
		mov ebx, 5
		mul ebx
		mov ebx, 8
		div ebx							;eax = numL ;edx = numB
		push eax						;numL
		push edx						;numB
		/*mov numL, eax					;numL = N*5/8
		mov numB, edx					;numB = (N*5)%8*/
		mov edi, [ebp + 8]				;edi = arch
		add eax, 4
		mov esi, [edi + eax]			;informacion + numL
		mov dl, [esi]					;*pnt = arch->informacion + numL
		push edx
		mov dh, dl						;letra = *pnt
		mov ebx, [ebp + 16]				;bits (4 bytes)
		and bl, 1FH						;bits = 0x1f & bits
		mov bh, bl						;tmp = bits
		mov ecx, [ebp - 8]				;numB
		cmp ecx, 4						;numB < 4?
        jge PrimerElse
            neg ecx							; - numB
            add ecx, 3						;3 + (- numB) = 3 - numB
            loop correr						;--------------------------------
            jmp BreakCorrer
            correr:							; bits << (3 - numB)	
                shl bl, 1
            BreakCorrer:					;--------------------------------
                or dh, bl					;letra|(bits << (3 - numB)
                mov dl, dh
            jmp fin
            /*and cl, 1FH						
            movsx tmp, cl
            cmp edx, 4
            mov eax, 3
            sub eax, numB
            sal cl, eax
            or letra, cl*/
		PrimerElse:
			mov eax, [ebp - 4]			;numL
			mov esi, [ebp + 8]			;pnt
			mov esi, [esi]				;tamanho
			dec esi						;tamanho - 1
			cmp eax, esi				;numL == (tamanho - 1)?
				jne ElseDentroDeElse
                    mov ecx, [ebp - 8]
                    sub ecx, 3
                    shr bl, cl					;bits >> (numB-3)
                    or dh, bl					;letra|(bits>>(numB-3))
                    mov dl, dh
                    jmp fin
				ElseDentroDeElse:
					mov ecx, [ebp - 8]
					sub ecx, 3
						shr bh, cl					;tmp >> (numB-3)
						or dh, bh					;letra|(tmp>>(numB-3))
						mov dl, dh					;No dude
						mov eax, [ebp - 12]
						inc eax
						mov eax, [eax]
						mov ecx, [ebp - 8]
						neg ecx
						add ecx, 11
						shl bl, cl
						or al, bl
						mov esi,eax
		fin:
			pop edx
			pop edx
			pop eax
			pop ebp
			mov eax, esi
	}
}

/*
* Procedimiento que recibe un byte con informaci�n en los 5 bits menos significativos, y realiza la codificaci�n correspondiente en ASCII
*/
unsigned char codificar(unsigned char cinco) {
    __asm {
		mov ebx, 0
		mov eax, 0
        mov bl, cinco
        cmp ebx, 0
        jl Else
            cmp ebx, 0x0f ;15
            jg Else
                mov al, 0x40 ;64
                or al, bl
                jmp fin
        Else:
            mov al, 0x20 ;32
            or al, bl
        fin:
    }
}

/*
* Procedimiento que convierte la informaci�n en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
* como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuaci�n de los otros
*/
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado) {
    __asm {
        mov ebx, data
        mov ecx, resultado
		mov edi, [ebx+4]
        mov esi, 0
        Bwhile: cmp esi, [ebx]
        jge Ewhile
            add edi, esi
            mov edx, [edi]				; bits edx

			push edi
            push ebx
            push ecx
            push esi

            push edx
            push esi
            push ecx
            call meter5bits
			add esp, 12

            pop esi
            pop ecx
            pop ebx
			pop edi
            inc esi
        jmp Bwhile
        Ewhile:
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