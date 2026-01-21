NAME	= pipex
BONUS	= pipex_bonus

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= pipex.c exec.c parse.c path.c utils.c
OBJS	= $(SRCS:.c=.o)

BONUS_SRCS	= pipex_bonus.c bonus_exec.c heredoc.c exec.c parse.c path.c utils.c
BONUS_OBJS	= $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

bonus: $(BONUS)

$(BONUS): $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) -o $(BONUS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME) $(BONUS)

re: fclean all

.PHONY: all bonus clean fclean re
