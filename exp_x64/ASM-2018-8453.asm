.code

GetPEB proc
	mov rax, gs:[60h]
	ret
GetPEB endp

NtUserSetWindowFNID proc
	mov r10, rcx
	mov r11, rdx
	mov eax, 1095h
	syscall
	ret
NtUserSetWindowFNID endp

end