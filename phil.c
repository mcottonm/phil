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

int		nbr_of_phil;
int		time_to_die;
int		time_to_eat;
int		times;
int		mark;
bool	kill;
int		test = 0;
pthread_mutex_t	*lock;

void	print(int mark, int l_fork, int r_fork)
{
	pthread_mutex_lock(&(lock[-1]));
	printf("MARK: [%d] [%d] [%d]\n", mark, l_fork, r_fork);
	printf("[%d] [%d]\n", mark, ++test);
	pthread_mutex_unlock(&(lock[-1]));
}

void	*inc_x(void *x_void_ptr)
{
	const int	mark = *((const int *)x_void_ptr);
	const int	l_fork = mark;
	const int	r_fork = mark == nbr_of_phil - 1 ? 0 : mark + 1;

	pthread_mutex_lock(&(lock[l_fork]));
	pthread_mutex_lock(&(lock[r_fork]));
	print(mark + 1, l_fork, r_fork);
	pthread_mutex_unlock(&(lock[l_fork]));
	pthread_mutex_unlock(&(lock[r_fork]));
	return NULL;
}

int	ft_atoi(const char *str)
{
	int			i;
	int			at;

	i = 1;
	at = 0;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
		i = -1;
	if ((*str == '+') || (*str == '-'))
		str++;
	while (*str >= '0' && *str <= '9')
		at = at * 10 - '0' + *str++;
	return (at * i);
}

int		*glob_init(int ac, char **av)
{
	int			*phil_nmbr;
	int 		i;

	nbr_of_phil = ft_atoi(av[1]);
	time_to_die = ft_atoi(av[2]);
	time_to_eat = ft_atoi(av[3]);
	if (ac == 5)
		times = ft_atoi(av[4]);
	else
		times = -1;
	kill = 0;
	i = -1;
	if (!(phil_nmbr = malloc(4 * nbr_of_phil + 1)))
		exit(1);
	while ( ++i < nbr_of_phil)
		phil_nmbr[i] = i;
	return (phil_nmbr);
}

int		main(int ac, char **av)
{
	pthread_t		*phils;
	int				*phil_nmbr;
	int				i;

	if (ac < 4 || ac > 5)
		return (0);
	phils = malloc(sizeof(pthread_t) * ft_atoi(av[1]) + 1);
	lock = malloc(sizeof(pthread_mutex_t) * ft_atoi(av[1]) + 1);
	phil_nmbr = glob_init(ac, av);
	i = -2;
	while (++i < nbr_of_phil)
		if (pthread_mutex_init(&(lock[i]), NULL))
        	return 1;
	while (--i != -1)
		if(pthread_create(&(phils[i]), NULL, inc_x, &(phil_nmbr[i])))
			return (1);
	while(++i < nbr_of_phil)
		if((pthread_join(phils[i], NULL)))
			return (1);
	--i;
	while (--i != -1)
		if (pthread_mutex_destroy(&(lock[i])))
        	return (1);
	printf("END\n");
	return (0); 
}