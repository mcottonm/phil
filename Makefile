NAME =      philo_one

SRCS =      aux.c\
			logger.c\
			phil.c\
			pthreads.c\
			time.c

OBG =       $(SRCS:.c=.o)

all:        $(NAME)

$(NAME):
			gcc $(SRCS) -Wall -Wextra -Werror philo_one

clean:
			rm -f $(OBG)

fclean:		clean
			rm -f $(NAME)

re:			fclean all