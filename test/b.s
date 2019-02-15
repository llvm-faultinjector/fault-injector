	.text
	.def	 @feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 1
	.def	 _main;
	.scl	2;
	.type	32;
	.endef
	.globl	_main                   # -- Begin function main
	.p2align	4, 0x90
_main:                                  # @main
# %bb.0:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$20, %esp
	movl	$999, (%esp)            # imm = 0x3E7
	calll	_fault_inject_init
	movl	$0, -16(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -12(%ebp)
	movl	$0, -8(%ebp)
LBB0_1:                                 # =>This Inner Loop Header: Depth=1
	cmpl	$10, -8(%ebp)
	jge	LBB0_4
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-8(%ebp), %eax
	addl	-12(%ebp), %eax
	movl	%eax, -12(%ebp)
# %bb.3:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	LBB0_1
LBB0_4:
	movl	-16(%ebp), %esi
	calll	_fault_inject_finish
	movl	%esi, %eax
	addl	$20, %esp
	popl	%esi
	popl	%ebp
	retl
                                        # -- End function

