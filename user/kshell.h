#ifndef KSHELL_H
#define KSHELL_H

struct Trapframe;
void kshell(struct Trapframe *tf);
char *k_readline(const char *prompt);

// Functions implementing shell commands.
int k_help(int argc, char **argv, struct Trapframe *tf);
int k_kerninfo(int argc, char **argv, struct Trapframe *tf);
int k_play(int argc, char **argv, struct Trapframe *tf);
int k_game(int argc, char **argv, struct Trapframe *tf);
int k_tlb(int argc, char **argv, struct Trapframe *tf);
int k_ls(int argc, char **argv, struct Trapframe *tf);
int k_touch(int argc, char **argv, struct Trapframe *tf);
int k_mkdir(int argc, char **argv, struct Trapframe *tf);
int k_rm(int argc, char **argv, struct Trapframe *tf);
int k_about(int argc, char **argv, struct Trapframe *tf);
int k_2048(int args, char * argc[]);
int k_debug(int args, char * argc[]);
int k_cd(int argc, char **argv, struct Trapframe *tf);
int k_read(int argc, char **argv, struct Trapframe *tf);
int k_write(int argc, char **argv, struct Trapframe *tf);

#endif