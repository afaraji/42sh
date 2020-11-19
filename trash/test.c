/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 22:00:18 by sazouaka          #+#    #+#             */
/*   Updated: 2020/10/29 23:56:26 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

// typedef	struct			s_process
// {
// 	char				**argv;
// 	pid_t				pid;
// 	struct s_process	*next;
// }						process;

// typedef	struct			s_job
// {
// 	int					stdin;
// 	int					stdout;
// 	int					stderr;
// 	process				*first_process;
// 	pid_t				pgid;
// 	struct s_job		*next;
// }						job;

// int		format_job_info (job *j, char *str)
// {
// 	printf("job [%d] has %s\n", j->pgid, str);
// 	return (0);
// }

// void	launch_process (process *p, pid_t pgid, int infile, int outfile, int errfile, int foreground)
// {
// 	pid_t	pid;
// 	int		shell_terminal = 0;

// 	if (1)//is_ineractive
// 	{
// 	  /* Put the process into the process group and give the process group
// 		 the terminal, if appropriate.
// 		 This has to be done both by the shell and in the individual
// 		 child processes because of potential race conditions.  */
// 		pid = getpid ();
// 		if (pgid == 0)
// 			pgid = pid;
// 		setpgid (pid, pgid);
// 		if (foreground)
// 			tcsetpgrp (shell_terminal, pgid);

// 	  /* Set the handling for job control signals back to the default.  */
// 		signal (SIGINT, SIG_DFL);
// 		signal (SIGQUIT, SIG_DFL);
// 		signal (SIGTSTP, SIG_DFL);
// 		signal (SIGTTIN, SIG_DFL);
// 		signal (SIGTTOU, SIG_DFL);
// 		signal (SIGCHLD, SIG_DFL);
// 	}

//   /* Set the standard input/output channels of the new process.  */
// 	if (infile != STDIN_FILENO)
// 	{
// 		dup2 (infile, STDIN_FILENO);
// 		close (infile);
// 	}
// 	if (outfile != STDOUT_FILENO)
// 	{
// 		dup2 (outfile, STDOUT_FILENO);
// 		close (outfile);
// 	}
// 	if (errfile != STDERR_FILENO)
// 	{
// 		dup2 (errfile, STDERR_FILENO);
// 		close (errfile);
// 	}

// 	/* Exec the new process.  Make sure we exit.  */
// 	execvp (p->argv[0], p->argv);
// 	perror ("execvp");
// 	exit (1);
// }

// void	launch_job (job *j, int foreground)
// {
// 	process	*p;
// 	pid_t	pid;
// 	int		shell_is_interactive = 0;
// 	int		mypipe[2], infile, outfile;

// 	infile = j->stdin;
// 	p = j->first_process;
// 	while (p)
// 	{
// 		/* Set up pipes, if necessary.  */
// 		if (p->next)
// 		{
// 			if (pipe (mypipe) < 0)
// 			{
// 				perror ("pipe");
// 				exit (1);
// 			}
// 			outfile = mypipe[1];
// 		}
// 		else
// 			outfile = j->stdout;

// 		/* Fork the child processes.  */
// 		pid = fork ();
// 		if (pid == 0)
// 		/* This is the child process.  */
// 			launch_process (p, j->pgid, infile,	outfile, j->stderr, foreground);
// 		else if (pid < 0)
// 		{
// 			/* The fork failed.  */
// 			perror ("fork");
// 			exit (1);
// 		}
// 		else
// 		{
// 			/* This is the parent process.  */
// 			p->pid = pid;
// 			if (1)//is_interactive
// 			{
// 				if (!j->pgid)
// 					j->pgid = pid;
// 				setpgid (pid, j->pgid);
// 			}
// 		}

// 	  /* Clean up after pipes.  */
// 		if (infile != j->stdin)
// 			close (infile);
// 		if (outfile != j->stdout)
// 			close (outfile);
// 		infile = mypipe[0];
// 		p = p->next;
// 	}

// 	format_job_info (j, "launched");

// 	if (!shell_is_interactive)
// 		wait_for_job (j);
// 	else if (foreground)
// 		put_job_in_foreground (j, 0);
// 	else
// 		put_job_in_background (j, 0);
// }

int		chr_index(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if(s[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int		get_opt_str(int opt[], char *s, char *opt_list)
{
	int	j;
	int	pos;

	j = 1;
	while (s[j])
	{
		if ((pos = chr_index(opt_list, s[j])) < 0)
		{
			opt[0] = s[j];
			return (-1);
		}
		opt[pos] = 1;
		j++;
	}
	return (1);
}

int		get_opt(char **av, int opt[], char *opt_list, int *index)
{
	int		i;
	int		j;

	i = 0;
	while (i < strlen(opt_list))
		opt[i++] = 0;
	i = 1;
	j = -1;
	while (av[i] && (av[i][0] == '-' && av[i][1]))
	{
		j = 0;
		if (get_opt_str(opt, av[i], opt_list) == -1)
			return (-1);
		i++;
	}
	*index = i;
	if (j == -1)
		return (0);
	return (1);
}

int			main(int ac, char **av, char **env)
{
	int	ret;
	int	opt[4];
	char *str = "lphg";
	int	i;

	ret = get_opt(av, opt, str, &i);
	printf("ret[%d]\n", ret);
	if (ret == -1)
	{
		printf ("invalid option -- [%c]\n", opt[0]);
		return (-1);
	}
	if (ret == 0)
		printf ("no options i = [%d]\n", i);
	else if (ret > 0)
	{
		printf ("option in [%s][%d|%d]\n", str, opt[0], opt[1]);
	}
	while (av[i])
	{
		printf("%d:%s\n", i, av[i]);
		i++;
	}

	return (0);
}
