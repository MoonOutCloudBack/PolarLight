#include <mips/regdef.h>
#include <mips/m32c0.h>
#include <mips/asm.h>
#include <inc/trap.h>
			.data
			.global	KERNEL_SP;
KERNEL_SP:
			.word		0



	.text
	.set noat
	
LEAF(env_pop_tf)
	.set noreorder
	addiu	sp,sp,-32
	sw  	ra,28(sp)
	move	k0, a0			# k0 = &curenv->env_tf
	mtc0	a1, C0_ENTRYHI	# C0_ENTRYHI = ASID

	mfc0	t0,C0_STATUS
	ori		t0,0x3
	xori	t0,0x3
	mtc0	t0,C0_STATUS


	lw 		k1, TF_CAUSE(k0)
	mtc0	k1, C0_CAUSE
	lw		v1,	TF_LO(k0)
	lw		v0,	TF_HI(k0)
	mtlo	v1	
	lw		v1,	TF_EPC(k0)
	mthi	v0	
	mtc0	v1,	C0_EPC
	lw		$31,TF_REG31(k0)
	lw		$30,TF_REG30(k0)
	#lw		$29,TF_REG29(k0)															
	lw		$28,TF_REG28(k0)
	lw		$25,TF_REG25(k0)
	lw		$24,TF_REG24(k0)
	lw		$23,TF_REG23(k0)
	lw		$22,TF_REG22(k0)
	lw		$21,TF_REG21(k0)
	lw		$20,TF_REG20(k0)
	lw		$19,TF_REG19(k0)
	lw		$18,TF_REG18(k0)
	lw		$17,TF_REG17(k0)
	lw		$16,TF_REG16(k0)
	lw		$15,TF_REG15(k0)
	lw		$14,TF_REG14(k0)
	lw		$13,TF_REG13(k0)
	lw		$12,TF_REG12(k0)
	lw		$11,TF_REG11(k0)
	lw		$10,TF_REG10(k0)
	lw		$9,TF_REG9(k0)
	lw		$8,TF_REG8(k0)
	lw		$7,TF_REG7(k0)
	lw		$6,TF_REG6(k0)
	lw		$5,TF_REG5(k0)
	lw		$4,TF_REG4(k0)
	lw		$3,TF_REG3(k0)
	lw		$2,TF_REG2(k0)
	lw		$1,TF_REG1(k0)
	lw		k1,TF_PC(k0) #test
	lw		k0,TF_STATUS(k0)
	#li		k0, 0x10000400		# $t1 = 
	li		k0, 0x10007c01		#!!!!!一会改
	mtc0	k0,C0_STATUS
	
	#jr      k1
	jalr	 k1
	

	nop


	nop
	lw		ra,28(sp)
	addiu	sp,sp,32
	nop
	jr 	ra
END(env_pop_tf)
	.set at

LEAF(lcontext)
	.extern	mCONTEXT
	sw		a0,mCONTEXT
	jr	ra
	nop
END(lcontext)

LEAF(jump2entry)
	nop
	jr a0
	nop
END(jump2entry)


LEAF(env_push_tf)
	.set noreorder
	.set at
	# mfc0 	k1, C0_PC
	# subi	k1, k1, 16		 
	# sw 		k1, TF_PC(a0)
	move 	k0, a0
	mfc0 	k1, C0_STATUS
	sw 		k1, TF_STATUS(k0)
	mfc0    k1, C0_CAUSE
	sw		k1, TF_CAUSE(k0)
	mfc0	k1, C0_ENTRYHI
	sw  	k1, TF_HI(k0)
	mfc0	k1, C0_ENTRYLO0
	sw		k1, TF_LO(k0)
	mfc0    k1, C0_EPC
	sw 		k1, TF_EPC(k0)
	sw		$31,TF_REG31(k0)
	sw		$30,TF_REG30(k0)
	sw		$29,TF_REG29(k0)															
	sw		$28,TF_REG28(k0)
	sw		$25,TF_REG25(k0)
	sw		$24,TF_REG24(k0)
	sw		$23,TF_REG23(k0)
	sw		$22,TF_REG22(k0)
	sw		$21,TF_REG21(k0)
	sw		$20,TF_REG20(k0)
	sw		$19,TF_REG19(k0)
	sw		$18,TF_REG18(k0)
	sw		$17,TF_REG17(k0)
	sw		$16,TF_REG16(k0)
	sw		$15,TF_REG15(k0)
	sw		$14,TF_REG14(k0)
	sw		$13,TF_REG13(k0)
	sw		$12,TF_REG12(k0)
	sw		$11,TF_REG11(k0)
	sw		$10,TF_REG10(k0)
	sw		$9,TF_REG9(k0)
	sw		$8,TF_REG8(k0)
	sw		$7,TF_REG7(k0)
	sw		$6,TF_REG6(k0)
	sw		$5,TF_REG5(k0)
	sw		$4,TF_REG4(k0)
	sw		$3,TF_REG3(k0)
	sw		$2,TF_REG2(k0)
	sw		$1,TF_REG1(k0)


END(env_push_tf)


LEAF(get_pc)

END(get_pc)


