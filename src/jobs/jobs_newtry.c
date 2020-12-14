/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 20:44:09 by afaraji           #+#    #+#             */
/*   Updated: 2020/11/14 20:44:14 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/jobs.h"

pid_t	g_current_job = 0;
pid_t	g_previous_job = 0;
FILE	*tty = NULL;

char	*ft_getsigstr1_12(int sig)
{
	static char *str[13];
	if (!str[0])
	{
		str[0] = "done";
		str[1] = "hangup";
		str[2] = "Interrupt";
		str[3] = "Quit";
		str[4] = "Illegal instruction";
		str[5] = "trace trap";
		str[6] = "abort";
		str[7] = "EMT trap";
		str[8] = "floating point exception";
		str[9] = "killed";
		str[10] = "bus error";
		str[11] = "segmentation fault";
		str[12] = "Bad system call";
	}
	if (sig >= 0 && sig <= 12)
		return (str[sig]);
	return (NULL);
}

char	*ft_getsigstr13_31(int sig)
{
	static char *str[20];
	if (!str[0])
	{
		str[0] = "broken pipe";	//13
		str[1] = "Alarm clock";	//14
		str[2] = "terminated";	//15
		str[4] = "stoped";	//17
		str[5] = "stoped";	//18
		str[8] = "stoped";	//21
		str[9] = "stoped";	//22
		str[11] = "Cputime limit exceeded";	//24
		str[12] = "Filesize limit exceeded"; //25
		str[13] = "Virtual timer expired"; //26
		str[14] = "Profiling timer expired"; //27
		str[17] = "User defined signal 1"; //30
		str[18] = "User defined signal 2"; //31
	}
	if (sig >= 13 && sig <= 31)
		return (str[sig - 13]);
	return (NULL);
}
char	*ft_strsignal(int sig)
{
	if (sig == 16 || sig == 19 || sig == 20 || sig == 23 || sig == 28 ||
																	sig == 29)
		return (NULL);
	if (sig <= 12)
		return (ft_getsigstr1_12(sig));
	return (ft_getsigstr13_31(sig));
}

int		killed_by(int sig)//not used yet
{
	ft_print(2, "killed func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}

int		stopped_by(int sig)//not used yet
{
	ft_print(2, "stopped func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}



char	*io_redirect_to_str(t_io_redirect *io)
{
	char	*typ;
	char	*num;
	char	*str;

	typ = tokentoa(io->redirect_type);
	num = ft_itoa(io->io_num);
	str = ft_4strjoin(num, typ, io->filename, "");
	ft_strdel(&num);
	return (str);
}

char	*args_to_str2(t_cmd_suffix *suff, char *s)
{
	char	*str;
	char	*tmp;
	char	*tmp2;

	str = s;
	while (suff)
	{
		if (suff->word)
			tmp = ft_strjoin(str, suff->word);
		else if (suff->io_redirect)
		{
			tmp2 = io_redirect_to_str(suff->io_redirect);
			tmp = ft_strjoin(str, tmp2);
			ft_strdel(&tmp2);
		}
		ft_strdel(&str);
		str = tmp;
		suff = suff->suffix;
	}
	return (str);
}

char	*args_to_str(t_cmd_suffix *suff, t_cmd_prefix *pref)
{
	char	*str;
	char	*tmp;
	char	*tmp2;

	str = ft_strdup(" ");
	while (pref)
	{
		if (pref->io_redirect)
			tmp2 = io_redirect_to_str(pref->io_redirect);
		else if (pref->ass_word)
			tmp2 = ft_4strjoin(pref->ass_word->key, "=", pref->ass_word->value, "");
		tmp = ft_4strjoin(str, " ",tmp2, " ");
		ft_strdel(&str);
		ft_strdel(&tmp2);
		str = tmp;
		pref = pref->prefix;
	}
	return (args_to_str2(suff, str));
}

char	*simple_cmd_to_str(t_simple_cmd *cmd)
{
	char	*name;
	char	*str;
	char	*args;

	if (cmd->name || cmd->word)
		name = (cmd->name) ? cmd->name : cmd->word;
	else
		name = "";
	args = args_to_str(cmd->suffix, cmd->prefix);
	str = ft_strjoin(name, args);
	ft_strdel(&args);
	return (str);
}

char	*ast_to_str(t_pipe_seq *cmd)//verify leak
{
	char	*tmp;
	char	*args;
	char	*name;
	char	*str;
	char	*pipe;

	str = ft_strdup("");
	while (cmd)
	{
		name = (cmd->left->name) ? cmd->left->name : cmd->left->word;
		name = (name) ? name : "";
		args = args_to_str(cmd->left->suffix, cmd->left->prefix);
		pipe = (cmd->right) ? " | " : "";
		tmp = ft_4strjoin(str, name, args, pipe);
		ft_strdel(&str);
		ft_strdel(&args);
		str = tmp;
		cmd = cmd->right;
	}
	return (str);
}

char	*and_or_to_str(t_and_or *cmd)
{
	char	*ast;
	char	*token;
	char	*str;
	char	*tmp;

	str = ft_strdup("");
	while (cmd)
	{
		ast = ast_to_str(cmd->ast);
		if (cmd->dependent)
			token = (cmd->dependent == 1) ? " && " : " || ";
		else
			token = "";
		tmp = ft_4strjoin(str, token, ast, "");
		ft_strdel(&str);
		ft_strdel(&ast);
		str = tmp;
		cmd = cmd->next;
	}
	return (str);
}

/******************************* jobs begin ***********************************/



/*
int		print_job(t_job *j, int foreground)
{
	char	**av;
	t_process *p;
	int k;

	for (p = j->first_process, k = 0; p; k++, p = p->next)
	{
		printf("***[%d]***** P N:%d [%d|%d|%d|%d] ********\n", foreground, k, p->pid, p->completed, p->status, p->stopped);
		av = p->argv;
		printf("-------argv-------\n");
		for (int i = 0; av && av[i]; i++)
			printf("%d[%s]\n", i, av[i]);
		av = p->env;
		printf("-------env-------\n");
		for (int i = 0; av && av[i]; i++)
			printf("%d[%s]\n", i, av[i]);
	}
	return (0);
}
*/

int			get_new_index(void)
{
	t_job	*node;
	int		index;

	node = g_var.job;
	index = 1;
	while (node)
	{
		index = (node->index >= index) ? node->index + 1 : index;
		node = node->next;
	}
	return (index);
}

t_process	*get_process(t_simple_cmd *cmd)
{
	t_process	*p;

	p = (t_process *)ft_malloc(sizeof(t_process));
	p->argv = get_arg_var_sub(cmd);
	p->env = NULL; //env_to_tab(g_var.var, 0);
	p->pid = 0;
	p->completed = 0;
	p->stopped = 0;
	p->status = 0;
	p->cmd = cmd;
	p->command = simple_cmd_to_str(cmd);
	p->next = NULL;
	return (p);
}

t_process	*get_first_proc(t_pipe_seq *cmd)
{
	t_process	*head;
	t_process	*node;

	head = NULL;
	while (cmd)
	{
		if (!head)
		{
			head = get_process(cmd->left);
			node = head;
		}
		else
		{
			node->next = get_process(cmd->left);
			node = node->next;
		}
		cmd = cmd->right;
	}
	return (head);
}

t_job	*get_job(t_and_or *cmd)
{
	t_job	*job;

	job = (t_job *)ft_malloc(sizeof(t_job));
	job->cmd = cmd;
	job->command = ast_to_str(cmd->ast);
	// job->command = and_or_to_str(cmd);
	job->first_process = get_first_proc(cmd->ast);
	job->pgid = 0;
	job->notified = 0;
	job->index = 0;
	job->fd_in = STDIN;
	job->fd_out = STDOUT;
	job->fd_err = STDERR;
	job->next = NULL;
	return (job);
}

t_job	*get_job_list(t_and_or *cmd)
{
	t_job	*job;

	job = (t_job *)ft_malloc(sizeof(t_job));
	job->cmd = cmd;
	// job->command = ast_to_str(cmd->ast);
	job->command = and_or_to_str(cmd);
	job->first_process = get_first_proc(cmd->ast);
	job->pgid = 0;
	job->notified = 0;
	job->index = 0;
	job->fd_in = STDIN;
	job->fd_out = STDOUT;
	job->fd_err = STDERR;
	job->next = NULL;
	return (job);
}

/*
** Return true if all processes in the job have stopped or completed.
*/
int job_is_stopped_completed(t_job *j)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (0);
		p = p->next;
	}
	return (1);
}

/*
** Return true if all processes in the job have completed.
*/
int	job_is_completed (t_job *j)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}

int		update_job(t_job *j, pid_t pid, int status)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (p->pid == pid)
		{
			j->notified = 0;
			p->status = status;
			if (WIFSTOPPED(status))
				p->stopped = 1;
			else if (WIFEXITED(status) || WIFSIGNALED(status))
				p->completed = 1;
			else if (WIFCONTINUED(status))
			{
				p->stopped = 0;
				p->completed = 0;
			}
			return (1);
		}
		p = p->next;
	}
	return (0);
}

void	add_job(t_job *j)
{
	t_job	*tmp;
	if (g_var.job == NULL)
		g_var.job = j;
	else
	{
		tmp = g_var.job;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = j;
	}
}

void	free_job(t_job **j)
{
	t_process	*p;
	t_process	*p2;

	if (!j || !*j)
		return ;
	ft_strdel(&((*j)->command));
	p = (*j)->first_process;
	while (p)
	{
		p2 = p;
		p = p->next;
		free_tab(p2->argv);
		free_tab(p2->env);
		ft_strdel(&(p2->command));
		free(p2);
		p2 = NULL;
	}
	free(*j);
	*j = NULL;
}

pid_t	find_last_bg_job(void)
{
	t_job	*j;
	pid_t	tmp;

	j = g_var.job;
	tmp = 0;
	while (j)
	{
		if (j->index != 0 && j->pgid != g_current_job && j->pgid != g_previous_job)
			tmp = j->pgid;
		j = j->next;
	}
	return (tmp);
}

void	give_current_job(pid_t pgid)
{
	if (g_current_job == pgid)
		return ;
	g_previous_job = g_current_job;
	g_current_job = pgid;
}

void	remove_current_job(pid_t pgid)
{
	if (g_current_job == pgid)
	{
		g_current_job = g_previous_job;
		g_previous_job = find_last_bg_job();
	}
	else if (g_previous_job == pgid)
	{
		g_previous_job = find_last_bg_job();
	}
}

int		remove_job(t_job *job)
{
	t_job	*current;
	t_job	*prec;

	current = g_var.job;
	prec = NULL;
	while (current)
	{
		if (current->pgid == job->pgid)
		{
			if (prec)
				prec->next = current->next;
			else
				g_var.job = current->next;
			remove_current_job(job->pgid);
			free_job(&job);
			return (1);
		}
		prec = current;
		current = current->next;
	}
	return (0);
}

int		report_to_user(t_job *j, int fg)//need norm
{
	t_process	*p;
	int			ret = 0;

	if (j->notified)
		return (0);
	p = j->first_process;
	while (p->next)
		p = p->next;
	if (job_is_completed(j))
	{
		if (j->index == 0 || fg)
		{
			if (WIFEXITED(p->status))
				ret = WEXITSTATUS(p->status);
			else
			{
				ret = WTERMSIG(p->status);
				if (ret != SIGINT)
					printf("*1*> %s: %d\n", ft_strsignal(ret), ret);
				if (ret == SIGINT)
					printf("\n");
			}
		}
		else
		{
			char	c;
			c = (j->pgid == g_current_job) ? '+' : ' ';
			c = (j->pgid == g_previous_job) ? '-' : c;
			if (WIFEXITED(p->status) && WEXITSTATUS(p->status) == 0)
				printf("*2*> [%d]%c  Done\t\t%s\n", j->index, c, j->command);
			else if (WIFEXITED(p->status) && WEXITSTATUS(p->status) != 0)
				printf("*3*> [%d]%c  Exit %d\t\t%s\n", j->index, c, WEXITSTATUS(p->status), j->command);
			else if (WIFSIGNALED(p->status))
			{
				ret = WTERMSIG(p->status);
				printf("*4*> [%d]%c  %s %d\t\t%s\n", j->index, c, ft_strsignal(ret), ret, j->command);
			}
		}
		if (remove_job(j) == 0)
			printf("JOBS: job not found.\n");
		return (ret);
	}
	if (job_is_stopped_completed(j))
	{
		j->index = (j->index == 0) ? get_new_index() : j->index;
		give_current_job(j->pgid);
		p = j->first_process;
		while (p)
		{
			if (p->stopped == 1)
			{
				ret = WSTOPSIG(p->status);
				break ;
			}
			p = p->next;
		}
		printf("*5*> [%d]+  %s\t\t%s\n", j->index, ft_strsignal(ret), j->command);
		j->notified = 1;
		return (ret);
	}
	return (1);
}

void	notify_user(void)
{
	t_job	*j;

	j = g_var.job;
	while (j)
	{
		report_to_user(j, 0);
		j = j->next;
	}
}

t_job	*find_job_ppid(pid_t pid)
{
	t_job		*j;
	t_process	*p;

	j = g_var.job;
	while (j)
	{
		p = j->first_process;
		while (p)
		{
			if (p->pid == pid)
				return (j);
			p = p->next;
		}
		j = j->next;
	}
	return (NULL);
}

t_job	*find_job(pid_t pgid)
{
	t_job	*j;

	j = g_var.job;
	while (j)
	{
		if (j->pgid == pgid)
			return (j);
		j = j->next;
	}
	return (find_job_ppid(pgid));
}

int		find_job_and_update(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if ((j = find_job(pid)) == NULL)
	{
		ft_print(STDERR, "ERROR pid [%d] not in any job.\n", pid);
		return (1);
	}
	j->notified = 0;
	p = j->first_process;
	while (p)
	{
		if (p->pid == pid)
		{
			p->status = status;
			if (WIFSTOPPED(status))
				p->stopped = 1;
			else if (WIFEXITED(status) || WIFSIGNALED(status))
				p->completed = 1;
		}
		p = p->next;
	}
	return (0);
}

int		wait_for_job(t_job *j)
{
	pid_t		pid;
	int			status;

	while (job_is_stopped_completed(j) == 0)
	{
		pid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
		if (pid == -1)
		{
			system_calls("waitpid: ", pid, -1);
		}
		if (update_job(j, pid, status) == 0)
		{
			find_job_and_update(pid, status);
		}
	}
	status = report_to_user(j, 1);
	return (status);
}

int		put_job_in_background (t_job *j, int cont)
{
	if (g_current_job == 0)
		g_current_job = j->pgid;
	else if (g_previous_job == 0)
		g_previous_job = j->pgid;
	j->notified = 0;
	j->index = (j->index == 0) ? get_new_index() : j->index;
	if (cont)
	{
		if (kill (-j->pgid, SIGCONT) < 0)
			system_calls ("kill (SIGCONT)", 1, 1);
	}
	else
		ft_print(STDOUT, "[%d] %d\n", j->index, j->pgid);
	return (0);
}

int		put_job_in_foreground (t_job *j, int cont)
{
	int	ret;
	pid_t pid;

	tcsetpgrp (STDIN, j->pgid);
	if (cont)
	{
		ft_set_attr(1);
		if (kill (- j->pgid, SIGCONT) < 0)
		{
			system_calls("kill (SIGCONT)", 1, 1);
		}
		pid = waitpid(- j->pgid, &ret, WUNTRACED | WCONTINUED);
		if (WIFCONTINUED(ret))
		{
			update_job(j, pid, ret);
		}
	}
	ret = wait_for_job (j);
	tcsetpgrp (STDIN, g_var.shell_pid);
	ft_set_attr(0);
	return (ret);
}

void	launch_process (t_process *p, t_job *j, int foreground, int io[2])//need norm
{
	pid_t	pid;
	char	*cmd_path;

	if (g_var.shell_is_interactive)
	{
		pid = getpid ();
		if (j->pgid == 0)
			j->pgid = pid;
		setpgid (pid, j->pgid);
		if (foreground)
		{
			tcsetpgrp (STDIN, j->pgid);
			ft_set_attr(1);
		}
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}
	if (io[1] != STDIN)
	{
		dup2 (io[1], STDIN);
		close (io[1]);
	}
	if (io[0] != STDOUT)
	{
		dup2 (io[0], STDOUT);
		close (io[0]);
	}
	if (do_simple_cmd(p->cmd) || p->argv == NULL)
		exit(1);
	// p->argv = get_arg_var_sub(p->cmd);
	p->env = env_to_tab(g_var.var, 0);
	if (is_builtin(p->argv[0]))
		exit(builtins(p->argv[0], p->argv, p->env));
	if (!(cmd_path = get_cmdpath(p->argv[0])))
		exit (127);
	execve (cmd_path, p->argv, p->env);
	ft_print(STDERR, "shell: permission denied: %s\n", p->argv[0]);
	exit (126);
}

int		launch_job(t_job *j, int foreground)//need norm
{
	t_process	*p;
	pid_t		pid;
	int			mypipe[2];
	int			infile;
	int			outfile;

	if ((infile = exec_no_fork(j->cmd->ast, j->first_process->argv, !foreground)) != -42)
	{
		free_job(&j);
		return (infile);
	}
	infile = j->fd_in;
	p = j->first_process;
	while (p)
	{
		if (p->next)
		{
			system_calls("pipe", pipe(mypipe), -1);
			outfile = mypipe[1];
		}
		else
			outfile = j->fd_out;
		system_calls("fork", (pid = fork ()), -1);
		if (pid == 0)
		{
			if (infile != mypipe[0])
				close(mypipe[0]);
			if (outfile != mypipe[1])
				close(mypipe[1]);
			mypipe[0] = outfile;
			mypipe[1] = infile;
			launch_process (p, j, foreground, mypipe);
		}
		else
		{
			p->pid = pid;
			if (g_var.shell_is_interactive)
			{
				if (!j->pgid)
					j->pgid = pid;
				setpgid (pid, j->pgid);
			}
			if (infile != j->fd_in)
				close (infile);
			if (outfile != j->fd_out)
				close (outfile);
			infile = mypipe[0];
		}
		p = p->next;
	}
	add_job(j);
	if (!g_var.shell_is_interactive)
		return (wait_for_job (j));
	else if (foreground)
		return (put_job_in_foreground (j, 0));
	else
		return (put_job_in_background (j, 0));
}

void	grouped_job(t_and_or *cmd)
{
	int		pid;
	t_job	*job;

	system_calls("fork", (pid = fork ()), -1);
	if (pid == 0)
	{
		pid = getpid();
		setpgid(pid, pid);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
		exit (execute(cmd, pid));
	}
	else
	{
		setpgid(pid, pid);
		job = get_job_list(cmd);
		job->pgid = pid;
		job->first_process->pid = pid;
		add_job(job);
		put_job_in_background(job, 0);
	}
}

int		job_control(t_and_or *cmd, int bg)
{
	t_job		*job;
	int 		dp;
	int 		ret;

	if (tty == NULL)// to be deleted
		tty = fopen("/dev/ttys003", "w");
	ret = 0;
	if (bg && cmd->next)
	{
		grouped_job(cmd);
		return (ret);
	}
	while (cmd)
	{
		dp = cmd->dependent;
		if (!dp || (dp == 1 && !ret) || (dp == 2 && ret))
		{
			job = get_job(cmd);
			ret = launch_job(job, !bg);
			exit_status(ret << 8);
		}
		cmd = cmd->next;
	}
	return (ret);
}


int		get_opt(char **av, int *index)
{
	int		i;
	int		j;
	int		opt;

	i = 1;
	opt = 0;
	while (av[i] && (av[i][0] == '-' && av[i][1] && av[i][1] != '-'))
	{
		j = 1;
		while (av[i][j])
		{
			if (av[i][j] == 'l')
				opt = 1;
			else if (av[i][j] == 'p')
				opt = 2;
			else
				return (-(av[i][j]));
			j++;
		}
		i++;
	}
	*index = i;
	return (opt);
}

t_job	*get_pid_str(char *s)
{
	t_job	*j;
	int		len;

	len = ft_strlen(s);
	j = g_var.job;
	if (*s == '?')
	{
		s++;
		while (j)
		{
			if (ft_strstr(j->command, s))
				return (j);
			j = j->next;
		}
	}
	while (j)
	{
		if (!ft_strncmp(j->command, s, len))
			return (j);
		j = j->next;
	}
	ft_print(STDERR, "shell: job_util: %s: no such job.\n", s);
	return (NULL);
}

t_job	*get_pid_n_plus_min(char c, char *s)
{
	t_job	*j;
	int		index;
	pid_t	pgid;

	j = g_var.job;
	index = (s != NULL) ? ft_atoi(s) : -1;
	pgid = (c == '+') ? g_current_job : 0;
	pgid = (c == '-') ? g_previous_job : pgid;
	while (j)
	{
		if (index == j->index || (pgid != 0 && pgid == j->pgid))
			return (j);
		j = j->next;
	}
	ft_putstr_fd("shell: job_util: ", STDERR);
	if (c == '+')
		ft_putstr_fd("current", STDERR);
	else if (c == '-')
		ft_putstr_fd("previous", STDERR);
	else
		ft_putstr_fd(s, STDERR);
	ft_putstr_fd(": no such job.\n", STDERR);
	return (NULL);
}

t_job	*selected_job(char *s)
{
	t_job	*j;

	if (s[0] == '%')
	{
		if (s[1] == '+' || s[1] == '%')
			j = get_pid_n_plus_min('+', NULL);
		else if (s[1] == '-')
			j = get_pid_n_plus_min('-', NULL);
		else if (is_all_digits(&s[1]))
			j = get_pid_n_plus_min(-1, &(s[1]));
		else
			j = get_pid_str(&s[1]);
	}
	else
		j = get_pid_str(s);
	return (j);
}

int		ft_bg(char **av)
{
	char	c;
	t_job	*j;

	j = NULL;
	if (av[1])
	{
		j = selected_job(av[1]);
	}
	else
		j = get_pid_n_plus_min('+', NULL);
	if (j == 0)
		return (1);
	c = (j->pgid == g_current_job) ? '+' : ' ';
	c = (j->pgid == g_previous_job) ? '-' : c;
	printf("-1-> [%d]%c  %s &\n", j->index, c, j->command);
	return (put_job_in_background(j, 1));
}

int		ft_fg(char **av)
{
	t_job	*j;

	j = NULL;
	if (av[1])
	{
		j = selected_job(av[1]);
	}
	else
		j = get_pid_n_plus_min('+', NULL);
	if (j == NULL)
		return (1);
	ft_print(STDOUT, "%s\n", j->command);
	return (put_job_in_foreground(j, 1));
}

void	job_print(t_job *j, int l)
{
	char	c;
	int		sig;

	c = (j->pgid == g_current_job) ? '+' : ' ';
	c = (j->pgid == g_previous_job) ? '-' : c;
	sig = j->first_process->status;
	sig = WIFEXITED(sig) ? WEXITSTATUS(sig) : sig;
	sig = WIFSIGNALED(sig) ? WTERMSIG(sig) : sig;
	sig = WIFSTOPPED(sig) ? WSTOPSIG(sig) : sig;
	ft_print(STDOUT,"[%d]%c  ", j->index, c);
	if (l)
		ft_print(STDOUT,"%d %s: %d\t\t%s\n", j->pgid, ft_strsignal(sig), sig,
																	j->command);
	else
	{
		if (job_is_stopped_completed(j) == 0)
			ft_print(STDOUT,"running\t\t%s\n", j->command);
		else if (job_is_completed(j))
			ft_print(STDOUT,"done\t\t%s\n", j->command);
		else if (job_is_stopped_completed(j))
			ft_print(STDOUT,"stopped\t\t%s\n", j->command);
	}
}

int		ft_jobs_av(char **av)
{
	t_job	*j;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if ((j = selected_job(av[i])) == NULL)
		{
			i++;
			ret = 1;
			continue ;
		}
		job_print(j, 0);
		i++;
	}
	return (ret);
}

int		ft_jobs_(char **av)
{
	t_job	*j;
	char	c;

	if (av && av[0])
		return (ft_jobs_av(av));
	j = g_var.job;
	while (j)
	{
		c = (j->pgid == g_current_job) ? '+' : ' ';
		c = (j->pgid == g_previous_job) ? '-' : c;
		ft_print(STDOUT,"[%d]%c\t", j->index, c);
		if (job_is_stopped_completed(j) == 0)
			ft_print(STDOUT,"running\t\t%s\n", j->command);
		else if (job_is_completed(j))
			ft_print(STDOUT,"done\t\t%s\n", j->command);
		else if (job_is_stopped_completed(j))
			ft_print(STDOUT,"stopped\t\t%s\n", j->command);
		j = j->next;
	}
	return (0);
}

int		ft_jobs_opt(char **av, int opt)
{
	int		i;
	int		ret;
	t_job	*j;

	i = 0;
	ret = 0;
	if (av && av[0])
	{
		while (av[i])
		{
			if((j = selected_job(av[i])))
				(opt == 1) ? job_print(j, 1) : ft_print(1, "%d\n", j->pgid);
			else
				ret = 1;
			i++;
		}
		return (ret);
	}
	j = g_var.job;
	while (j)
	{
		(opt == 1) ? job_print(j, 1) : ft_print(STDOUT, "%d\n", j->pgid);
		j = j->next;
	}
	return (0);
}

int		ft_jobs(char **av)
{
	int		i;
	int		opt;

	opt = get_opt(av, &i);
	if (opt < 0)
	{
		ft_print(STDERR, "shell: jobs: -");
		ft_putchar_fd(-opt, STDERR);
		ft_print(STDERR, ": invalid option.\n");
		return (1);
	}
	if (opt == 1 || opt == 2)
	{
		return (ft_jobs_opt(&av[i], opt));
	}
	return (ft_jobs_(&av[i]));
}

// norm functions with "need norm" tag
// fix builtins arg management
// add ft_jobs options..
// test leaks && sigfaults cases in get_cmdargs
// norm everytyhing
