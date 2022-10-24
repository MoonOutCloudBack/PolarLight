#include <inc/tlbop.h>
#include <inc/m32c0.h>
#include <inc/printf.h>
#include <inc/mmu.h>
#include <inc/pmap.h>

void print_tlb()
{
    u32 tlb_size = mips_tlb_size();
    printf("TLB size: %d\n", tlb_size);
    unsigned int i;
    tlbhi_t phi;
    tlblo_t plo0, plo1;
    unsigned pmsk;
    for (i = 0; i < tlb_size; ++i)
    {
        mips_tlbri2(&phi, &plo0, &plo1, &pmsk, i);
        printf("%d hi %x lo0 %x lo1 %x pmsk %x\n", i, phi, plo0, plo1, pmsk);
    }
}
extern Pde *boot_pgdir;
extern u32 mCONTEXT;
void my_tlb_refill()
{

    /**
     * 
     * 页目录在 mCONTEXT
     * 要重填的虚拟地址是在 BadVAddr
     * 通过查找页目录来看虚拟地址对应的物理地址，然后填入TLB，用封装tlb库
     */

    u32 va = mips32_get_c0(C0_BADVADDR);
    u_long r;
    struct Page *p = NULL;

    if (mCONTEXT < 0x80000000)
    {
        panic("tlb refill and alloc error!");
    }

    if ((va > 0x7f400000) && (va < 0x7f800000))
    {
        panic(">>>>>>>>>>>>>>>>>>>>>>it's env's zone");
    }

    if (/*va < 0x10000 */ false)
    {
        panic("^^^^^^TOO LOW^^^^^^^^^");
    }

    if ((r = page_alloc(&p)) < 0)
    {
        panic("page alloc error!");
    }

    p->pp_ref++;

    page_insert((Pde *)mCONTEXT, p, VA2PFN(va), PTE_R);
    printf("pageout:\t@@@___0x%x___@@@  ins a page \n", va);

    //u32 pa = (va2pa((Pde *)mCONTEXT, badvaddr) | 0x80000000);
    u32 pa = va2pa((Pde *)mCONTEXT, va| 0x80000000);
    //u32 pa = va2pa((Pde *)mCONTEXT, va);
    // printf("page2pa: %x\n", page2pa(p));
    printf("va2pa: %x\n",va2pa((Pde*)mCONTEXT,va));
    //u32 pa = (page2pa(p) | PTE_R | PTE_V);
    // /* TODO: 判断NOPAGE等情况 */

    // printf("In my_tlb_fill badvaddr %x pa %x\n", va, pa);
    // /* !!!NOTICE!!! TLB被设置但是地址转换失败 */
    //mips_tlbrwr2(va, pa | 0b110, 0, 0);
    
    extern int mips_tlbrwr2(tlbhi_t hi, tlblo_t lo0, tlblo_t lo1, unsigned msk);
    mips_tlbrwr2(VA2PFN(va), pa, 0, 0);
    printf("TLB bas been refilled \n");

    /* 看一看我们的重填结果 */
    print_tlb();
}