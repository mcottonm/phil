/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:50:08 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/11 20:10:40 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	think(long mark)
{
	add_to_queue(mark, 1);
	sem_wait(g_work_s.fork);
	add_to_queue(mark, 4);
	sem_wait(g_work_s.fork);
	add_to_queue(mark, 4);
}

static void	eat(long mark, long *timer)
{
	*timer = timer_now();
	g_work_s.phil_check = g_sphil.time_to_die;
	if (g_work_s.kill)
	{
		sem_post(g_work_s.fork);
		sem_post(g_work_s.fork);
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
	sem_post(g_work_s.fork);
	sem_post(g_work_s.fork);
	pth_sleep(*timer);
}

static bool	times_check(int *pth_times)
{
	if ((!--(*pth_times) && g_sphil.times) || g_work_s.kill)
	{
		sem_post(g_work_s.fork);
		sem_post(g_work_s.fork);
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

	pthread_detach(g_work_s.phil);
	pth_times = g_sphil.times;
	timer = g_work_s.start;
	g_work_s.phil_check = g_sphil.time_to_die;
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
