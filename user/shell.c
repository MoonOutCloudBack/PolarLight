// #include "shell.h"

// // Simple command-line kernel shell useful for
// // controlling the kernel and exploring the system interactively.

// //#include <printf.h>
// #include <string.h>
// #include <mmu.h>
// #include <uart.h>
// #include <../fs/ff.h>
// #include <fs.h>
// #include <printf.h>

// #include <console.h>
// #include <tlbop.h>

// // max size of file image is 16M
// #define MAX_FILE_SIZE 0x1000000

// // size of DDR RAM (256M for Minisys) 
// #define DDR_SIZE 0x10000000

// // 4K size read burst
// #define SD_READ_SIZE 4096


// #define CMDBUF_SIZE	80	// enough for one VGA text line
// FATFS FatFs;   // Work area (file system object) for logical drive

// struct Command {
//     const char *name;
//     const char *desc;
//     // return -1 to force shell to exit
//     int (*func)(int argc, char** argv, struct Trapframe* tf);
// };

// static struct Command commands[] = {
//     { "help", "Display this list of commands", mon_help },
//     //{ "kerninfo", "Display information about the kernel", mon_kerninfo },
//     //{ "tlb", "Dianoisplay 16 tlb table entries", mon_tlb },
//     { "piano", "Simulate a simple electric piano", mon_play },
// 	//{ "2048", "A simple 2048 game", mon_2048 },
//     //{ "game", "A simple MineSweeper game", mon_game },
//     { "ls", "List files and directories", mon_ls },
// 	{ "cd", "Change Directory", mon_cd },
//     { "touch", "Create file", mon_touch },
//     { "mkdir", "Create directory", mon_mkdir },
// 	{ "read", "Read a file", mon_read },
// 	{ "write", "Change a file", mon_write },
// 	{ "rm", "Delete files or directories", mon_rm } //，
//     //{ "about", "Display developers' name of this project", mon_about }
// };


// /***** Implementations of basic kernel shell commands *****/

// int
// mon_help(int argc, char **argv, struct Trapframe *tf)
// {
//     for (int i = 0; i < ARRAY_SIZE(commands); i++)
//         printf("%s - %s\n", commands[i].name, commands[i].desc);
//     return 0;
// }

// int
// mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
// {
//     // extern char _start[], entry[], etext[], edata[], end[];
//     // extern u_long npage;
//     // extern Pde *boot_pgdir;
//     // printf("Special kernel symbols:\n");
//     // printf("    KERNBASE is %x \n",KERNBASE);
//     // printf("****Virtual page table**** \n");
//     // printf("    VPT is %x \n",VPT);
//     // printf("    KSTACKTOP is %x \n",KSTACKTOP);
//     // printf("    KSTKSIZE is %d \n",KSTKSIZE);
//     // printf("    ULIM is %x \n",ULIM); 
//     // printf("    UVPT is %x\n",UVPT);
//     // printf("    UPAGES is %x\n",UPAGES);
//     // printf("    UENVS is %x\n",UENVS);
//     // printf("    UTOP is %x\n",UTOP);
//     // printf("    UXSTACKTOP is %x\n",UXSTACKTOP);
//     // printf("    TIMESTACK is %x\n",TIMESTACK);
//     // printf("    USTACKTOP is %x\n",USTACKTOP);
//     // printf("    UTEXT is %x\n",UTEXT);

//     // printf("  _start     %x\n", _start);
//     // printf("  entry      %x\n", boot_pgdir);
//     // printf("  end        %x \n", end);
//     // printf(" the count of page is %d\n",npage);
//     // printf(" %d bytes to a page \n",BY2PG);
//     // printf(" %d bytes mapped by a page directory entry \n",PDMAP);

//     return 0;
// }

// /***** printf tlb info*****/
// int
// mon_tlb(int argc, char **argv, struct Trapframe *tf)
// {
//     print_tlb();
//     return 0;
// }

// /***** stimulate a simple electric piano *****/
// int 
// mon_play(int argc, char **argv, struct Trapframe *tf)
// {
//     // char *buf;
//     // printf("*****input x to exit*******\n");
//     // while (1) {
//     //     buf = readline("Play> ");
//     //     if (buf != NULL&& *buf!='x')
//     //     {
//     //         set_seven_seg_value(*buf);
//     //         set_leds(*buf);
//     //         switch(*buf-48)
//     //         {
//     //             case 0:
//     //             delay_zero();
//     //             printf("pause~\n");
//     //             break;
//     //             case 1:
//     //             delay_do();
//     //             printf("do~\n");
//     //             break;
//     //             case 2:
//     //             delay_re();
//     //             printf("re~\n");
//     //             break;
//     //             case 3:
//     //             delay_mi();
//     //             printf("mi~\n");
//     //             break;
//     //             case 4:
//     //             delay_fa();
//     //             printf("fa~\n");
//     //             break;
//     //             case 5:
//     //             delay_so();
//     //             printf("so~\n");
//     //             break;
//     //             case 6:
//     //             delay_la();
//     //             printf("la~\n");
//     //             break;
//     //             case 7:
//     //             delay_xi();
//     //             printf("xi~\n");
//     //             break;                
//     //         }
//     //         printf("buf is %c\n",*buf);
//     //         //break;
//     //     }
//     //     if(*buf=='x')
//     //     {
//     //         printf("*****thanks for playing this naive game(#^.^#)*******\n");
//     //         break;
//     //     }    

//     // }
// 	FIL fil;                // File object
// 	FRESULT fr;             // FatFs return code
// 	uint8_t *boot_file_buf = (uint8_t *)(get_ddr_base()) + DDR_SIZE - MAX_FILE_SIZE; // at the end of DDR space
// 	uint8_t *memory_base = (uint8_t *)(get_ddr_base());

// 	*boot_file_buf = 47;
// 	printf("=======Start to load piano.elf======\n");
// 	// Register work area to the default drive
// 	// if(f_mount(&FatFs, "", 1)) {
// 	// 	printf("Fail to mount SD driver!\n\r", 0);
// 	// 	return 1;
// 	// }
	
// 	// Open a file
// 	printf("Loading piano.elf into memory...\n\r", 0);
// 	fr = f_open(&fil, "piano.elf", FA_READ);
// 	if (fr) {
// 		printf("Failed to open piano.elf!\n\r", 0);
// 		return (int)fr;
// 	}

// 	// Read file into memory
// 	uint8_t *buf = boot_file_buf;
// 	uint32_t fsize = 0;           // file size count
// 	uint32_t br;                  // Read count
// 	do {
// 		if( fsize % 1024 == 0 ) {
// 		printf("Loading %d KB to memory address \r", fsize / 1024);
// 		}
// 		fr = f_read(&fil, buf, SD_READ_SIZE, &br);  // Read a chunk of source file
// 		buf += br;
// 		fsize += br;
// 	} while(!(fr || br == 0));

// 	printf("Load %d bytes to memory address ", fsize);
// 	printf("%h from piano.elf \n\r", (uint32_t)boot_file_buf);

// 	// read elf
// 	if(br = load_elf_sd(boot_file_buf, fil.fsize))
// 		printf("elf read failed with code %d \n\r", br);

// 	// uint32_t *p;
// 	// get_entry(boot_file_buf, fil.fsize, p);
// 	// printf("elf address : %x", *p);
	
// 	// Close the file
// 	if(f_close(&fil)) {
// 		printf("fail to close file!\n\r", 0);
// 		return 1;
// 	}
	
// 	// if(f_mount(NULL, "", 1)) {         // unmount it
// 	// 	printf("fail to umount disk!\n\r", 0);
// 	// 	return 1;
// 	// }

// 	// spi_disable();
	
// 	asm("li $4, 0x10001310");
//     asm("jalr $4");
	
	
//     return 0;
// }


// /***** A simple MineSweeper game ******/
// void menu()
// {
// 	printf("******************************\n");
// 	printf("******1.play     0.exit*******\n");
// 	printf("******************************\n");
// }
// #define MAXFORGAME 10

// //打印棋盘
// void display(char arr[])
// {
//     int i=0;
   
//     for(;i<10;i++){
//         printf("%c ",arr[10*i+0]);
//         printf("%c ",arr[10*i+1]);
//         printf("%c ",arr[10*i+2]);
//         printf("%c ",arr[10*i+3]);
//         printf("%c ",arr[10*i+4]);
//         printf("%c ",arr[10*i+5]);
//         printf("%c ",arr[10*i+6]);
//         printf("%c ",arr[10*i+7]);
//         printf("%c ",arr[10*i+8]);
//         printf("%c ",arr[10*i+9]);
//         printf("\n");
//     }
// }

// //扫描雷
// int  get_boom(char arr[], int x, int y)//计算周围八个位置雷的个数
// {
//     int num;
//     if(x == 1){
//         if(y == 1){
//             num = arr[10 * x + (y + 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
//                   - 3 * '0';
//         }else if(y == 9){
//             num = arr[10 * x + (y - 1)] + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y]
//                   - 3 * '0';
//         }else{
//             num = arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
//                   + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
//                   - 5 * '0';
//         }
//     }else if(x == 9){
//         if(y == 1){
//             num = arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)] + arr[10 * x + (y + 1)]
//                   - 3 * '0';
//         }else if(y == 9){
//             num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * x + (y - 1)]
//                   - 3 * '0';
//         }else{
//             num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)]
//                   + arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
//                   - 5 * '0';
//         }
//     }else{
//         if(y == 1){
//             num = arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)] + arr[10 * x + (y + 1)]
//                   + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
//                   - 5 * '0';
//         }else if(y == 9){
//             num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * x + (y - 1)]
//                   + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y]
//                   - 5 * '0';
//         }else{
//             num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)]
//                   + arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
//                   + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
//                   - 8 * '0';
//         }
//     }
//     return num;
// }

// //对某一点进行扩展
// void fun(char arr[], char show[], int x, int y)
// {
//     if ((x >= 1) && (x <= 9) && (y >= 1) && (y <= 9))//递归约束条件
//     {
//         if (get_boom(arr, x, y) == 0)//判断雷的个数
//         {
//             show[10 * x + y] = ' ';
//             if (show[10 * (x - 1) + (y - 1)] == '*')//对周围八个位置分别进行递归
//                 fun(arr, show, x - 1, y - 1);
//             if (show[10 * (x - 1) + y] == '*')
//                 fun(arr, show, x - 1, y);
//             if (show[10 * (x - 1) + (y + 1)] == '*')
//                 fun(arr, show, x - 1, y + 1);
//             if (show[10 * x + (y - 1)] == '*')
//                 fun(arr, show, x, y - 1);
//             if (show[10 * x + (y + 1)] == '*')
//                 fun(arr, show, x, y + 1);
//             if (show[10 * (x + 1) + (y - 1)] == '*')
//                 fun(arr, show, x + 1, y - 1);
//             if (show[10 * (x + 1) + y] == '*')
//                 fun(arr, show, x + 1, y);
//             if (show[10 * (x + 1) + (y + 1)] == '*')
//                 fun(arr, show, x + 1, y + 1);
//         }
//         else
//             show[10 * x + y] = get_boom(arr, x, y) + '0';//如果周围有雷则显示雷的个数
        
//     }
// }


// void game()
// {
//     char arr[100] = {' ','1','2','3','4','5','6','7','8','9',
//                      '1','0','0','1','0','0','0','0','0','1',
//                      '2','0','0','0','0','0','0','0','0','0',
//                      '3','0','0','0','0','0','0','0','0','0',
//                      '4','0','0','0','0','0','0','0','0','0',
//                      '5','0','0','0','0','0','0','0','1','1',
//                      '6','0','0','1','0','0','0','0','0','0',
//                      '7','0','1','0','0','0','0','0','0','1',
//                      '8','0','1','0','0','0','0','0','1','0',
//                      '9','0','1','0','0','0','0','0','0','0'};
//     char show[100] = {' ','1','2','3','4','5','6','7','8','9',
//                       '1','*','*','*','*','*','*','*','*','*',
//                       '2','*','*','*','*','*','*','*','*','*',
//                       '3','*','*','*','*','*','*','*','*','*',
//                       '4','*','*','*','*','*','*','*','*','*',
//                       '5','*','*','*','*','*','*','*','*','*',
//                       '6','*','*','*','*','*','*','*','*','*',
//                       '7','*','*','*','*','*','*','*','*','*',
//                       '8','*','*','*','*','*','*','*','*','*',
//                       '9','*','*','*','*','*','*','*','*','*'};
//     int win=0;
//     int x = 0;
//     int y = 0;
//     char *xchar;
//     char *ychar;

    
//     display(show);
//     while (win != MAXFORGAME)//判断是否排完雷
//     {
//         printf("请输入坐标：\n");
//         xchar = readline("X(row) input> ");
//         x = *xchar - '0';
//         ychar = readline("Y(col) input> ");
//         y = *ychar - '0';
//         printf("x is %d y is %d\n",x,y);
//         if (((x >= 1) && (x <= 9)) && ((y >= 1) && (y <= 9)))
//         {
//             if (arr[10 * x + y] == '1')
//             {
//                 printf("踩雷，游戏结束!\n");
//                 printf("*****thanks for playing this naive game(#^.^#)*******\n");
//                 break;
//             }
//             else
//             {
//                 fun(arr, show, x, y);
//                 win = 0;
//                 int i=1;
                
//                 for (; i <= 9; i++)
//                 {

//                         if (show[10 * i + 0] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 1] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 2] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 3] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 4] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 5] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 6] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 7] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 8] == '*'){
//                             win++;
//                         }
//                         if (show[10 * i + 9] == '*'){
//                             win++;
//                         }
 
//                 }
//                 display(show);
//             }
//         }
//         else
//             printf("输入坐标有误\n");
//     }
//     if (win == MAXFORGAME){
//         printf("CONGRATULATIONS!雷阵如下：\n");
//         display(arr);
//     }
// }


// int 
// mon_game(int argc, char **argv, struct Trapframe *tf)
// {
//     char *buf;
//     menu();
//     while (1) {
//         buf = readline("Game> ");
//         if (buf != NULL&& *buf!='0')
//         {
//             game();
           
//             break;
//         }
//         if(*buf=='0')
//         {
//             printf("*****thanks for playing this naive game(#^.^#)*******\n");
//             break;
//         }    

//     }
//     return 0;
// }

// /***** File system operations *****/
// int
// mon_ls(int argc, char **argv, struct Trapframe *tf)
// {
// 	FIL fil;
// 	FRESULT fr;
// 	DIR dir;
// 	FILINFO fno;
// 	if(f_opendir(&dir, "."))
// 		printf("Can not open current directory!\n");
// 	while(1)
// 	{
// 		fr = f_readdir(&dir, &fno);
// 		if(fr != FR_OK || fno.fname[0] == 0) break;
// 		if(fno.fname[0] == '.') continue;
// 		if(fno.fattrib & AM_DIR) 
// 		{
// 			printf("\033[1;36;40m");
// 			printf(fno.fname);
// 			printf("\033[0m");
// 		}
// 		else
// 			printf(fno.fname);
// 		printf(" ");
// 	}
// 	printf("\n");
// 	return 1;
// }

// int
// mon_touch(int argc, char **argv, struct Trapframe *tf)
// {
// 	//my_create(argv[1]);
// 	FIL fil;
// 	FRESULT fr;
// 	fr = f_open(&fil, argv[1], FA_CREATE_ALWAYS);
// 	if(fr)
// 		{
// 			printf("Failed to create file <");
// 			printf(argv[1]);
// 			printf(">\n");
// 		}
//     return 1;
// }

// int mon_cd(int argc, char **argv, struct Trapframe *tf)
// {
// 	if(f_chdir(argv[1]))
// 		{
// 			printf("Failed to change directory to <");
// 			printf(argv[1]);
// 			printf(">\n");
// 		}
//     return 1;
// }

// int
// mon_mkdir(int argc, char **argv, struct Trapframe *tf)
// {
// 	if(f_mkdir(argv[1]))
// 		{
// 			printf("Failed to make directory <");
// 			printf(argv[1]);
// 			printf(">\n");
// 		}
// 	return 1;
// }

// int
// mon_rm(int argc, char **argv, struct Trapframe *tf)
// {
// 	if(f_unlink(argv[1]))
// 		{
// 			printf("Failed to remove <");
// 			printf(argv[1]);
// 			printf(">\n");
// 		}
// 	return 1;
// }

// /***** Display developers' name of this project *****/
// int
// mon_about(int argc, char **argv, struct Trapframe *tf)
// {
//     printf("******************************************\n");
// 	printf("****** Developer List ******\n");
//     printf("******     JIAJUN XIE         *******\n");
//     printf("******       FAN XU           *******\n");
//     printf("******      ZHONG LIU         *******\n");
//     printf("******    YONGTING YOU        *******\n");
//     printf("******     TINGXUAN GU        *******\n");
//     printf("******     JINMIAO SU         *******\n");
//     return 0;
// }

// /***** Kernel shell command interpreter *****/

// #define WHITESPACE "\t\n "
// #define MAXARGS 16

// static int
// runcmd(char *buf, struct Trapframe *tf)
// {
// 	//printf("%s\n", "In runcmd");
//     int argc;
//     char *argv[MAXARGS];
//     int i;

//     // Parse the command buffer into whitespace-separated arguments
//     argc = 0;
//     argv[argc] = 0;
//     while (1) {
//         // gobble whitespace
//         while (*buf && strchr(WHITESPACE, *buf))
//             *buf++ = 0;
//         if (*buf == 0)
//             break;

//         // save and scan past next arg
//         if (argc == MAXARGS-1) {
//             printf("Too many arguments (max %d)\n", MAXARGS);
//             return 0;
//         }
//         argv[argc++] = buf;
//         while (*buf && !strchr(WHITESPACE, *buf))
//             buf++;
//     }
//     argv[argc] = 0;

//     // Lookup and invoke the command
//     if (argc == 0)
//         return 0;
//     for (i = 0; i < ARRAY_SIZE(commands); i++) {
//         if (strcmp(argv[0], commands[i].name) == 0)
//             return commands[i].func(argc, argv, tf);
//     }
//     printf("Unknown command '%s'\n", argv[0]);
//     return 0;
// }

// void
// shell(struct Trapframe *tf)
// {
//     char *buf;

//     printf("Aurora, an operating system based on MIPS32\n");
//     printf("Type 'help' for more commands.\n");


//     while (1) {
//         buf = readline("Aurora> ");
//         if (buf != NULL)
//             if (runcmd(buf, tf) < 0)
//                 break;
//     }
// }

// #define GRID_WIDTH  4
// #define GRID_HEIGHT 4
// #define GRID_SIZE   (GRID_WIDTH * GRID_HEIGHT)

// static void clear_grid(unsigned int * grid)
// {
// 	int i;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		grid[i] = 0;
// 	}
// }

// static void print_grid(unsigned int * grid)
// {
// 	int i, x, y;
// 	int size=0;
// 	int bigest_size;
// 	char tmp_buff[16];

// 	// Try to get the largest sized number
// 	bigest_size = 1;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		//size = sprintf(tmp_buff, "%u", grid[i]);
//         int temp=grid[i];
//         while(temp!=0)
//         {
//             temp/=10;
//             size++;
//         }

// 		if(size > bigest_size)
// 		{
// 			bigest_size = size;
// 		}
//         size=0;
// 	}

// 	// put everything onto the screen
// 	x = 0;
// 	y = 0;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		if(grid[i] == 0)
// 		{
// 			//printf("[%*s] ", bigest_size, "");
// 			printf("[%5s] ", " ");
// 		}
// 		else
// 		{
// 			char temp_str[16]={0};
// 			int temp_num=grid[i];
// 			Int2String(temp_num,temp_str);
// 			//printf("[%*u] ", bigest_size, grid[i]);
// 			printf("[%5s] ", temp_str);
// 		}

// 		x ++;
// 		if(x >= GRID_WIDTH)
// 		{
// 			x = 0;
// 			y ++;
// 			printf("\n");
// 		}
// 	}
// }

// static void add_value_at_random(unsigned int * grid, unsigned int value)
// {
// 	int free_cells;
// 	int spots_left;
// 	int i;
// 	int rand_num;

// 	int target_cell, cell;

// 	free_cells = 0;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		if(grid[i] == 0)
// 		{
// 			free_cells ++;
// 		}
// 	}

// 	if(free_cells > 0)
// 	{
// 		// target_cell = rand() % free_cells;
//         target_cell = free_cells-1;
// 		cell = 0;
// 		for(i = 0; i < GRID_SIZE; i++)
// 		{
// 			if(grid[i] == 0)
// 			{
// 				if(cell == target_cell)
// 				{
// 					grid[i] = value;
// 					break;
// 				}
// 				else
// 				{
// 					cell ++;
// 				}
// 			}
// 		}
// 	}
// }

// static int update_cell(unsigned int * grid, 
// 							  int num_index, 
// 							  int end_index, 
// 							  int delta_index, 
// 							  unsigned int * score)
// {
// 	int i;
// 	int first_num_index, second_num_index;
// 	int valid_move;
// 	unsigned int value;

// 	if(grid[num_index] == 0)
// 	{
// 		first_num_index = -1;
// 	}
// 	else
// 	{
// 		first_num_index = num_index;
// 	}
// 	second_num_index = -1;

// 	i = num_index;
// 	while(i != end_index && second_num_index < 0)
// 	{

// 		i += delta_index;
// 		if(grid[i] != 0)
// 		{
// 			if(first_num_index < 0)
// 			{
// 				first_num_index = i;
// 			}
// 			else if(second_num_index < 0)
// 			{
// 				second_num_index = i;
// 			}
// 		}

// 	}

	

// 	// Move numbers
// 	if(first_num_index  >= 0 &&
// 		second_num_index >= 0)
// 	{
// 		if(grid[first_num_index] == grid[second_num_index])
// 		{
// 			value = grid[first_num_index] + grid[second_num_index];
// 			grid[first_num_index]  = 0;
// 			grid[second_num_index] = 0;
// 			grid[num_index]        = value;
// 			valid_move             = 1;
// 			if(score != NULL)
// 			{
// 				(*score) += value;
// 			}
// 		}
// 		else
// 		{
// 			value                 = grid[first_num_index];
// 			grid[first_num_index] = 0;
// 			grid[num_index]       = value;
// 			if(first_num_index != num_index)
// 			{
// 				valid_move = 1;
// 			}
// 			else
// 			{
// 				valid_move = 0;
// 			}
// 		}
// 	}
// 	else if(first_num_index >= 0)
// 	{
// 		value                 = grid[first_num_index];
// 		grid[first_num_index] = 0;
// 		grid[num_index]       = value;
// 		valid_move            = 1;
// 		if(first_num_index != num_index)
// 		{
// 			valid_move = 1;
// 		}
// 		else
// 		{
// 			valid_move = 0;
// 		}

// 	}
// 	else
// 	{
// 		valid_move = 0;
// 	}

// 	return valid_move;
// }

// static int move_up(unsigned int * grid, unsigned int * score)
// {
// 	int i1, i2, x, y, end_index;
// 	int valid_move;

// 	valid_move = 0;
// 	i1 = 0;
// 	for(x = 0; x < GRID_WIDTH; x++)
// 	{
// 		i2 = i1;
// 		end_index = i1 + (GRID_SIZE - GRID_WIDTH);
// 		for(y = 0; y < GRID_HEIGHT; y++)
// 		{
// 			if(update_cell(grid, i2, end_index, GRID_WIDTH, score) == 1)
// 			{
// 				valid_move = 1;
// 			}  

// 			i2 += GRID_WIDTH;
// 		}
// 		i1 ++;
// 	}

// 	return valid_move;
// }

// static int move_down(unsigned int * grid, unsigned int * score)
// {
// 	int i1, i2, x, y, end_index;
// 	int valid_move;

// 	valid_move = 0;
// 	i1 = GRID_SIZE - GRID_WIDTH;
// 	for(x = 0; x < GRID_WIDTH; x++)
// 	{
// 		i2 = i1;
// 		end_index = i1 - (GRID_SIZE - GRID_WIDTH);
// 		for(y = 0; y < GRID_HEIGHT; y++)
// 		{
// 			if(update_cell(grid, i2, end_index, -GRID_WIDTH, score) == 1)
// 			{
// 				valid_move = 1;
// 			}  
// 			i2 -= GRID_WIDTH;
// 		}
// 		i1 ++;
// 	}

// 	return valid_move;
// }

// static int move_left(unsigned int * grid, unsigned int * score)
// {
// 	int i1, i2, x, y, end_index;
// 	int valid_move;

// 	valid_move = 0;
// 	i1 = 0;
// 	for(y = 0; y < GRID_HEIGHT; y++)
// 	{
// 		i2 = i1;
// 		end_index = i1 + (GRID_WIDTH - 1);
// 		for(x = 0; x < GRID_WIDTH; x++)
// 		{
// 			if(update_cell(grid, i2, end_index, 1, score) == 1)
// 			{
// 				valid_move = 1;
// 			}  

// 			i2 ++;
// 		}
// 		i1 += GRID_WIDTH;
// 	}

// 	return valid_move;
// }

// static int move_right(unsigned int * grid, unsigned int * score)
// {
// 	int i1, i2, x, y, end_index;
// 	int valid_move;

// 	valid_move = 0;
// 	i1 = GRID_WIDTH - 1;
// 	for(y = 0; y < GRID_HEIGHT; y++)
// 	{
// 		i2 = i1;
// 		end_index = i1 - (GRID_WIDTH - 1);
// 		for(x = 0; x < GRID_WIDTH; x++)
// 		{
// 			if(update_cell(grid, i2, end_index, -1, score) == 1)
// 			{
// 				valid_move = 1;
// 			}  

// 			i2 --;
// 		}
// 		i1 += GRID_WIDTH;
// 	}

// 	return valid_move;
// }


// // Returns 1 if a move is avaliable
// static int can_move(unsigned int * grid)
// {
// 	int i, x, y;
// 	int result;

// 	result = 0;
// 	x      = 0;
// 	y      = 0;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		// Check for an empty cell
// 		if(grid[i] == 0)
// 		{
// 			result = 1;
// 			break;
// 		}
		
// 		// Check to see if the cell to the right matches
// 		if(x < GRID_WIDTH - 1)
// 		{
// 			if(grid[i] == grid[i + 1])
// 			{
// 				result = 1;
// 				break;
// 			}
// 		}

// 		// Check to see if the cell to the left matches
// 		if(y < GRID_HEIGHT - 1)
// 		{
// 			if(grid[i] == grid[i + GRID_WIDTH])
// 			{
// 				result = 1;
// 				break;
// 			}
// 		}

// 		// Update x and y
// 		x ++;
// 		if(x >= GRID_WIDTH)
// 		{
// 			x = 0;
// 			y ++;
// 		}
// 	}

// 	return result;
// }

// unsigned int max_value(unsigned int * grid)
// {
// 	int i;
// 	unsigned int value;
// 	value = 0;
// 	for(i = 0; i < GRID_SIZE; i++)
// 	{
// 		if(grid[i] > value)
// 		{
// 			value = grid[i];
// 		}
// 	}
// 	return value;
// }

// int mon_2048(int args, char * argc[])
// {
// 	unsigned int grid[GRID_SIZE];
// 	unsigned int score;
// 	unsigned int moves;
// 	unsigned int mv;

// 	char* cmd;
// 	int running;
// 	int move_result;
// 	int valid_cmd;
// 	//clock_t start, diff;
// 	float seconds;

// 	// Init board
// 	//srand(time(NULL));
// 	clear_grid(grid);
// 	add_value_at_random(grid, 2);
// 	add_value_at_random(grid, 2);

// 	score = 0;
// 	moves = 0;

// 	printf("? <Enter> for help\n");

// 	// Play
// 	valid_cmd = 1;
// 	running   = 1;
// 	//start = clock();
// 	while(running == 1)
// 	{
// 		if(valid_cmd == 1)
// 		{
// 			printf("s: %u\n", score);
// 			print_grid(grid);         
// 		}      
// 		cmd = readline();
// 		if(*cmd == 'q')
// 		{
// 			running     = 0;
// 			move_result = 0;
// 			valid_cmd   = 0;
// 		}
// 		else if(*cmd == 'w')
// 		{
// 			move_result = move_up(grid, &score);
// 			valid_cmd   = 1;
// 		}
// 		else if(*cmd == 's')
// 		{
// 			move_result = move_down(grid, &score);
// 			valid_cmd   = 1;
// 		}
// 		else if(*cmd == 'a')
// 		{
// 			move_result = move_left(grid, &score);
// 			valid_cmd   = 1;
// 		}
// 		else if(*cmd == 'd')
// 		{
// 			move_result = move_right(grid, &score);
// 			valid_cmd   = 1;
// 		}
// 		else if(*cmd == '?')
// 		{
// 			valid_cmd   = 0;
// 			move_result = 0;
// 			printf("Move: [wasd] <Enter>\n");
// 			printf("Quit: q <Enter>\n");
// 		}
// 		else
// 		{
// 			move_result = 0;
// 			valid_cmd   = 0;
// 		}

// 		if(move_result == 1)
// 		{
//             add_value_at_random(grid, 2);
// 			// if(rand() >= (RAND_MAX / 10)) // 10% chance to add 4
// 			// {
// 			// 	add_value_at_random(grid, 2);
// 			// }
// 			// else
// 			// {
// 			// 	add_value_at_random(grid, 4);
// 			// }
// 			moves ++;

// 			if(can_move(grid) == 0)
// 			{
// 				printf("s: %u\n", score);
// 				print_grid(grid);
// 				printf("No Moves Left\n");
// 				running = 0;
// 			}
// 		}

// 	}

// 	//diff = clock() - start;
	
// 	//seconds = (float)(diff / (double)CLOCKS_PER_SEC);
// 	mv = max_value(grid);

// 	printf("Score:          %u\n", score);
// 	printf("Max Value:      %u\n", mv);
// 	//printf("Time:           %.2f seconds\n", seconds);
// 	printf("Moves:          %u\n", moves);
// 	// if(moves > 0)
// 	// {
// 	// 	printf("Sec Per Move:   %.2f\n", seconds / (float)moves);
// 	// }
// 	// if(seconds > 0.01)
// 	// {
// 	// 	printf("Score Per Sec:  %.2f\n", score / seconds);
// 	// }
// 	return 0;
// }


// int mon_debug(int args, char * argc[]){
// 	return 0;
// }

// int mon_read(int argc, char **argv, struct Trapframe *tf){
// 	FIL fil;
// 	FRESULT fr;
// 	fr = f_open(&fil, argv[1], FA_READ);
// 	if (fr) {
// 		printf("Failed to open file!\n\r", 0);
// 		return (int)fr;
// 	}
// 	uint32_t * read_file_buf = 0x89a00000;
// 	char* str = f_gets (read_file_buf, 1024, &fil);
// 	str[1023] = "\0";

// 	if(fil.fsize != 0)
// 		printf(str);
	
// 	if(f_close(&fil))
// 		printf("Failed to close file!\n");
// 	printf("\n");
// 	return 1;
// }

// int mon_write(int argc, char **argv, struct Trapframe *tf){
// 	char *filename = argv[1];
// 	const TCHAR *input = argv[2];
// 	// int fd = my_open(filename);
// 	// if(fd < 0)
// 	// 	return -1;
// 	// my_write(fd, input, FS_BLOCK_SIZE);
// 	FIL fil;
// 	FRESULT fr;
// 	fr = f_open(&fil, argv[1], FA_WRITE);
// 	if (fr) {
// 		printf("Failed to open file!\n\r", 0);
// 		return (int)fr;
// 	}
// 	f_puts(input, &fil);
// 	if(f_close(&fil))
// 		printf("Failed to close file!\n");
// 	return 1;
// }

// char* Int2String(int num,char *str)//10进制 
// {
//     int i = 0;//指示填充str 
//     if(num<0)//如果num为负数，将num变正 
//     {
//         num = -num;
//         str[i++] = '-';
//     } 
//     //转换 
//     do
//     {
//         str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
//         num /= 10;//去掉最低位    
//     }while(num);//num不为0继续循环
    
//     str[i] = '\0';
    
//     //确定开始调整的位置 
//     int j = 0;
//     if(str[0]=='-')//如果有负号，负号不用调整 
//     {
//         j = 1;//从第二位开始调整 
//         ++i;//由于有负号，所以交换的对称轴也要后移1位 
//     }
//     //对称交换 
//     for(;j<i/2;j++)
//     {
//         //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
//         str[j] = str[j] + str[i-1-j];
//         str[i-1-j] = str[j] - str[i-1-j];
//         str[j] = str[j] - str[i-1-j];
//     } 
    
//     return str;//返回转换后的值 
// }
#include "shell.h"
// Simple command-line kernel shell useful for
// controlling the kernel and exploring the system interactively.

//#include <printf.h>
#include <string.h>
// #include <mmu.h>
// #include <uart.h>
#include "ff.h"
// #include <fs.h>
// #include <printf.h>

// #include <console.h>
#include "lib.h"
// #include "syscall_lib.h"
// max size of file image is 16M
#define MAX_FILE_SIZE 0x1000000

// size of DDR RAM (256M for Minisys) 
#define DDR_SIZE 0x10000000

// 4K size read burst
#define SD_READ_SIZE 4096


#define CMDBUF_SIZE	80	// enough for one VGA text line
FATFS FatFs;   // Work area (file system object) for logical drive

struct Command {
    const char *name;
    const char *desc;
    // return -1 to force shell to exit
    int (*func)(int argc, char** argv, struct Trapframe* tf);
};

static struct Command commands[] = {
    { "help", "Display this list of commands", mon_help },
    //{ "kerninfo", "Display information about the kernel", mon_kerninfo },
    //{ "tlb", "Dianoisplay 16 tlb table entries", mon_tlb },
    { "piano", "Simulate a simple electric piano", mon_play },
	//{ "2048", "A simple 2048 game", mon_2048 },
    //{ "game", "A simple MineSweeper game", mon_game },
    { "ls", "List files and directories", mon_ls },
	{ "cd", "Change Directory", mon_cd },
    { "touch", "Create file", mon_touch },
    { "mkdir", "Create directory", mon_mkdir },
	{ "read", "Read a file", mon_read },
	{ "write", "Change a file", mon_write },
	{ "rm", "Delete files or directories", mon_rm } //，
    //{ "about", "Display developers' name of this project", mon_about }
};


/***** Implementations of basic kernel shell commands *****/

int
mon_help(int argc, char **argv, struct Trapframe *tf)
{
    for (int i = 0; i < ARRAY_SIZE(commands); i++)
        {
			//printf("%s - %s\n", commands[i].name, commands[i].desc);
			syscall_printf(commands[i].name);
			syscall_printf(" - ");
			syscall_printf(commands[i].desc);
			syscall_printf("\n");
		}
    return 0;
}

int
mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
{
    // extern char _start[], entry[], etext[], edata[], end[];
    // extern u_long npage;
    // extern Pde *boot_pgdir;
    // printf("Special kernel symbols:\n");
    // printf("    KERNBASE is %x \n",KERNBASE);
    // printf("****Virtual page table**** \n");
    // printf("    VPT is %x \n",VPT);
    // printf("    KSTACKTOP is %x \n",KSTACKTOP);
    // printf("    KSTKSIZE is %d \n",KSTKSIZE);
    // printf("    ULIM is %x \n",ULIM); 
    // printf("    UVPT is %x\n",UVPT);
    // printf("    UPAGES is %x\n",UPAGES);
    // printf("    UENVS is %x\n",UENVS);
    // printf("    UTOP is %x\n",UTOP);
    // printf("    UXSTACKTOP is %x\n",UXSTACKTOP);
    // printf("    TIMESTACK is %x\n",TIMESTACK);
    // printf("    USTACKTOP is %x\n",USTACKTOP);
    // printf("    UTEXT is %x\n",UTEXT);

    // printf("  _start     %x\n", _start);
    // printf("  entry      %x\n", boot_pgdir);
    // printf("  end        %x \n", end);
    // printf(" the count of page is %d\n",npage);
    // printf(" %d bytes to a page \n",BY2PG);
    // printf(" %d bytes mapped by a page directory entry \n",PDMAP);

    return 0;
}

/***** printf tlb info*****/
int
mon_tlb(int argc, char **argv, struct Trapframe *tf)
{
    //print_tlb();
    return 0;
}

/***** stimulate a simple electric piano *****/
int 
mon_play(int argc, char **argv, struct Trapframe *tf)
{	
	// FIL fil;                // File object
	// FRESULT fr;             // FatFs return code
	// uint8_t *boot_file_buf = (uint8_t *)(get_ddr_base()) + DDR_SIZE - MAX_FILE_SIZE; // at the end of DDR space
	// uint8_t *memory_base = (uint8_t *)(get_ddr_base());

	// *boot_file_buf = 47;
	// printf("=======Start to load piano.elf======\n");
	// // Register work area to the default drive
	// // if(f_mount(&FatFs, "", 1)) {
	// // 	printf("Fail to mount SD driver!\n\r", 0);
	// // 	return 1;
	// // }
	
	// // Open a file
	// printf("Loading piano.elf into memory...\n\r", 0);
	// fr = f_open(&fil, "piano.elf", FA_READ);
	// if (fr) {
	// 	printf("Failed to open piano.elf!\n\r", 0);
	// 	return (int)fr;
	// }

	// // Read file into memory
	// uint8_t *buf = boot_file_buf;
	// uint32_t fsize = 0;           // file size count
	// uint32_t br;                  // Read count
	// do {
	// 	if( fsize % 1024 == 0 ) {
	// 	printf("Loading %d KB to memory address \r", fsize / 1024);
	// 	}
	// 	fr = f_read(&fil, buf, SD_READ_SIZE, &br);  // Read a chunk of source file
	// 	buf += br;
	// 	fsize += br;
	// } while(!(fr || br == 0));

	// printf("Load %d bytes to memory address ", fsize);
	// printf("%h from piano.elf \n\r", (uint32_t)boot_file_buf);

	// // read elf
	// if(br = load_elf_sd(boot_file_buf, fil.fsize))
	// 	printf("elf read failed with code %d \n\r", br);

	// // uint32_t *p;
	// // get_entry(boot_file_buf, fil.fsize, p);
	// // printf("elf address : %x", *p);
	
	// // Close the file
	// if(f_close(&fil)) {
	// 	printf("fail to close file!\n\r", 0);
	// 	return 1;
	// }
	
	// // if(f_mount(NULL, "", 1)) {         // unmount it
	// // 	printf("fail to umount disk!\n\r", 0);
	// // 	return 1;
	// // }

	// // spi_disable();
	
	// asm("li $4, 0x10001310");
    // asm("jalr $4");
	
	
    // return 0;
}


/***** A simple MineSweeper game ******/
void menu()
{
	syscall_printf("******************************\n");
	syscall_printf("******1.play     0.exit*******\n");
	syscall_printf("******************************\n");
}
#define MAXFORGAME 10

//打印棋盘
void display(char arr[])
{
    int i=0;
   
    for(;i<10;i++){
        syscall_printf("%c ",arr[10*i+0]);
        syscall_printf("%c ",arr[10*i+1]);
        syscall_printf("%c ",arr[10*i+2]);
        syscall_printf("%c ",arr[10*i+3]);
        syscall_printf("%c ",arr[10*i+4]);
        syscall_printf("%c ",arr[10*i+5]);
        syscall_printf("%c ",arr[10*i+6]);
        syscall_printf("%c ",arr[10*i+7]);
        syscall_printf("%c ",arr[10*i+8]);
        syscall_printf("%c ",arr[10*i+9]);
        syscall_printf("\n");
    }
}

//扫描雷
int  get_boom(char arr[], int x, int y)//计算周围八个位置雷的个数
{
    int num;
    if(x == 1){
        if(y == 1){
            num = arr[10 * x + (y + 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
                  - 3 * '0';
        }else if(y == 9){
            num = arr[10 * x + (y - 1)] + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y]
                  - 3 * '0';
        }else{
            num = arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
                  + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
                  - 5 * '0';
        }
    }else if(x == 9){
        if(y == 1){
            num = arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)] + arr[10 * x + (y + 1)]
                  - 3 * '0';
        }else if(y == 9){
            num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * x + (y - 1)]
                  - 3 * '0';
        }else{
            num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)]
                  + arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
                  - 5 * '0';
        }
    }else{
        if(y == 1){
            num = arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)] + arr[10 * x + (y + 1)]
                  + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
                  - 5 * '0';
        }else if(y == 9){
            num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * x + (y - 1)]
                  + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y]
                  - 5 * '0';
        }else{
            num = arr[10 * (x - 1) + (y - 1)] + arr[10 * (x - 1) + y] + arr[10 * (x - 1) + (y + 1)]
                  + arr[10 * x + (y - 1)] + arr[10 * x + (y + 1)]
                  + arr[10 * (x + 1) + (y - 1)] + arr[10 * (x + 1) + y] + arr[10 * (x + 1) + (y + 1)]
                  - 8 * '0';
        }
    }
    return num;
}

//对某一点进行扩展
void fun(char arr[], char show[], int x, int y)
{
    if ((x >= 1) && (x <= 9) && (y >= 1) && (y <= 9))//递归约束条件
    {
        if (get_boom(arr, x, y) == 0)//判断雷的个数
        {
            show[10 * x + y] = ' ';
            if (show[10 * (x - 1) + (y - 1)] == '*')//对周围八个位置分别进行递归
                fun(arr, show, x - 1, y - 1);
            if (show[10 * (x - 1) + y] == '*')
                fun(arr, show, x - 1, y);
            if (show[10 * (x - 1) + (y + 1)] == '*')
                fun(arr, show, x - 1, y + 1);
            if (show[10 * x + (y - 1)] == '*')
                fun(arr, show, x, y - 1);
            if (show[10 * x + (y + 1)] == '*')
                fun(arr, show, x, y + 1);
            if (show[10 * (x + 1) + (y - 1)] == '*')
                fun(arr, show, x + 1, y - 1);
            if (show[10 * (x + 1) + y] == '*')
                fun(arr, show, x + 1, y);
            if (show[10 * (x + 1) + (y + 1)] == '*')
                fun(arr, show, x + 1, y + 1);
        }
        else
            show[10 * x + y] = get_boom(arr, x, y) + '0';//如果周围有雷则显示雷的个数
        
    }
}


void game()
{
    char arr[100] = {' ','1','2','3','4','5','6','7','8','9',
                     '1','0','0','1','0','0','0','0','0','1',
                     '2','0','0','0','0','0','0','0','0','0',
                     '3','0','0','0','0','0','0','0','0','0',
                     '4','0','0','0','0','0','0','0','0','0',
                     '5','0','0','0','0','0','0','0','1','1',
                     '6','0','0','1','0','0','0','0','0','0',
                     '7','0','1','0','0','0','0','0','0','1',
                     '8','0','1','0','0','0','0','0','1','0',
                     '9','0','1','0','0','0','0','0','0','0'};
    char show[100] = {' ','1','2','3','4','5','6','7','8','9',
                      '1','*','*','*','*','*','*','*','*','*',
                      '2','*','*','*','*','*','*','*','*','*',
                      '3','*','*','*','*','*','*','*','*','*',
                      '4','*','*','*','*','*','*','*','*','*',
                      '5','*','*','*','*','*','*','*','*','*',
                      '6','*','*','*','*','*','*','*','*','*',
                      '7','*','*','*','*','*','*','*','*','*',
                      '8','*','*','*','*','*','*','*','*','*',
                      '9','*','*','*','*','*','*','*','*','*'};
    int win=0;
    int x = 0;
    int y = 0;
    char *xchar;
    char *ychar;

    
    display(show);
    while (win != MAXFORGAME)//判断是否排完雷
    {
        syscall_printf("请输入坐标：\n");
        xchar = syscall_readline("X(row) input> ");
        x = *xchar - '0';
        ychar = syscall_readline("Y(col) input> ");
        y = *ychar - '0';
        syscall_printf("x is %d y is %d\n",x,y);
        if (((x >= 1) && (x <= 9)) && ((y >= 1) && (y <= 9)))
        {
            if (arr[10 * x + y] == '1')
            {
                syscall_printf("踩雷，游戏结束!\n");
                syscall_printf("*****thanks for playing this naive game(#^.^#)*******\n");
                break;
            }
            else
            {
                fun(arr, show, x, y);
                win = 0;
                int i=1;
                
                for (; i <= 9; i++)
                {

                        if (show[10 * i + 0] == '*'){
                            win++;
                        }
                        if (show[10 * i + 1] == '*'){
                            win++;
                        }
                        if (show[10 * i + 2] == '*'){
                            win++;
                        }
                        if (show[10 * i + 3] == '*'){
                            win++;
                        }
                        if (show[10 * i + 4] == '*'){
                            win++;
                        }
                        if (show[10 * i + 5] == '*'){
                            win++;
                        }
                        if (show[10 * i + 6] == '*'){
                            win++;
                        }
                        if (show[10 * i + 7] == '*'){
                            win++;
                        }
                        if (show[10 * i + 8] == '*'){
                            win++;
                        }
                        if (show[10 * i + 9] == '*'){
                            win++;
                        }
 
                }
                display(show);
            }
        }
        else
            syscall_printf("输入坐标有误\n");
    }
    if (win == MAXFORGAME){
        syscall_printf("CONGRATULATIONS!雷阵如下：\n");
        display(arr);
    }
}


int 
mon_game(int argc, char **argv, struct Trapframe *tf)
{
    char *buf;
    menu();
    while (1) {
        buf = syscall_readline("Game> ");
        if (buf != NULL&& *buf!='0')
        {
            game();
           
            break;
        }
        if(*buf=='0')
        {
            syscall_printf("*****thanks for playing this naive game(#^.^#)*******\n");
            break;
        }    

    }
    return 0;
}

/***** File system operations *****/
int
mon_ls(int argc, char **argv, struct Trapframe *tf)
{
	return syscall_ls();
	// FIL fil;
	// FRESULT fr;
	// DIR dir;
	// FILINFO fno;
	// if(f_opendir(&dir, "."))
	// 	syscall_printf("Can not open current directory!\n");
	// while(1)
	// {
	// 	fr = f_readdir(&dir, &fno);
	// 	if(fr != FR_OK || fno.fname[0] == 0) break;
	// 	if(fno.fname[0] == '.') continue;
	// 	if(fno.fattrib & AM_DIR) 
	// 	{
	// 		syscall_printf("\033[1;36;40m");
	// 		syscall_printf(fno.fname);
	// 		syscall_printf("\033[0m");
	// 	}
	// 	else
	// 		syscall_printf(fno.fname);
	// 	syscall_printf(" ");
	// }
	// syscall_printf("\n");
	// return 1;
}

int
mon_touch(int argc, char **argv, struct Trapframe *tf)
{
	return syscall_fcraete(argv[1]);
	// FIL fil;
	// FRESULT fr;
	// fr = f_open(&fil, argv[1], FA_CREATE_ALWAYS);
	// if(fr)
	// 	{
	// 		syscall_printf("Failed to create file <");
	// 		syscall_printf(argv[1]);
	// 		syscall_printf(">\n");
	// 	}
    // return 1;
}

int mon_cd(int argc, char **argv, struct Trapframe *tf)
{
	return syscall_cd(argv[1]);
	// if(f_chdir(argv[1]))
	// 	{
	// 		syscall_printf("Failed to change directory to <");
	// 		syscall_printf(argv[1]);
	// 		syscall_printf(">\n");
	// 	}
    // return 1;
}

int
mon_mkdir(int argc, char **argv, struct Trapframe *tf)
{
	// if(f_mkdir(argv[1]))
	// 	{
	// 		syscall_printf("Failed to make directory <");
	// 		syscall_printf(argv[1]);
	// 		syscall_printf(">\n");
	// 	}
	// return 1;
	// int a = argc;
	// char *s = argv[0];
	// char *k = argv[1];
	// char *q = argv[2];
	// syscall_printf(argv[1]);
	return syscall_mkdir(argv[1]);
}

int
mon_rm(int argc, char **argv, struct Trapframe *tf)
{
	// if(f_unlink(argv[1]))
	// 	{
	// 		syscall_printf("Failed to remove <");
	// 		syscall_printf(argv[1]);
	// 		syscall_printf(">\n");
	// 	}
	// return 1;
	return syscall_rm(argv[1]);
}

/***** Display developers' name of this project *****/
int
mon_about(int argc, char **argv, struct Trapframe *tf)
{
    syscall_printf("******************************************\n");
	syscall_printf("****** Developer List ******\n");
    syscall_printf("******     JIAJUN XIE         *******\n");
    syscall_printf("******       FAN XU           *******\n");
    syscall_printf("******      ZHONG LIU         *******\n");
    syscall_printf("******    YONGTING YOU        *******\n");
    syscall_printf("******     TINGXUAN GU        *******\n");
    syscall_printf("******     JINMIAO SU         *******\n");
    return 0;
}

/***** Kernel shell command interpreter *****/

#define WHITESPACE "\t\n "
#define MAXARGS 16

static int
runcmd(char *buf, struct Trapframe *tf)
{
	//printf("%s\n", "In runcmd");
    int argc;
    char *argv[MAXARGS];
    int i;

    // Parse the command buffer into whitespace-separated arguments
    argc = 0;
    argv[argc] = 0;
    while (1) {
        // gobble whitespace
        while (*buf && strchr(WHITESPACE, *buf))
            *buf++ = 0;
        if (*buf == 0)
            break;

        // save and scan past next arg
        if (argc == MAXARGS-1) {
            syscall_printf("Too many arguments (max %d)\n", MAXARGS);
            return 0;
        }
        argv[argc++] = buf;
        while (*buf && !strchr(WHITESPACE, *buf))
            buf++;
    }
    argv[argc] = 0;

    // Lookup and invoke the command
    if (argc == 0)
        return 0;
    for (i = 0; i < ARRAY_SIZE(commands); i++) {
        if (strcmp(argv[0], commands[i].name) == 0)
            return commands[i].func(argc, argv, tf);
    }
    syscall_printf("Unknown command '%s'\n", argv[0]);
    return 0;
}

void
shell(struct Trapframe *tf)
{
    char *buf;

    syscall_printf("Aurora, an operating system based on MIPS32\n");
    syscall_printf("Type 'help' for more commands.\n");


    while (1) {
        buf = syscall_readline("Aurora> ");
        if (buf != NULL)
            if (runcmd(buf, tf) < 0)
                break;
    }
}

#define GRID_WIDTH  4
#define GRID_HEIGHT 4
#define GRID_SIZE   (GRID_WIDTH * GRID_HEIGHT)

static void clear_grid(unsigned int * grid)
{
	int i;
	for(i = 0; i < GRID_SIZE; i++)
	{
		grid[i] = 0;
	}
}

static void print_grid(unsigned int * grid)
{
	int i, x, y;
	int size=0;
	int bigest_size;
	char tmp_buff[16];

	// Try to get the largest sized number
	bigest_size = 1;
	for(i = 0; i < GRID_SIZE; i++)
	{
		//size = sprintf(tmp_buff, "%u", grid[i]);
        int temp=grid[i];
        while(temp!=0)
        {
            temp/=10;
            size++;
        }

		if(size > bigest_size)
		{
			bigest_size = size;
		}
        size=0;
	}

	// put everything onto the screen
	x = 0;
	y = 0;
	for(i = 0; i < GRID_SIZE; i++)
	{
		if(grid[i] == 0)
		{
			//printf("[%*s] ", bigest_size, "");
			syscall_printf("[%5s] ", " ");
		}
		else
		{
			char temp_str[16]={0};
			int temp_num=grid[i];
			Int2String(temp_num,temp_str);
			//printf("[%*u] ", bigest_size, grid[i]);
			syscall_printf("[%5s] ", temp_str);
		}

		x ++;
		if(x >= GRID_WIDTH)
		{
			x = 0;
			y ++;
			syscall_printf("\n");
		}
	}
}

static void add_value_at_random(unsigned int * grid, unsigned int value)
{
	int free_cells;
	int spots_left;
	int i;
	int rand_num;

	int target_cell, cell;

	free_cells = 0;
	for(i = 0; i < GRID_SIZE; i++)
	{
		if(grid[i] == 0)
		{
			free_cells ++;
		}
	}

	if(free_cells > 0)
	{
		// target_cell = rand() % free_cells;
        target_cell = free_cells-1;
		cell = 0;
		for(i = 0; i < GRID_SIZE; i++)
		{
			if(grid[i] == 0)
			{
				if(cell == target_cell)
				{
					grid[i] = value;
					break;
				}
				else
				{
					cell ++;
				}
			}
		}
	}
}

static int update_cell(unsigned int * grid, 
							  int num_index, 
							  int end_index, 
							  int delta_index, 
							  unsigned int * score)
{
	int i;
	int first_num_index, second_num_index;
	int valid_move;
	unsigned int value;

	if(grid[num_index] == 0)
	{
		first_num_index = -1;
	}
	else
	{
		first_num_index = num_index;
	}
	second_num_index = -1;

	i = num_index;
	while(i != end_index && second_num_index < 0)
	{

		i += delta_index;
		if(grid[i] != 0)
		{
			if(first_num_index < 0)
			{
				first_num_index = i;
			}
			else if(second_num_index < 0)
			{
				second_num_index = i;
			}
		}

	}

	

	// Move numbers
	if(first_num_index  >= 0 &&
		second_num_index >= 0)
	{
		if(grid[first_num_index] == grid[second_num_index])
		{
			value = grid[first_num_index] + grid[second_num_index];
			grid[first_num_index]  = 0;
			grid[second_num_index] = 0;
			grid[num_index]        = value;
			valid_move             = 1;
			if(score != NULL)
			{
				(*score) += value;
			}
		}
		else
		{
			value                 = grid[first_num_index];
			grid[first_num_index] = 0;
			grid[num_index]       = value;
			if(first_num_index != num_index)
			{
				valid_move = 1;
			}
			else
			{
				valid_move = 0;
			}
		}
	}
	else if(first_num_index >= 0)
	{
		value                 = grid[first_num_index];
		grid[first_num_index] = 0;
		grid[num_index]       = value;
		valid_move            = 1;
		if(first_num_index != num_index)
		{
			valid_move = 1;
		}
		else
		{
			valid_move = 0;
		}

	}
	else
	{
		valid_move = 0;
	}

	return valid_move;
}

static int move_up(unsigned int * grid, unsigned int * score)
{
	int i1, i2, x, y, end_index;
	int valid_move;

	valid_move = 0;
	i1 = 0;
	for(x = 0; x < GRID_WIDTH; x++)
	{
		i2 = i1;
		end_index = i1 + (GRID_SIZE - GRID_WIDTH);
		for(y = 0; y < GRID_HEIGHT; y++)
		{
			if(update_cell(grid, i2, end_index, GRID_WIDTH, score) == 1)
			{
				valid_move = 1;
			}  

			i2 += GRID_WIDTH;
		}
		i1 ++;
	}

	return valid_move;
}

static int move_down(unsigned int * grid, unsigned int * score)
{
	int i1, i2, x, y, end_index;
	int valid_move;

	valid_move = 0;
	i1 = GRID_SIZE - GRID_WIDTH;
	for(x = 0; x < GRID_WIDTH; x++)
	{
		i2 = i1;
		end_index = i1 - (GRID_SIZE - GRID_WIDTH);
		for(y = 0; y < GRID_HEIGHT; y++)
		{
			if(update_cell(grid, i2, end_index, -GRID_WIDTH, score) == 1)
			{
				valid_move = 1;
			}  
			i2 -= GRID_WIDTH;
		}
		i1 ++;
	}

	return valid_move;
}

static int move_left(unsigned int * grid, unsigned int * score)
{
	int i1, i2, x, y, end_index;
	int valid_move;

	valid_move = 0;
	i1 = 0;
	for(y = 0; y < GRID_HEIGHT; y++)
	{
		i2 = i1;
		end_index = i1 + (GRID_WIDTH - 1);
		for(x = 0; x < GRID_WIDTH; x++)
		{
			if(update_cell(grid, i2, end_index, 1, score) == 1)
			{
				valid_move = 1;
			}  

			i2 ++;
		}
		i1 += GRID_WIDTH;
	}

	return valid_move;
}

static int move_right(unsigned int * grid, unsigned int * score)
{
	int i1, i2, x, y, end_index;
	int valid_move;

	valid_move = 0;
	i1 = GRID_WIDTH - 1;
	for(y = 0; y < GRID_HEIGHT; y++)
	{
		i2 = i1;
		end_index = i1 - (GRID_WIDTH - 1);
		for(x = 0; x < GRID_WIDTH; x++)
		{
			if(update_cell(grid, i2, end_index, -1, score) == 1)
			{
				valid_move = 1;
			}  

			i2 --;
		}
		i1 += GRID_WIDTH;
	}

	return valid_move;
}


// Returns 1 if a move is avaliable
static int can_move(unsigned int * grid)
{
	int i, x, y;
	int result;

	result = 0;
	x      = 0;
	y      = 0;
	for(i = 0; i < GRID_SIZE; i++)
	{
		// Check for an empty cell
		if(grid[i] == 0)
		{
			result = 1;
			break;
		}
		
		// Check to see if the cell to the right matches
		if(x < GRID_WIDTH - 1)
		{
			if(grid[i] == grid[i + 1])
			{
				result = 1;
				break;
			}
		}

		// Check to see if the cell to the left matches
		if(y < GRID_HEIGHT - 1)
		{
			if(grid[i] == grid[i + GRID_WIDTH])
			{
				result = 1;
				break;
			}
		}

		// Update x and y
		x ++;
		if(x >= GRID_WIDTH)
		{
			x = 0;
			y ++;
		}
	}

	return result;
}

unsigned int max_value(unsigned int * grid)
{
	int i;
	unsigned int value;
	value = 0;
	for(i = 0; i < GRID_SIZE; i++)
	{
		if(grid[i] > value)
		{
			value = grid[i];
		}
	}
	return value;
}

int mon_2048(int args, char * argc[])
{
	unsigned int grid[GRID_SIZE];
	unsigned int score;
	unsigned int moves;
	unsigned int mv;

	char* cmd;
	int running;
	int move_result;
	int valid_cmd;
	//clock_t start, diff;
	float seconds;

	// Init board
	//srand(time(NULL));
	clear_grid(grid);
	add_value_at_random(grid, 2);
	add_value_at_random(grid, 2);

	score = 0;
	moves = 0;

	syscall_printf("? <Enter> for help\n");

	// Play
	valid_cmd = 1;
	running   = 1;
	//start = clock();
	while(running == 1)
	{
		if(valid_cmd == 1)
		{
			syscall_printf("s: %u\n", score);
			print_grid(grid);         
		}      
		cmd = syscall_readline("");
		if(*cmd == 'q')
		{
			running     = 0;
			move_result = 0;
			valid_cmd   = 0;
		}
		else if(*cmd == 'w')
		{
			move_result = move_up(grid, &score);
			valid_cmd   = 1;
		}
		else if(*cmd == 's')
		{
			move_result = move_down(grid, &score);
			valid_cmd   = 1;
		}
		else if(*cmd == 'a')
		{
			move_result = move_left(grid, &score);
			valid_cmd   = 1;
		}
		else if(*cmd == 'd')
		{
			move_result = move_right(grid, &score);
			valid_cmd   = 1;
		}
		else if(*cmd == '?')
		{
			valid_cmd   = 0;
			move_result = 0;
			syscall_printf("Move: [wasd] <Enter>\n");
			syscall_printf("Quit: q <Enter>\n");
		}
		else
		{
			move_result = 0;
			valid_cmd   = 0;
		}

		if(move_result == 1)
		{
            add_value_at_random(grid, 2);
			// if(rand() >= (RAND_MAX / 10)) // 10% chance to add 4
			// {
			// 	add_value_at_random(grid, 2);
			// }
			// else
			// {
			// 	add_value_at_random(grid, 4);
			// }
			moves ++;

			if(can_move(grid) == 0)
			{
				syscall_printf("s: %u\n", score);
				print_grid(grid);
				syscall_printf("No Moves Left\n");
				running = 0;
			}
		}

	}

	//diff = clock() - start;
	
	//seconds = (float)(diff / (double)CLOCKS_PER_SEC);
	mv = max_value(grid);

	syscall_printf("Score:          %u\n", score);
	syscall_printf("Max Value:      %u\n", mv);
	//printf("Time:           %.2f seconds\n", seconds);
	syscall_printf("Moves:          %u\n", moves);
	// if(moves > 0)
	// {
	// 	printf("Sec Per Move:   %.2f\n", seconds / (float)moves);
	// }
	// if(seconds > 0.01)
	// {
	// 	printf("Score Per Sec:  %.2f\n", score / seconds);
	// }
	return 0;
}


int mon_debug(int args, char * argc[]){
	return 0;
}

int mon_read(int argc, char **argv, struct Trapframe *tf){
	// FIL fil;
	// FRESULT fr;
	// fr = f_open(&fil, argv[1], FA_READ);
	// if (fr) {
	// 	syscall_printf("Failed to open file!\n\r", 0);
	// 	return (int)fr;
	// }
	// uint32_t * read_file_buf = 0x89a00000;
	// char* str = f_gets (read_file_buf, 1024, &fil);
	// str[1023] = "\0";

	// if(fil.fsize != 0)
	// 	syscall_printf(str);
	
	// if(f_close(&fil))
	// 	syscall_printf("Failed to close file!\n");
	// syscall_printf("\n");
	// return 1;
	return syscall_fread(argv[1]);
}

int mon_write(int argc, char **argv, struct Trapframe *tf){
	// char *filename = argv[1];
	// const TCHAR *input = argv[2];
	// // int fd = my_open(filename);
	// // if(fd < 0)
	// // 	return -1;
	// // my_write(fd, input, FS_BLOCK_SIZE);
	// FIL fil;
	// FRESULT fr;
	// fr = f_open(&fil, argv[1], FA_WRITE);
	// if (fr) {
	// 	syscall_printf("Failed to open file!\n\r", 0);
	// 	return (int)fr;
	// }
	// f_puts(input, &fil);
	// if(f_close(&fil))
	// 	syscall_printf("Failed to close file!\n");
	// return 1;
	return syscall_fwrite(argv[1], argv[2]);
}

char* Int2String(int num,char *str)//10进制 
{
    int i = 0;//指示填充str 
    if(num<0)//如果num为负数，将num变正 
    {
        num = -num;
        str[i++] = '-';
    } 
    //转换 
    do
    {
        str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
        num /= 10;//去掉最低位    
    }while(num);//num不为0继续循环
    
    str[i] = '\0';
    
    //确定开始调整的位置 
    int j = 0;
    if(str[0]=='-')//如果有负号，负号不用调整 
    {
        j = 1;//从第二位开始调整 
        ++i;//由于有负号，所以交换的对称轴也要后移1位 
    }
    //对称交换 
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//返回转换后的值 
}