NAME =      philo_three

SRCS =      aux.c\
			logger.c\
			phil.c\
			pthreads.c\
			control.c

OBG =       $(SRCS:.c=.o)

all:        $(NAME)

$(NAME):	$(OBG)
			gcc -o philo_three $(SRCS) -Wall -Wextra -Werror

clean:
			rm -f $(OBG)

fclean:		clean
			rm -f $(NAME)

re:			fclean all