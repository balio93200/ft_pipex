#include "pipex.h"

static void	usage(void)
{
	putstr_fd("Usage: ./pipex file1 \"cmd1 args\" \"cmd2 args\" file2\n", 2);
	exit(1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;
	pid_t	c1;
	pid_t	c2;

	if (argc != 5)
		usage();
	px.envp = envp;
	if (pipe(px.pipe_fd) == -1)
		die_perror("pipe", 1);
	c1 = fork();
	if (c1 == -1)
		die_perror("fork", 1);
	if (c1 == 0)
		run_child1(&px, argv[2], argv[1]);
	c2 = fork();
	if (c2 == -1)
		die_perror("fork", 1);
	if (c2 == 0)
		run_child2(&px, argv[3], argv[4]);
	close(px.pipe_fd[0]);
	close(px.pipe_fd[1]);
	return (wait_and_get_status(c1, c2));
}
