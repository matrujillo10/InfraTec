convertir PROC
	pop eax			; se recibe p desde la pila
	mov ebx, "-"
	cmp eax, ebx
	jne notEquals
		mov ecx, 0
		jmp end
	notEquals:
	mov ebx, "a"
	add eax, 1
	cmp eax, ebx
	jne notEqualsA
		mov ecx, 1
		jmp end
	notEqualsA:
	mov ebx, "b"
	cmp eax, ebx
	jne notEqualsB
		mov ecx, 2
		jmp end
	notEqualsB:
	mov ecx, 0
	end:
	push ecx
	ret 4
convertir ENDP
