#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


// int	main(int argc, char *argv[])
// {
// 	pid_t cpid, w;
// 	int status;

// 	cpid = fork();
// 	if (cpid == -1) {
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}

// 	if (cpid == 0)
// 	{            /* Code executed by child */
// 		printf("Child PID is %ld\n", (long) getpid());
// 		if (argc == 1)
// 			pause();                    /* Wait for signals */
// 		_exit(atoi(argv[1]));
// 	}
// 	else
// 	{                    /* Code executed by parent */
// 		do
// 		{
// 			w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
// 			if (w == -1)
// 			{
// 				perror("waitpid");
// 				exit(EXIT_FAILURE);
// 			}
// 			if (WIFEXITED(status))
// 			{
// 				printf("exited, status=%d\n", WEXITSTATUS(status));
// 			}
// 			else if (WIFSIGNALED(status))
// 			{
// 				printf("killed by signal %d\n", WTERMSIG(status));
// 			}
// 			else if (WIFSTOPPED(status))
// 			{
// 				printf("stopped by signal %d\n", WSTOPSIG(status));
// 			}
// 			else if (WIFCONTINUED(status))
// 			{
// 				printf("continued\n");
// 			}
// 		}
// 		while (!WIFEXITED(status) && !WIFSIGNALED(status));
// 		exit(EXIT_SUCCESS);
// 	}
// }

// int		main(int ac, char **av)
// {
// 	int	sleep_time = 0;
// 	int	ret = 0;

// 	if (ac != 3)
// 	{
// 		printf("usage: ./a.out [sleep in sec] [exit value]\n");
// 		return (-1);
// 	}
// 	sleep_time = atoi(av[1]);
// 	ret = atoi(av[2]);
// 	sleep(sleep_time);
// 	av[10000] = "str";
// 	return (ret);
// }

int		if_system_failure(int ret, int failure_value)
{
	extern const char *const	sys_errlist[];
	extern int					errno;

	if (ret != failure_value)
		return (0);
	printf("System call failure: %s\n", sys_errlist[errno]);
	exit(42);
}

int		main(void)
{
	char	*s;
	int		i;

	if_system_failure(wait(&i), -1);
	return (0);
}
