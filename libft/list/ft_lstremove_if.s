section .text
	global _ft_list_remove_if
	extern _free

_ft_list_remove_if:
	mov r14, rdi

.loop:
	mov r11, [r14]		; Check if ptr is null to exit
	cmp r11, 0
	je .exit

	push rsi
	push rdx
	push rcx
	mov rdi, [r11]
	call rdx			; Call the function cmp
	pop rcx
	pop rdx
	pop rsi

	cmp rax, 0			; Delete if needed
	je .remove

	mov r15, [r14]
	add r15, 8			; Otherwise, go to next ref
	mov r14, r15

	jmp .loop

.exit:
	ret

.remove:
	push rsi
	push rdx
	push rcx
	mov rdi, [r14]
	mov rdi, [rdi]
	call rcx			; Call the remove function with the data
	pop rcx
	pop rdx
	pop rsi

	mov r12, [r14]		; r12 = actual to free

	mov r15, [r14]
	add r15, 8
	mov r15, [r15]		; r15 = next
	mov [r14], r15		; r14 = prev ref

	push rsi
	push rdx
	push rcx
	mov rdi, r12
	call _free			; Call free
	pop rcx
	pop rdx
	pop rsi

	jmp .loop
