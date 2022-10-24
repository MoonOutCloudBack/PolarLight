#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>

.macro	__build_clear_sti
	STI
.endm

.macro	__build_clear_cli
	CLI
.endm

# .macro	BUILD_HANDLER exception handler clear
# 	.align	5
# 	NESTED(handle_\exception, TF_SIZE, sp)  
# 	//.set	noat

# nop

# 	SAVE_ALL				
# 	__build_clear_\clear
# 	.set	at
# 	move	a0, sp
# 	la 		v0, \handler
# 	jr 		v0
# 	#jal	\handler
# 	nop
# 	j	ret_from_exception
# 	nop
# 	END(handle_\exception)
# .endm

NESTED(handle_tlb, TF_SIZE, sp)
	nop
	SAVE_ALL
	CLI
	# EntryHi[31:13] field is set by hardware on TLB exception or on a TLB read instruction
	# We just need to set EntryHi[7:0] (ASID) field
	# mfc0 	k0, CP0_BADVADDR
	# and 	k0, 0xfffff000
	# mtc0 	k0, CP0_ENTRYHI
	li 		k0, 0x00001800
	mtc0	k0, CP0_PAGEMASK
	mfc0	k0, CP0_ENTRYHI
	or 		k0, 0x0 # 0x0 is must write by ASID in futher
	mtc0	k0, CP0_ENTRYHI
	# Set EntryLo, need to search page table
	# search page table, if the item is invalible then pageout
	mfc0	k0, CP0_BADVADDR
	move	a1, k0
	lw		a0, mCONTEXT
	sw 		ra, tlbra
	jal 	va2pa
	nop
	lw 		ra, tlbra
	li		t0, 0xffffffff
	nop
	beq		v0, t0, page_fault	# if $t0 == $t1 then target
	nop		
	ehb
	j		b1				# jump to b1 ,not page fault
	
page_fault:  				#page fault occur
	mfc0	k0, CP0_BADVADDR
	move	a0, k0
	lw		a1, mCONTEXT
	nop
	sw 		ra, tlbra
	jal 	pageout
	nop
	lw 		ra, tlbra
							#下面的是瞎写的，回头改！！！
	mfc0	k0, CP0_BADVADDR
	move	a0, k0
	addi 	a0, 0x1000
	lw		a1, mCONTEXT
	nop
	sw 		ra, tlbra
	jal 	pageout
	nop
	lw 		ra, tlbra
							#over
b1:
	move 	k1, v0		#v0 is pa
	srl		k1, 12
	sll 	k1, 6
	or		k1, 0x6

	mfc0	k0, CP0_BADVADDR
	and 	k0, 0x1000			#####可能这里有问题
	nop
	bnez	k0, SET_ENTRYLO1
	nop
SET_ENTRYLO0:
	mtc0 	k1, CP0_ENTRYLO0
	addi	k1, -0x1000
	mtc0    k1, CP0_ENTRYLO1
	nop		
	j 		tlb_refill_done
	nop
SET_ENTRYLO1:
	# mtc0 	k1, CP0_ENTRYLO1
	# mtc0    zero, CP0_ENTRYLO0
tlb_refill_done:
	ehb
	tlbwr
	nop
	RESTORE_ALL
	STI
	ehb
	nop
	eret


	# cli
	# .set at
	# move	a0, sp
	# la 		v0, my_refill #my_refill #do_refill
	# jr 		v0
	# #jal 	my_refill
	# nop
	# j	ret_from_exception
	# nop
END(handle_tlb)

NESTED(handle_mod, TF_SIZE, sp)
	nop
	cli
	.set at
	move	a0, sp
	la 		v0, page_fault_handler
	jr 		v0
	nop
	j	ret_from_exception
	nop
END(handle_mod)

NESTED(handle_reserved, TF_SIZE, sp)
	nop
	cli
	.set at
	move	a0, sp
	la 		v0, do_reserved
	jr 		v0
	nop
	j	ret_from_exception
	nop
END(handle_reserved)

FEXPORT(ret_from_exception)
	.set noat
	.set noreorder
	RESTORE_SOME
	
	lw	k0,TF_EPC(sp)				 
	lw	sp,TF_REG29(sp) /* Deallocate stack */  
//1:	j	1b
	nop
	mfc0 k1, CP0_STATUS
	li   a0, 0x10000000
	or  k1, a0
	mtc0 k1, CP0_STATUS
	mtc0 k0, CP0_EPC      # TODO fix
	li   k0, 0x00000000
	mtc0 k0, CP0_CAUSE
	nop
	STI
	eret
	.set at
	# jr	k0								 
	# rfe									 



.set noreorder
# .align	5
NESTED(handle_int, TF_SIZE, sp)
.set	noat

//1: j 1b
nop

SAVE_TF 	#SAVE_ALL
CLI
//.set	at
mfc0	t0, CP0_CAUSE
mfc0	t2, CP0_STATUS

and	t0, t2

andi	t1, t0, STATUSF_IP0

bnez	t1, timer_irq
nop
nop
j  		ret_from_exception
# andi	t1, t0, STATUSF_IP2
# bnez	t1, button_deal
nop
END(handle_int)

	.extern delay

button_deal:
	# jal disable_timer0
	# nop	
	jal	read_button
	nop
	# jal enable_timer0
	# nop
	j	ret_from_exception
	nop


timer_irq:
	nop
	jal clear_timer0_int
	nop
	jal	sched_yield
	nop
	
	# mfc0	t0, CP0_CAUSE
	# andi 	t1, t0, 0xfbff
	# mtc0	t1, CP0_CAUSE

	# jal enable_timer0

	nop
	j	ret_from_exception
	nop

LEAF(do_reserved)
	eret
END(do_reserved)

	.extern tlbra
.set	noreorder
.set 	at
NESTED(do_refill,0 , sp)
			//li	k1, '?'
			//sb	k1, 0x90000000
			.extern	mCONTEXT
//this "1" is important
1:			//j 1b
			nop
			lw		k1,mCONTEXT
			and		k1,0xfffff000
			mfc0		k0,CP0_BADVADDR
			srl		k0,20
			and		k0,0xfffffffc
			addu		k0,k1
			
			lw		k1,0(k0)
			nop
			move		t0,k1
			and		t0,0x0200
			beqz		t0,NOPAGE
			nop
			and		k1,0xfffff000
			mfc0		k0,CP0_BADVADDR
			srl		k0,10
			and		k0,0xfffffffc
			and		k0,0x00000fff
			addu		k0,k1

			or		k0,0x80000000
			lw		k1,0(k0)
			nop
			move		t0,k1
			and		t0,0x0200
			beqz		t0,NOPAGE
			nop
			move		k0,k1
			and		k0,0x1
			beqz		k0,NoCOW
			nop
			and		k1,0xfffffbff
NoCOW:
			or		k1,0x0000003e  #set ENTRYLO C=111, D=1, V=1, G=0 
			mtc0	k1,CP0_ENTRYLO0
			nop
			tlbwr

			j		2f
			nop
NOPAGE:
//3: j 3b
nop
			mfc0		a0,CP0_BADVADDR
			lw		a1,mCONTEXT
			nop
				
			sw	 	ra,tlbra
			jal 	pageout
			nop
//3: j 3b
nop			
			lw		ra,tlbra
			nop

			j	1b
2:			nop

			jr		ra
			nop
END(do_refill)


LEAF(my_refill)
	
	mfc0 k0, CP0_BADVADDR
	mtc0 k0, CP0_ENTRYHI
	li 	 k1, 0x00040006
	mtc0 k1, CP0_ENTRYLO0
	mtc0 zero, CP0_ENTRYLO1

	ehb
	tlbwr
	nop

	eret
   	#jr 	ra
END(my_refill)

# LEAF(tlb_refill)
# 	SAVE_ALL
# 	CLI
# 	# EntryHi[31:13] field is set by hardware on TLB exception or on a TLB read instruction
# 	# We just need to set EntryHi[7:0] (ASID) field
# 	# mfc0 	k0, CP0_BADVADDR
# 	# and 	k0, 0xfffff000
# 	# mtc0 	k0, CP0_ENTRYHI
# 	mfc0	k0, CP0_ENTRYHI
# 	or 		k0, 0x0 # 0x0 is must write by ASID in futher
# 	mtc0	k0, CP0_ENTRYHI
# 	# Set EntryLo, need to search page table
# 	# search page table, if the item is invalible then pageout
# 	mfc0	k0, CP0_BADVADDR
# 	move	a1, k0
# 	lw		a0, mCONTEXT
# 	sw 		ra, tlbra
# 	jal 	va2pa
# 	nop
# 	lw 		ra, tlbra
# 	li		t0, 0xffffffff
# 	nop
# 	beq		v0, t0, page_fault	# if $t0 == $t1 then target
# 	nop		
# 	ehb
# 	j		b1				# jump to b1 ,not page fault
	
# page_fault:  				#page fault occur
# 	mfc0	k0, CP0_BADVADDR
# 	move	a0, k0
# 	lw		a1, mCONTEXT
# 	nop
# 	sw 		ra, tlbra
# 	jal 	pageout
# 	nop
# 	lw 		ra, tlbra
# b1:
# 	move 	k1, v0		#v0 is pa
# 	srl		k1, 12
# 	sll 	k1, 6
# 	or		k1, 0x6

# 	mfc0	k0, CP0_BADVADDR
# 	and 	k0, 0x1000
# 	bnez	k0, SET_ENTRYLO1
	
# SET_ENTRYLO0:
# 	mtc0 	k1, CP0_ENTRYLO0
# 	j 		tlb_refill_done
# SET_ENTRYLO1:
# 	mtc0 	k1, CP0_ENTRYLO1
# tlb_refill_done:
# 	ehb
# 	tlbwr

# 	RESTORE_ALL
# 	STI
# 	eret

# END(tlb_refill)
#BUILD_HANDLER reserved do_reserved cli
#BUILD_HANDLER tlb	do_refill	cli
#BUILD_HANDLER mod	page_fault_handler cli
