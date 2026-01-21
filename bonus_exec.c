#include "pipex.h"

static void	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		die_perror("dup2", 1);
}

static void	exec_cmdline(char *cmdline, char **envp)
{
	char	**argv;
	char	*path;

	argv = split_cmd(cmdline);
	if (!argv || !argv[0] || argv[0][0] == '\0')
	{
		free_strv(argv);
		exit(127);
	}
	path = find_cmd_path(argv[0], envp);
	if (!path)
	{
		puterr_prefix_cmd("pipex: ", argv[0], "command not found");
		free_strv(argv);
		exit(127);
	}
	execve(path, argv, envp);
	if (errno == EACCES)
	{
		puterr_prefix_cmd("pipex: ", argv[0], "Permission denied");
		free(path);
		free_strv(argv);
		exit(126);
	}
	putstr_fd("pipex: ", 2);
	putstr_fd(argv[0], 2);
	putstr_fd(": ", 2);
	putstr_fd(strerror(errno), 2);
	putstr_fd("\n", 2);
	free(path);
	free_strv(argv);
	exit(126);
}

static pid_t	spawn_cmd(t_pipex *px, char *cmdline, int in_fd, int out_fd, int close_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		die_perror("fork", 1);
	if (pid == 0)
	{
		safe_dup2(in_fd, STDIN_FILENO);
		safe_dup2(out_fd, STDOUT_FILENO);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (close_out && out_fd != STDOUT_FILENO)
			close(out_fd);
		exec_cmdline(cmdline, px->envp);
	}
	return (pid);
}

int	run_pipeline(t_pipex *px, char **cmds, int cmd_count, int in_fd, int out_fd, pid_t writer_pid)
{
	int		i;
	int		pfd[2];
	int		prev_read;
	pid_t	last_pid;
	pid_t	pids[1024]; /* enough for bonus eval sizes; could be dynamic */
	int		pidn;
	int		status;
	int		st;

	prev_read = in_fd;
	i = 0;
	pidn = 0;
	last_pid = -1;
	while (i < cmd_count)
	{
		if (i == cmd_count - 1)
		{
			last_pid = spawn_cmd(px, cmds[i], prev_read, out_fd, 0);
			pids[pidn++] = last_pid;
			if (prev_read != in_fd)
				close(prev_read);
			break ;
		}
		if (pipe(pfd) == -1)
			die_perror("pipe", 1);
		pids[pidn++] = spawn_cmd(px, cmds[i], prev_read, pfd[1], 1);
		close(pfd[1]);
		if (prev_read != in_fd)
			close(prev_read);
		prev_read = pfd[0];
		i++;
	}
	/* parent closes endpoints */
	close(in_fd);
	close(out_fd);
	if (prev_read != in_fd && prev_read != -1)
		close(prev_read);
	/* wait children */
	status = 1;
	i = 0;
	while (i < pidn)
	{
		waitpid(pids[i], &st, 0);
		if (pids[i] == last_pid && WIFEXITED(st))
			status = WEXITSTATUS(st);
		i++;
	}
	if (writer_pid != -1)
		waitpid(writer_pid, NULL, 0);
	return (status);
}
