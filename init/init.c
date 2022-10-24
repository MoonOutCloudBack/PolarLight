#include <mips/cpu.h>
#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>
#include <../inc/types.h>
#include <../user/shell.h>
 
#define K_ENV 0x88000000
#define KENV_A 0x88010000
#define KENV_B 0x88020000

extern int mCONTEXT;

void interface_init()
{
    printf("\n");
    printf("\n");
    printf("\n");
    printf("       db                                                                      \n");
    printf("      d88b                                                                     \n");
    printf("     d8\'`8b                                                                    \n");
    printf("    d8\'  `8b     88       88  8b,dPPYba,   ,adPPYba,   8b,dPPYba,  ,adPPYYba,  \n");
    printf("   d8YaaaaY8b    88       88  88P\'   \"Y8  a8\"     \"8a  88P\'   \"Y8  \"\"     `Y8  \n");
    printf("  d8\"\"\"\"\"\"\"\"8b   88       88  88          8b       d8  88          ,adPPPPP88  \n");
    printf(" d8\'        `8b  \"8a,   ,a88  88          \"8a,   ,a8\"  88          88,    ,88  \n");
    printf("d8\'          `8b  `\"YbbdP\'Y8  88           `\"YbbdP\"\'   88          `\"8bbdP\"Y8  \n");
    printf("\n");
    //boot_music();
}
void time_setup()
{
    char * buf;
    char * h, *m, *s;
    printf("Please setup the system time(xx:xx:xx) \n");
    h = readline("hour> ");
    m = readline("minute> ");
    s = readline("sec> ");
    
    set_seven_seg_value(*s);
}
void device_init()
{
 	set_leds(0x5555); 
  	mips32_bicsr (SR_BEV);
    mips32_bissr (SR_IE | SR_HINT0 | SR_HINT1 | SR_HINT2 | SR_HINT3 | SR_HINT4);
    init_seven_seg();
    enable_all_seven_seg();
    cons_init();
    //time_setup();
}
void sys_init()
{
    printf("\n");
	//mips_tlbinvalall (); //??????为什么禁用tlb???
    printf("*******Start to detect available memory space:\n");
	set_physic_mm();
    printf("\n");
    printf("*******Start to initialize the virtual memory:\n");
	vm_init();
    printf("\n");
    printf("*******Start to initialize page memory management:\n");
	page_init();
    page_check();
    printf("\n");
    printf("*******Start to initialize process management:\n");
	env_init();
    printf("\n");
	env_check();
    printf("*******Start to load user program:\n");
    //ENV_CREATE(user_idle);
    printf("\n");
    printf("*******Start to initialize traps:\n");
	trap_init();
    printf("\n");
    printf("*******Start to initialize kclock:\n");
    
    printf("kclock init has been completed\n"); 
    printf("\n");
    printf("*******The whole system is ready!\n");
    
    interface_init(); //initialize the interface
    
    //tlb_refill_test();
    env_create_priority("ushell.elf", 0, 1);
    // uint32_t *entry_point = NULL;
    // load_elf_mapper("ushell.elf", entry_point);
    
    // asm("li $4, 0x15001e60");
    // asm("jalr $4");

    // sc_test();
    // Trapframe * tf = NULL;
    
    // //kenv_create(K_ENV, 1000);
    // kenv_create(KENV_A, 1000);
    // kenv_create(KENV_B, 1000);

    //load_binary_test();

    //print_tlb();
    // printf("\ntlb refill is OK \n");
    // Pde* pgdir = 0x80400000;
    // printf("va2pa va: %x, pa: %x",0x10000000, va2pa(pgdir, 0x10000000));
    asm ("ei");
    
    kclock_init();
    //sched_yield();
    
    while(1){
        //shell(NULL);
    }
    panic("!!!!!!!!!!!!!!!!!!");
    //sched_yield();
  //  panic("!!!!!!!!!!!!!!!!!!");
	//while (1) monitor(NULL);
}

/**
 * this function is used to test the syscall functions 
 */
void sc_test()
{
    u32 sw = syscall_get_switchs();
    
    syscall_set_leds(sw);
    int a = 100;
    syscall_printf("a: %d\n", a);
    char * msg = "syscall success!\n";
    syscall_printf(msg);
    
    return;
}

void code_test()
{
    //yscall_set_leds(0xffffffff);
    set_leds(0xff);
}


void SD_TEST()
{
    //spi_init();
    // for(uint8_t i = 0;i < 15; i++){
    //     set_leds(xchg_spi(i));
    //     for(uint32_t j = 0; j < 1000*00000; j++);
    // }
    disk_initialize(0);
}

