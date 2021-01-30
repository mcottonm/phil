# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include "libft.h"

typedef struct			phil
{
	int		nbr_of_phil;
	int		time_to_die;
	int		time_to_eat;
	int		times;
	int		mark;
	bool	kill;
}						phil;

void	*inc_x(void *x_void_ptr)
{
	const int	mark = ((struct phil *)x_void_ptr)->mark;
	char c = mark + 48;
	struct phil	*x_ptr;
	x_ptr = (struct phil *)x_void_ptr;
	printf("%c\n", c);
/*	while (1)
	{
		if (mark == x_ptr->nbr_of_phil - 1)
			x_ptr->kill = 1;
		write(1, &c, 1);
		write(1, "\n", 1);
	}*/
	return NULL;
}

struct	phil	struc_init(int ac, char **av)
{
	struct phil one;

	one.nbr_of_phil = ft_atoi(av[1]);
	one.time_to_die = ft_atoi(av[2]);
	one.time_to_eat = ft_atoi(av[3]);
	if (ac == 5)
		one.times = ft_atoi(av[4]);
	else
		one.times = -1;
	one.kill = 0;
	return (one);
}

int		main(int ac, char **av)
{
	struct phil	one;
	pthread_t	*phils;
	int			i;

	if (ac < 4 || ac > 5)
		return (0);
	phils = malloc(sizeof(pthread_t) * ft_atoi(av[1]) + 1);
	one = struc_init(ac, av);
	i = -1;
	while (++i < one.nbr_of_phil)
	{
		one.mark = i;
		if(pthread_create(&(phils[i]), NULL, inc_x, &one))
				return 1;
	}
	while (!one.kill);
		//printf("PZDC%d\n", one.kill);
	return (0);
}