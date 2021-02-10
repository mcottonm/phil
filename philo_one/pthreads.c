/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pthreads.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:50:08 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/10 18:31:26 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	think(long mark, const int l_fork, const int r_fork)
{
	add_to_queue(mark, 1);
	pthread_mutex_lock(&(g_work_s.lock[l_fork]));
	add_to_queue(mark, 4);
	pthread_mutex_lock(&(g_work_s.lock[r_fork]));
	add_to_queue(mark, 4);
}

static void	eat(long mark, long *timer, const int l_fork, const int r_fork)
{
	*timer = timer_now();
	g_work_s.phil_check[mark] = 0;
	g_work_s.phil_check[mark] = g_sphil.time_to_die;
	if (g_work_s.kill)
	{
		pthread_mutex_unlock(&(g_work_s.lock[l_fork]));
		pthread_mutex_unlock(&(g_work_s.lock[r_fork]));
		return ;
	}
	add_to_queue(mark, 2);
	*timer += g_sphil.time_to_eat;
	pth_sleep(*timer);
}

static void	sleeping(long mark, long *timer, const int l_fork, const int r_fork)
{
	add_to_queue(mark, 3);
	*timer += g_sphil.time_to_sleep;
	pthread_mutex_unlock(&(g_work_s.lock[l_fork]));
	pthread_mutex_unlock(&(g_work_s.lock[r_fork]));
	pth_sleep(*timer);
}

static bool	times_check(int mark, int *pth_times,
const int l_fork, const int r_fork)
{
	if ((!--(*pth_times) && g_sphil.times) || g_work_s.kill)
	{
		pthread_mutex_unlock(&(g_work_s.lock[l_fork]));
		pthread_mutex_unlock(&(g_work_s.lock[r_fork]));
		g_work_s.phil_check[mark] = TIMES_FLAG;
		return (false);
	}
	return (true);
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
	if (mark % 2 && !(g_sphil.nbr_of_phil % 2))
		pth_sleep(timer_now() + DELTA_TIME);
	while (!g_work_s.kill)
	{
		if (g_work_s.kill)
			return (NULL);
		if (mark % 2 && (g_sphil.nbr_of_phil % 2))
			pth_sleep(timer_now() + 10);
		think(mark, l_fork, r_fork);
		eat(mark, &timer, l_fork, r_fork);
		if (!times_check(mark, &pth_times, l_fork, r_fork))
			return (NULL);
		sleeping(mark, &timer, l_fork, r_fork);
	}
	return (NULL);
}
