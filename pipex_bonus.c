#include "pipex.h"

int	is_here_doc(const char *s)
{
	if (!s)
		return (0);
	return (ft_strcmp(s, "here_doc") == 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;
	int		here_doc;
	int		in_fd;
	int		out_fd;
	int		cmd_start;
	int		cmd_count;
	pid_t	writer_pid;

	if (argc < 5)
	{
		putstr_fd("Usage:\n", 2);
		putstr_fd("./pipex_bonus file1 cmd1 cmd2 ... cmdn file2\n", 2);
		putstr_fd("./pipex_bonus here_doc LIMITER cmd1 cmd2 ... cmdn file2\n", 2);
		return (1);
	}
	px.envp = envp;
	writer_pid = -1;
	here_doc = is_here_doc(argv[1]);
	if (here_doc)
	{
		if (argc < 6)
			return (putstr_fd("pipex_bonus: here_doc needs at least 2 cmds\n", 2), 1);
		in_fd = create_heredoc_pipe(argv[2], &writer_pid);
		cmd_start = 3;
		cmd_count = argc - 4; /* cmds between limiter and outfile */
		out_fd = open_outfile_mode(argv[argc - 1], 1);
	}
	else
	{
		in_fd = open_infile_like_shell(argv[1]);
		cmd_start = 2;
		cmd_count = argc - 3; /* cmds between infile and outfile */
		out_fd = open_outfile_mode(argv[argc - 1], 0);
	}
	return (run_pipeline(&px, argv + cmd_start, cmd_count, in_fd, out_fd, writer_pid));
}
