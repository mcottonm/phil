/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:10:23 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/10 17:44:58 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	pars_args(int ac, char **av, t_phil *g_sphil)
{
	if (ac < 5 || ac > 6)
		ft_exit(1);
	if (200 < (g_sphil->nbr_of_phil = ft_atoi(av[1]))
	|| g_sphil->nbr_of_phil < 2)
		ft_exit(2);
	if (60 > (g_sphil->time_to_die = ft_atoi(av[2])))
		ft_exit(3);
	if (60 > (g_sphil->time_to_eat = ft_atoi(av[3])))
		ft_exit(3);
	if (60 > (g_sphil->time_to_sleep = ft_atoi(av[4])))
		ft_exit(3);
	if (ac == 6)
	{
		g_sphil->times = ft_atoi(av[5]);
		if (g_sphil->times <= 0)
			exit(0);
	}
	else
		g_sphil->times = 0;
	g_work_s.kill = false;
	g_work_s.qu_i = 0;
}

static void	emul_strt(t_phil sphil, pthread_t *phils)
{
	int		i;

	g_work_s.start = timer_now() + 1000;
	i = -1;
	while (++i < sphil.nbr_of_phil + 2)
		pthread_mutex_init(&(g_work_s.lock[i]), NULL);
	i = -1;
	g_work_s.start = timer_now() + TIME_TO_START;
	while (++i < sphil.nbr_of_phil)
		pthread_create(&(phils[i]), NULL, start_thread, (void *)(long)i);
}

static void	emul_end(t_phil g_sphil, pthread_t *phils)
{
	int	i;

	i = -1;
	while (++i < g_sphil.nbr_of_phil + 1)
		if ((pthread_join(phils[i], NULL)))
			exit(1);
	i = -1;
	while (++i < g_sphil.nbr_of_phil + 2)
		pthread_mutex_destroy(&(g_work_s.lock[i]));
}

int			main(int ac, char **av)
{
	pthread_t		phils[201];

	pars_args(ac, av, &g_sphil);
	emul_strt(g_sphil, phils);
	pthread_create(&(phils[g_sphil.nbr_of_phil + 1]), NULL, logger, NULL);
	control();
	emul_end(g_sphil, phils);
	return (0);
}
