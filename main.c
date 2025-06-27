#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/reg.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#define GREEN "\x1b[32m"
#define CYAN "\x1b[36m"
#define YELLOW "\x1b[33m"
#define RESET "\x1b[0m"

const char	*syscall_name(long n);
void	run_target(const char *fname);
void	run_tracer(pid_t pid);

int	main(int argc, char **argv)
{
	pid_t	pid;

	if (argc != 2)
	{
		fprintf(stderr, "%s: Enter a program name\n", argv[0]);
		return (1);
	}

	pid = fork();
	if (pid == 0)
		run_target(argv[1]);	
	else if (pid > 0)
		run_tracer(pid);
	return (0);
}

void	run_target(const char *fname)
{
	if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0)
	{
		perror("ptrace");
		return ;
	}
	execl(fname, fname, 0);
}

void	run_tracer(pid_t pid)
{
	int						status;
	int						in_syscall;
	int						count;
	struct user_regs_struct	regs;

	printf("TRACER START\n");
	waitpid(pid, NULL, 0);
	if (WIFEXITED(status))
		return;

	ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);
	while (1)
	{
		ptrace(PTRACE_SYSCALL, pid, 0, 0);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			break;

		if (WIFSTOPPED(status) && (WSTOPSIG(status) == (SIGTRAP | 0x80)))
		{
			ptrace(PTRACE_GETREGS, pid, 0, &regs);
			if (!in_syscall)
			{
				long n = regs.orig_rax;
				if (strcmp(syscall_name(n), "other") != 0)
				{
					printf("\n");
					printf(GREEN ">>> syscall: %-8s" RESET "(%ld)\n", syscall_name(n), n);
					printf(CYAN "rdi" RESET " =   0x%016llx\n", regs.rdi);
					printf(CYAN "rsi" RESET " =   0x%016llx\n", regs.rsi);
					printf(CYAN "rdx" RESET " =   0x%016llx\n", regs.rdx);
					in_syscall = 1;
				}
				else	
					in_syscall = 0;
			}
			else if (in_syscall)
			{
				printf(YELLOW "return" RESET ": 0x%llx\n", regs.rax);
				in_syscall = 0;
			}
		}
	}
}

const char	*syscall_name(long n)
{
	    switch (n)
		{
        case SYS_read: return "read";
        case SYS_write: return "write";
        case SYS_open: return "open";
        case SYS_close: return "close";
        case SYS_execve: return "execve";
        case SYS_brk: return "brk";
        case SYS_mmap: return "mmap";
        case SYS_munmap: return "munmap";
        case SYS_mremap: return "mremap";
        case SYS_openat: return "openat";
        case SYS_pipe: return "pipe";
        case SYS_dup: return "dup";
        case SYS_dup2: return "dup2";
        case SYS_dup3: return "dup3";
        default: return "other";
		}
}
