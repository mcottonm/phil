NAME =      philo_one

SRCS =      aux.c\
			logger.c\
			phil.c\
			pthreads.c\
			control.c

OBG =       $(SRCS:.c=.o)

all:        $(NAME)

$(NAME):
			gcc -o philo_one $(SRCS) -Wall -Wextra -Werror

clean:
			rm -f $(OBG)

fclean:		clean
			rm -f $(NAME)

re:			fclean all