/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:50:08 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/10 18:26:16 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	think(long mark)
{
	add_to_queue(mark, 1);
	sem_wait(g_work_s.forks);
	add_to_queue(mark, 4);
	sem_wait(g_work_s.forks);
	add_to_queue(mark, 4);
}

static void	eat(long mark, long *timer)
{
	*timer = timer_now();
	g_work_s.phil_check[mark] = g_sphil.time_to_die;
	if (g_work_s.kill)
	{
		sem_post(g_work_s.forks);
		sem_post(g_work_s.forks);
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
	sem_post(g_work_s.forks);
	sem_post(g_work_s.forks);
	pth_sleep(*timer);
}

static bool	times_check(int mark, int *pth_times)
{
	if ((!--(*pth_times) && g_sphil.times) || g_work_s.kill)
	{
		sem_post(g_work_s.forks);
		sem_post(g_work_s.forks);
		g_work_s.phil_check[mark] = TIMES_FLAG;
		return (false);
	}
	return (true);
}

void		*start_thread(void *void_ptr)
{
	const int	mark = (const int)void_ptr;
	long		timer;
	int			pth_times;

	pth_times = g_sphil.times;
	timer = g_work_s.start;
	pth_sleep(g_work_s.start);
	if (mark % 2)
		pth_sleep(timer_now() + DELTA_TIME);
	while (!g_work_s.kill)
	{
		if (g_work_s.kill)
			return (NULL);
		think(mark);
		eat(mark, &timer);
		if (!times_check(mark, &pth_times))
			return (NULL);
		sleeping(mark, &timer);
	}
	return (NULL);
}
