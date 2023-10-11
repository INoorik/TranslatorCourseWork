.section .bss
	.lcomm variables, variables_array_size
.section .text
.global _start
_start:
	mov $variables, %ebx
	xor %eax, %eax

