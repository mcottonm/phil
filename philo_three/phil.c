/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:10:23 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/09 23:23:10 by mcottonm         ###   ########.fr       */
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
		exit(3);
	if (60 > (g_sphil->time_to_eat = ft_atoi(av[3])))
		exit(3);
	if (60 > (g_sphil->time_to_sleep = ft_atoi(av[4])))
		exit(3);
	if (ac == 6)
	{
		g_sphil->times = ft_atoi(av[5]);
		if (!g_sphil->times)
			exit(0);
	}
	else
		g_sphil->times = 0;
	g_work_s.kill = false;
	g_work_s.qu_i = 0;
}

static void	emul_strt(t_phil sphil)
{
	int		i;

	g_work_s.start = timer_now() + 1000;
	i = -1;
	sem_unlink(SEM_NAME_L);
	g_work_s.l_fork = sem_open(SEM_NAME_L, O_CREAT,
		0644, g_sphil.nbr_of_phil / 2);
	sem_unlink(SEM_NAME_R);
	g_work_s.r_fork = sem_open(SEM_NAME_R, O_CREAT,
		0644, g_sphil.nbr_of_phil / 2);
	sem_unlink(SEM_NAME_LOG);
	g_work_s.log_sem = sem_open(SEM_NAME_LOG, O_CREAT, 0644, 1);
	g_work_s.start = timer_now() + TIME_TO_START;
	i = -1;
	while (++i < sphil.nbr_of_phil)
		if (!(g_work_s.pid[i] = fork()))
			control(i);
}

static void	emul_end(t_phil g_sphil)
{
	int status;
	int g;
	int pid;

	status = 0;
	g = -1;
	while ((pid = waitpid(-1, &status, WNOHANG)) == 0)
	{
		if (status)
		{
			while (++g < g_sphil.nbr_of_phil)
				if (g != pid)
					kill(g_work_s.pid[g++], SIGKILL);
			sem_post(g_work_s.log_sem);
			break ;
		}
	}
	sem_unlink(SEM_NAME_L);
	sem_unlink(SEM_NAME_R);
	sem_unlink(SEM_NAME_LOG);
}

int			main(int ac, char **av)
{
	pars_args(ac, av, &g_sphil);
	emul_strt(g_sphil);
	emul_end(g_sphil);
	return (0);
}
