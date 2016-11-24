#define _CRT_SECURE_NO_DEPRECATE
#include "stdlib.h"
#include "stdio.h"

/**
 * Miller Andrés Trujillo Achury      - 201517402
 * Julián Andrés Bermúdez Valderrama  - 201519648
 * Jeison Mateo Contreras Castellanos - 201514104
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
	// Registros usados: EBX, ESI
	int ans;
    __asm {
		mov ebx, p[0]				; Guarda en ebx la posicion del primer caracter del apuntador p
		cmp[ebx], ['-']				; Compara el primer caracter que hay en p, con el caracter 45 ('-') de la tabla ascii
        je Equals					; Si son diferentes, no es el formato esperado. Por tanto, se devolverá 0
            mov esi, 0				; Se mueve a esi el valor 0
            jmp end					; Se salta hasta el final del programa
		Equals:
        cmp [ebx+1], ['a']			; Compara el segundo caracter que hay en p, con el caracter 97 ('a') de la tabla ascii
        jne notEqualsA				; Si son iguales, ya se conoce la instrucción
            mov esi, 1				; Se mueve a esi el valor 1 para indicar que se convertira a ascii
            jmp end					; Se salta hasta el final del programa
        notEqualsA:
        cmp [ebx+1], ['b']			; Compara el segundo caracter que hay en p, con el caracter 97 ('a') de la tabla ascii
        jne notEqualsB				; Si son iguales, ya se conoce la instrucción
            mov esi, 2				; Se mueve a esi el valor 2 para indicar que se convertira a binario
            jmp end					; Se salta hasta el final del programa
        notEqualsB:
        mov esi, 0					; Si el programa llegó acá, significa que no se tenía el formato esperado. Por tanto, se devolverá 0
        end:
        mov ans, esi				; Se mueve el valor de esi a ans que es la variable que será retornada en el código de c
    }
	return ans;
}

/**
 * Procedimiento que convierte la información binaria de la estructura ARCHIVO apuntada por 'arch' (archivo original)
 * en texto (codificacion en ASCII) y lo guarda en la estructura ARCHIVO apuntada por 'resultado'.
 */
void conversionTexto(ARCHIVO *arch, ARCHIVO *resultado) {
	// Registros usados: EAX (AL), EBX, ECX, EDX, ESI
    __asm {
		mov ebx, arch					; Se mueve a ebx el apuntador del archivo arch
		mov edx, resultado				; Se mueve a edx el apuntador del archivo resultado
        mov esi, 0						; Se inicializa el contador en esi y en 0
		mov ecx, [edx+4]				; Se mueve a ecx el apuntador a la información del archivo resultado
        Bwhile:	cmp esi, [edx]			; Se compara el tamaño del archivo resultado con el contador
        jge Ewhile                      ; esi < resultado->tamanho
            push ebx
			push edx
			push esi
			push ecx					; Se salvan los registros que usa este método
            push esi                    ; Parametros  sacar5bits - n
            push arch                   ; Parametros  sacar5bits - arch
            call sacar5bits             ; Saca el grupo de 5 bits indicado por esi
			add esp, 8					; Se adelanta el apuntador de la pila 8, pues sacar5bits requiere 2 parametros
            push eax                    ; Parametros codificar - cinco
            call codificar              ; Codifica el valor devuelto por sacar5bits y lo retorna en eax
			add esp, 4					; Se adelanta el apuntador de la pila 4, pues codificar requiere 1 parametros
			pop ecx
			pop esi
			pop edx
			pop ebx						; Se recuperan los registros usados en el método
			cmp esi, 0					; Se compara esi con 0
			je zero						; esi != 0
				add ecx, 1				; Se añade una posición al apuntador de la información del archivo resultado
			zero:
            mov [ecx], al				; Se guarda en el apuntador de la información del archivo resultado el valor codificado
            inc esi						; Se incrementa en 1
        jmp Bwhile
        Ewhile:
    }
}

/**
* Procedimiento que extrae el n-ésimo grupo de 5 bits de la entrada (estructura ARCHIVO apuntada por 'arch') y 
* retorna un char que los contiene * en sus 5 bits menos significativos (los restantes bits deben estar en cero)
*/
unsigned char sacar5bits(ARCHIVO *arch, int n) {
	// Registros usados: EAX (AL), EBX (BL), ECX (CL), EDX, EDI
	int numL, numB;
    __asm {
        mov edi, arch				; Se mueve al registro edi el apuntador del archivo arch
        mov ecx, n					; Se mueve al registro ecx el valor de n
        mov eax, n					; Se mueve al registro eax el valor de n
		mov ebx, 8					; Se mueve al registro ebx el valor 8
        imul eax, 5					; Se multiplica el valor de eax (n) por 5. (n*5)
		mov edx, 0					; Se vacia el registro edx
        idiv ebx					; Se divide el valor de eax (n*5) en el valor de edx (8). eax = (n*5)/8, edx = (n * 5)%8
        mov numL, eax				; Se mueve a la variable numL el valor de eax ((n * 5) / 8)
        mov numB, edx				; Se nueve a la variable numB el valor de edx ((n * 5) % 8)
		mov eax, 0					;-----------------------------------------------------------------------------------------
		mov ebx, 0					;
		mov ecx, 0					; Se vacian los registros
		mov edx, 0					;-----------------------------------------------------------------------------------------
        mov edx, [edi+4]			; Se mueve al registro edx el apuntador de la información del archivo arch
        add edx, numL				; Se le añade el valor de numL al apuntador de la información del archivo arch
        mov al, [edx]				; Se guarda en el registro al el byte en la posición numL de la información del archivo arch
        cmp numB, 4					; Se compara numB con 4
        jge Else1					; numB < 4
			mov ecx, 0				; Se vacia el registro ecx
            mov cl, 3				; Se mueve el valor 3 al registro cl
            sub ecx, numB			; Se resta el valor de numB al valor almacenado en ecx (3)
			shr al, cl				; Se corren a la derecha (3-numB) bits del registro al
            and al, 0x1F			; Se hace una operación and bit a bit sobre el registro al, (al and 31). ((XXXXXXX)_2 and (00011111)_2) = (000XXXXX)_2
            jmp fin					; Se salta hasta el final del programa
        Else1:						; numB >= 4
			mov ebx, [edi]			; Se mueve al registro ebx el valor apuntado por edi. Es decir, el tamaño del archivo arch
            dec ebx					; Se disminuye en uno el valor de ebx. (arch->tamanho-1)
            cmp ebx, numL			; Se compara ebx con numL
            jne Else2				; ebx == numL, o bien, numL == arch->tamanho - 1
				mov ecx, 0			; Se vacia el registro ecx
				mov cl, -3			; Se mueve el valor -3 al registro cl
				add ecx, numB		; Se suma el valor de numB al valor almacebadi eb ecx (-3)
				shl al, cl			; Se corren a la izquierda (numB-3) bits del registro al
				and al, 0x1F		; Se hace una operación and bit a bit sobre el registro al, (al and 31). ((XXXXXXX)_2 and (00011111)_2) = (000XXXXX)_2
                jmp fin				; Se salta hasta el final del programa
            Else2:					; ebx != numL, o bien, numL != arch->tamanho - 1
				mov ecx, 0			; Se vacia el registro ecx
				mov cl, -3			; Se mueve el valor - 3 al registro cl
				add ecx, numB		; Se suma el valor de numB al valor almacebadi eb ecx(-3)
				shl al, cl			; Se corren a la izquierda(numB - 3) bits del registro al
				and al, 0x1F		; Se hace una operación and bit a bit sobre el registro al, (al and 31). ((XXXXXXX)_2 and (00011111)_2) = (000XXXXX)_2
				mov ebx, 0			; Se vacia el registro ebx
                mov bl, [edx+1]		; Se guarda en el registro bl el byte en la posición (numL+1) de la información del archivo arch
				mov ecx, 0			; Se vacia el registro ecx
                mov cl, 11			; Se mueve el valor 11 al registro cl
                sub ecx, numB		; Se resta el valor de numB al valor almacenado en ecx (11)
				shr bl, cl			; Se corren a la derecha (11-numB) bits del registro al
                or al, bl			; Se hace una operación or bit a bit sobre el registro al con el registro bl, (al or bl). ((XXXXXXX)_2 or (XXXXXXX)_2) = (XXXXXXX)_2
			fin:
    }
}

/*
* Procedimiento que introduce los 5 bits menos significativos del byte 'bits' en el n-�simo grupo de 5 bits de la estructura ARCHIVO apuntada por 'arch'
*/
void meter5bits(ARCHIVO *arch, int n, unsigned char bits) {
	__asm {
        mov ebx, [ebp+8]            ; Se guarda en ebx el apuntador al archivo arch
        mov eax, [ebp+12]           ; Se guaarda en eax el parametro n
        mov  cl, [ebp+16]           ; Se guarda en cl el parametro bits
        mov edi, [ebx+4]            ; Se guarda en edi la información del archivo arch, arch->informacion
		mov esi, 8					; Se mueve al registro ebx el valor 8
        imul eax, 5					; Se multiplica el valor de eax (n) por 5. (n*5)
		mov edx, 0					; Se vacia el registro edx
        idiv esi					; Se divide el valor de eax (n*5) en el valor de edx (8). eax = (n*5)/8, edx = (n * 5)%8
        push eax                    ; numL - [esp+8]
        push edx                    ; numB - [esp+4]
        mov eax, [esp+4]			; Se guarda el valor de numL en eax
		add eax, edi				; Se suma numL + arch->informacion
        mov edx, eax				; Se guarda en edx el puntero pnt y [edx] = letra
        and  cl, 0x1f				; Se realiza una operación and bit a bit con 31. (cl and 31). ((XXXXXXX)_2 and (00011111)_2) = (000XXXXX)_2
        push cl                     ; tmp - [esp]
        mov esi, [esp+4]			; Se guarda en esi el valor de numB
        cmp esi, 4					; Se compara esi con 4
        jge Else1					; numB < 4
			mov eax, 0				; Se vacia el registro eax
			mov ecx, 0				; Se vacia el registro ecx
            mov  al, [esp]			; Se guarda en al el valor de la variable tmp
            mov  cl, 3				; Se guarda en cl el valor 3
            sub ecx, [esp+4]		; Se resta numB a 3. (3 - numB)
            shl  al, cl				; Se corren a la izquierda (3 - numB) bits del registro al 
            or [edx], eax			; Se hace una operación or bit a bit con el valor apuntado por edx y eax
            jmp fin					; Se salta hasta el final del programa
        Else1:						; numB >= 4
			mov esi, 0				; Se limpia el registro esi
            mov esi, [ebx]			; Se mueve al registro esi el valor apuntado por ebx. Es decir, el tamaño del archivo arch
            dec esi					; Se disminuye en uno el valor de esi. (arch->tamanho-1)
            cmp esi, [esp+8]		; Se compara ebx con numL
            jne Else2               ; esi == numL, o bien, numL == arch->tamanho - 1
				mov eax, 0			; Se limpia el registro eax
				mov ecx, 0			; Se limpia ecx
                mov  al, [esp]		; Se mueve a al el valor de la variable tmp
                mov ecx, [esp+4]	; Se mueve a ecx el valor de la variable numB
                sub  cl, 3			; Se resta 3 a numB
                shr  al, cl			; Se hace corrimiento a la derecha a al, (al >> numB-3).
                or [edx], eax		; Se hace una operación or bit a bit sobre la letra apuntada por el registro edx y eax.
                jmp fin				; Se salta hasta el final del programa
            Else2:					; esi != numL, o bien, numL != arch->tamanho - 1
				mov ebx, 0			; Se vacia el registro ebx
				mov eax, 0			; Se vacia el registro eax
				mov ecx, 0			; Se vacia el registro ecx
                mov  al, [esp]		; Se mueve a al el valor de tmp
				mov  bl, [esp]		; Se mueve a bl el valor de tmp
                mov ecx, [esp+4]	; Se mueve a ecx el valor de numB
                sub  cl, 3			; Se resta 3 a numB. (numB-3)
                shr  al, cl			; Se hace corrimiento a la derecha a al, (al >> numB-3).
                or [edx], eax		; Se hace una operación or bit a bit sobre la letra apuntada por el registro edx y eax.
                mov ecx, 0			; Se vacia el registro ecx
                mov  cl, 11			; Se mueve a cl el valor 11
                sub ecx, [esp+4]	; Se resta numB a 11. (11-numB)
                shl  bl, cl			; Se hace corrimiento a la izquierda a bl, (bl << 11-numB).
                or [edx+1], ebx		; Se hace una operación or bit a bit sobre la letra apuntada por el registro edx y ebx.
        fin:
        pop eax 
		pop ebx
		pop ecx						; Se sacan de la pila las variables.
	}
}

/*
* Procedimiento que recibe un byte con informaci�n en los 5 bits menos significativos, y realiza la codificaci�n correspondiente en ASCII
*/
unsigned char codificar(unsigned char cinco) {
	// Registros usados: EAX (AL), EBX (BL)
    __asm {
		mov ebx, 0						; Se vacia el registro ebx
		mov eax, 0						; Se vacia el registro eax
        mov bl, cinco					; Se usa el registro bl para guardar el parametro cinco
        cmp ebx, 0						; Se compara ebx con 0
		jl Else							; ebx >= 0
            cmp ebx, 0x0f				; Se compara ebx con 15
            jg Else						; ebx <= 15
                mov al, 0x40			; Se mueve al registro al el valor 64, al = (01000000)_2
                or al, bl				; Se hace un or bit a bit para poder manipular el 7 bit de bl y dejarlo en 1. ((01000000)_2 or (0000XXXX)_2) = (0100XXXX)_2
                jmp fin					; Se salta hasta el final del programa
        Else:							; ebx > 15
            mov al, 0x20				; Se mueve al registro al el valor 32, al = (00100000)_2
            or al, bl					; Se hace un or bit a bit para poder manipular el 6 bit de bl y dejarlo en 1. ((00100000)_2 or (0001XXXX)_2) = (0011XXXX)_2
        fin:
    }
}

/*
* Procedimiento que convierte la informaci�n en ASCII de la estructura ARCHIVO apuntada por 'data' (archivo de texto) * y la guarda en la estructura ARCHIVO apuntada por 'resultado'.
* como una serie de bytes en la que a cada caracter se le extraen sus �ltimos 5 bits y se guardan en la salida unos a continuaci�n de los otros
*/
void conversionBinario(ARCHIVO *data, ARCHIVO *resultado) {
	// Registros usados: EBX, ECX, EDX, EDI, ESI
    __asm {
        mov ebx, data					; Se mueve a ebx el apuntador del archivo data
        mov ecx, resultado				; Se mueve a ebx el apuntador del archivo resultado
		mov edi, [ebx+4]				; Se mueve a ecx el apuntador a la información del archivo data
        mov esi, 0						; Se inicializa el contador en esi y en 0
        Bwhile: cmp esi, [ebx]			; Se compara el tamaño del archivo data con el contador esi
        jge Ewhile						; esi < data->tamanho
			cmp esi, 0					; Se compara esi con 0
			je zero						; esi != 0
				add edi, 1				; Se añade una posición al apuntador de la información del archivo data
			zero:
			mov edx, 0					; Se vacia el registro edx
            mov dl, [edi]				; Se guarda el Byte, de la posición apuntada por edi, en bl
			push edi
			push ebx
			push ecx
			push esi					; Se salvan los registros usados en este método
            push dl						; Parametros meter5bits - bits
            push esi					; Parametros meter5bits - n
            push ecx					; Parametros meter5bits - arch
            call meter5bits				; Mete el grupo de 5 bits, en la información del archivo resultado, en la posición indicada por esi
			add esp, 12					; Se adelanta el apuntador de la pila 12, pues meter5bits requiere 3 parametros
            pop esi
			pop	ecx
			pop ebx
			pop edi						; Se recuperan los registros usados en este método
            inc esi						; Se incrementa en 1 el valor de esi
        jmp Bwhile
        Ewhile:
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
	//int i;

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