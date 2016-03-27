.globl dritt
.globl printf

.data
.align 4
flag1: .string "TG16{f0rmat_strings_4re_"
flag2: .string "_rait?}\n"
lol: .string "NOPE\n"

.text
.align 4

dritt:
	push	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax

	# make some room
	sub	$256, %esp
	movl	$0xc001, 12(%esp)	# 4th
	movl	$lol, 8(%esp)		# 3rd
	movl	$flag1, 4(%esp)		# 2nd
	movl	$flag2, (%esp)		# 1st

	pushl	%eax		# buf
	call	printf
	add	$0x256, %esp

	leave
	ret
