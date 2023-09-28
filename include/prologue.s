.section .bss
	.lcomm variables, variables_array_size
.section .text
.global main
main:
	mov $variables, %ebx
	xor %eax, %eax

