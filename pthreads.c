/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:50:08 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/05 00:04:55 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	think(long mark, const int l_fork, const int r_fork)
{
	add_to_queue(mark, 1);
	pthread_mutex_lock(&(g_work_s.lock[l_fork]));
	pthread_mutex_lock(&(g_work_s.lock[r_fork]));
}

static void	eat(long mark, long *timer, const int l_fork, const int r_fork)
{
	*timer = timer_now();
	g_work_s.phil_check[mark] = g_sphil.time_to_die + g_sphil.time_to_eat;
	if (g_work_s.kill)
	{
		pthread_mutex_unlock(&(g_work_s.lock[l_fork]));
		pthread_mutex_unlock(&(g_work_s.lock[r_fork]));
		return ;
	}
	add_to_queue(mark, 2);
	*timer += g_sphil.time_to_eat;
	pth_sleep(*timer);
	pthread_mutex_unlock(&(g_work_s.lock[l_fork]));
	pthread_mutex_unlock(&(g_work_s.lock[r_fork]));
}

static void	sleeping(long mark, long *timer)
{
	add_to_queue(mark, 3);
	*timer += g_sphil.time_to_sleep;
	pth_sleep(*timer);
}

void		*start_thread(void *void_ptr)
{
	const int	mark = (const int)void_ptr;
	const int	l_fork = mark == g_sphil.nbr_of_phil - 1 ? 0 : mark;
	const int	r_fork = mark == g_sphil.nbr_of_phil - 1 ? mark : mark + 1;
	long		timer;
	int			pth_times;

	pth_times = g_sphil.times;
	timer = g_work_s.start;
	pth_sleep(g_work_s.start);
	if ((mark) % 2)
		pth_sleep(timer_now() + DELTA_TIME);
	while (!g_work_s.kill)
	{
		if (g_work_s.kill)
			return (NULL);
		think(mark, l_fork, r_fork);
		eat(mark, &timer, l_fork, r_fork);
		printf()
		if ((!pth_times-- && g_sphil.times) || g_work_s.kill)
			return (NULL);
		sleeping(mark, &timer);
	}
	return (NULL);
}
