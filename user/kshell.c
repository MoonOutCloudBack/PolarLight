// #include "kshell.h"
// #include <string.h>
// #include <mmu.h>
// #include <../drivers/uart.h>
// #include <../fs/fs.h>
// #include <fs.h>
// #include <printf.h>

// #include <../drivers/console.h>
// #include <tlbop.h>
#include <../user/lib.h>

#define ENV_HELP 0x88010000

struct Command
{
	const char *name;
	const char *desc;
	// return -1 to force shell to exit
	int (*func)(int argc, char **argv, struct Trapframe *tf);
};

void k_help(int argc, char **argv, struct Trapframe *tf)
{
	syscall_env_create(ENV_HELP);
    printf("help\n");
}
static struct Command commands[] = {
	{"help", "Display this list of commands", k_help},
	// {"kerninfo", "Display information about the kernel", k_kerninfo},
	// {"tlb", "Dianoisplay 16 tlb table entries", k_tlb},
	// {"piano", "Simulate a simple electric piano", k_play},
	// {"2048", "A simple 2048 game", k_2048},
	// //{ "game", "A simple MineSweeper game", k_game },
	// {"ls", "List files and directories", k_ls},
	// {"cd", "Change Directory", k_cd},
	// {"touch", "Create file", k_touch},
	// {"mkdir", "Create directory", k_mkdir},
	// {"read", "Read a file", k_read},
	// {"write", "Change a file", k_write},
	// {"rm", "Delete files or directories", k_rm},
	// //{ "debug", "我们遇到什么bug, 也不要怕！ 微笑着面对它！ 消除bug的最好办法就是面对bug！ 坚持才是胜利！ 加油， ***！", k_debug },
	// {"about", "Display developers' name of this project", k_about}

};

/***** Kernel shell command interpreter *****/

#define WHITESPACE_ "\t\n "
#define MAXARGS_ 16

static int
runcmd(char *buf, struct Trapframe *tf)
{
	//printf("%s\n", "In runcmd");
	int argc;
	char *argv[MAXARGS_];
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1)
	{
		// gobble whitespace
		while (*buf && strchr(WHITESPACE_, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		// save and scan past next arg
		if (argc == MAXARGS_ - 1)
		{
			printf("Too many arguments (max %d)\n", MAXARGS_);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE_, *buf))
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0)
		return 0;
	for (i = 0; i < ARRAY_SIZE(commands); i++)
	{
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv, tf);
	}
	printf("Unknown command '%s'\n", argv[0]);
	return 0;
}

void kshell(struct Trapframe *tf)
{
    char *buf;
	
	syscall_printf("Aurora, an operating system based on MIPS32\n");
	syscall_printf("Type 'help' for more commands.\n");

	while (1)	
	{
		buf = syscall_readline("$Aurora> ");
		if (buf != NULL)
			{
				if (runcmd(buf, tf) < 0)
				{
					break;
				}
			}
		else self_kill();

	}
	
}

void self_kill()
{
	u_int curenv_id =  syscall_getenvid();
	syscall_env_destroy(curenv_id);
	syscall_yield();
}
