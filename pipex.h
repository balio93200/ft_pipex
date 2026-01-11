#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <stdio.h>

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int		pipe_fd[2];
	char	**envp;
}t_pipex;

int		main(int argc, char **argv, char **envp);
void	run_child1(t_pipex *px, char *cmd1, char *file1);
void	run_child2(t_pipex *px, char *cmd2, char *file2);
int		wait_and_get_status(pid_t c1, pid_t c2);

char	**split_cmd(const char *s);
char	*find_cmd_path(const char *cmd, char **envp);

size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_substr(const char *s, size_t start, size_t len);
char	*ft_strjoin3(const char *a, const char *b, const char *c);
void	free_strv(char **v);

void	putstr_fd(const char *s, int fd);
void	puterr_prefix_cmd(const char *prefix, const char *cmd, const char *msg);
void	die_perror(const char *ctx, int code);
int		is_space(char c);

#endif
