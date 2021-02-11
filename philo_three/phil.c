/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:10:23 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/11 20:39:00 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static bool	pars_args(int ac, char **av, t_phil *g_sphil)
{
	if (ac < 5 || ac > 6)
		return (ft_exit(1));
	if (200 < (g_sphil->nbr_of_phil = ft_atoi(av[1]))
	|| g_sphil->nbr_of_phil < 1)
		return (ft_exit(2));
	if (60 > (g_sphil->time_to_die = ft_atoi(av[2])))
		return (ft_exit(3));
	if (60 > (g_sphil->time_to_eat = ft_atoi(av[3])))
		return (ft_exit(3));
	if (60 > (g_sphil->time_to_sleep = ft_atoi(av[4])))
		return (ft_exit(3));
	if (g_sphil->nbr_of_phil == 1)
		return (ft_exit(4));
	if (ac == 6)
	{
		g_sphil->times = ft_atoi(av[5]);
		if (g_sphil->times <= 0)
			return (false);
	}
	else
		g_sphil->times = 0;
	g_work_s.kill = false;
	return (true);
}

static void	emul_strt(t_phil sphil)
{
	int		i;

	g_work_s.start = timer_now() + 1000;
	i = -1;
	sem_unlink(SEM_NAME_F);
	g_work_s.fork = sem_open(SEM_NAME_F, O_CREAT,
		0644, g_sphil.nbr_of_phil);
	sem_unlink(SEM_NAME_LOG);
	g_work_s.log_sem = sem_open(SEM_NAME_LOG, O_CREAT, 0644, 1);
	g_work_s.start = timer_now() + TIME_TO_START;
	i = -1;
	while (++i < sphil.nbr_of_phil)
		if (!(g_work_s.pid[i] = fork()))
			control(i);
}

static int	proc_check(int *status)
{
	int pid;
	int g;

	*status = 42;
	pid = -1;
	g = -1;
	while (++g < g_sphil.nbr_of_phil)
	{
		if ((pid = waitpid(g_work_s.pid[g], status, WNOHANG)))
		{
			if (*status == 256)
			{
				g = -1;
				while (++g < g_sphil.nbr_of_phil)
						kill(g_work_s.pid[g], SIGKILL);
				return (-1);
			}
			else
				g_work_s.pid[g] = -1;
		}
		pid = g_work_s.pid[g] > pid ? g_work_s.pid[g] : pid;
	}
	return (pid);
}

static void	emul_end(void)
{
	int status;

	status = 0;
	while (true)
		if (proc_check(&status) == -1)
			break ;
	sem_unlink(SEM_NAME_F);
	sem_post(g_work_s.fork);
	sem_unlink(SEM_NAME_LOG);
}

int			main(int ac, char **av)
{
	if (!pars_args(ac, av, &g_sphil))
		return (0);
	emul_strt(g_sphil);
	emul_end();
	return (0);
}
