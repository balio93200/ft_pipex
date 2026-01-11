#include "pipex.h"

static void	safe_dup2(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1)
		die_perror("dup2", 1);
}

static int	open_infile_like_shell(const char *file1)
{
	int	fd;

	fd = open(file1, O_RDONLY);
	if (fd == -1)
	{
		putstr_fd("pipex: ", 2);
		putstr_fd(file1, 2);
		putstr_fd(": ", 2);
		putstr_fd(strerror(errno), 2);
		putstr_fd("\n", 2);
		fd = open("/dev/null", O_RDONLY);
		if (fd == -1)
			die_perror("open /dev/null", 1);
	}
	return (fd);
}

static int	open_outfile(const char *file2)
{
	int	fd;

	fd = open(file2, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
	{
		putstr_fd("pipex: ", 2);
		putstr_fd(file2, 2);
		putstr_fd(": ", 2);
		putstr_fd(strerror(errno), 2);
		putstr_fd("\n", 2);
		exit(1);
	}
	return (fd);
}

static void	exec_cmdline(char *cmdline, char **envp)
{
	char	**argv;
	char	*path;

	argv = split_cmd(cmdline);
	if (!argv || !argv[0])
		exit(127);
	path = find_cmd_path(argv[0], envp);
	if (!path)
	{
		puterr_prefix_cmd("pipex: ", argv[0], "command not found");
		free_strv(argv);
		exit(127);
	}
	execve(path, argv, envp);
	puterr_prefix_cmd("pipex: ", argv[0], strerror(errno));
	free(path);
	free_strv(argv);
	exit(126);
}

void	run_child1(t_pipex *px, char *cmd1, char *file1)
{
	int	infd;

	infd = open_infile_like_shell(file1);
	close(px->pipe_fd[0]);
	safe_dup2(infd, STDIN_FILENO);
	safe_dup2(px->pipe_fd[1], STDOUT_FILENO);
	close(infd);
	close(px->pipe_fd[1]);
	exec_cmdline(cmd1, px->envp);
}

void	run_child2(t_pipex *px, char *cmd2, char *file2)
{
	int	outfd;

	outfd = open_outfile(file2);
	close(px->pipe_fd[1]);
	safe_dup2(px->pipe_fd[0], STDIN_FILENO);
	safe_dup2(outfd, STDOUT_FILENO);
	close(outfd);
	close(px->pipe_fd[0]);
	exec_cmdline(cmd2, px->envp);
}

int	wait_and_get_status(pid_t c1, pid_t c2)
{
	int	st1;
	int	st2;

	waitpid(c1, &st1, 0);
	waitpid(c2, &st2, 0);
	if (WIFEXITED(st2))
		return (WEXITSTATUS(st2));
	return (1);
}
