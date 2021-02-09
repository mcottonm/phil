/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:50:08 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/09 23:18:08 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	think(long mark)
{
	sem_wait(g_work_s.l_fork);
	sem_wait(g_work_s.r_fork);
	add_to_queue(mark, 1);
}

static void	eat(long mark, long *timer)
{
	*timer = timer_now();
	g_work_s.phil_check = g_sphil.time_to_die + g_sphil.time_to_eat;
	if (g_work_s.kill)
	{
		sem_post(g_work_s.l_fork);
		sem_post(g_work_s.r_fork);
		return ;
	}
	add_to_queue(mark, 2);
	*timer += g_sphil.time_to_eat;
	pth_sleep(*timer);
}

static void	sleeping(long mark, long *timer)
{
	add_to_queue(mark, 3);
	*timer += g_sphil.time_to_sleep;
	sem_post(g_work_s.l_fork);
	sem_post(g_work_s.r_fork);
	pth_sleep(*timer);
}

static bool	times_check(int *pth_times)
{
	if ((!--(*pth_times) && g_sphil.times) || g_work_s.kill)
	{
		sem_post(g_work_s.l_fork);
		sem_post(g_work_s.r_fork);
		g_work_s.kill = true;
		return (false);
	}
	return (true);
}

void		*phil_start(void *void_ptr)
{
	long		timer;
	int			pth_times;
	const int	mark = (const int)void_ptr;

	pth_times = g_sphil.times;
	timer = g_work_s.start;
	pth_sleep(g_work_s.start);
	if ((mark) % 2)
		pth_sleep(timer_now() + DELTA_TIME);
	while (!g_work_s.kill)
	{
		if (g_work_s.kill)
			return (NULL);
		think(mark);
		eat(mark, &timer);
		if (!times_check(&pth_times))
			return (NULL);
		sleeping(mark, &timer);
	}
	return (NULL);
}
