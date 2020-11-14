#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"


/* NOTE: This example illustrates tcsetgrp() and setpgrp(), but doesn’t function correctly because SIGTTIN and SIGTTOU aren’t handled.*/
int main() 
{
    int status;
    int cpid;
    int ppid;
    char buf[256];
    sigset_t blocked;

    ppid = getpid();
    if (!(cpid=fork()))
    {
        setpgid(0,0);
        tcsetpgrp (0, getpid());
        execl ("/usr/bin/vim", "vim", NULL);
        exit (-1);
    }
    if (cpid < 0)
        exit(-1);
    setpgid(cpid, cpid);
    tcsetpgrp (0, cpid);
    waitpid (cpid, NULL, 0);
    tcsetpgrp (0, ppid);
    while (1)
    {
        memset (buf, 0, 256);
        fgets (buf, 256, stdin);
        puts ("ECHO: ");
        puts (buf);
        puts ("\n");
    }
}