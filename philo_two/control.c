/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 18:50:33 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/05 18:52:19 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static bool		cntr_chk(int i)
{
	int	zring;
	int	j;

	if (g_work_s.phil_check[i] == TIMES_FLAG)
	{
		j = -1;
		zring = TIMES_FLAG;
		while (++j < g_sphil.nbr_of_phil)
			zring = g_work_s.phil_check[i]
			> zring ? g_work_s.phil_check[i] : zring;
		if (zring == TIMES_FLAG)
		{
			pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
			return (false);
		}
	}
	else if ((g_work_s.phil_check[i] -= DELTA_TIME * 2) < 0)
	{
		g_work_s.kill = true;
		printf("[%ld] %d is died\n", timer_now() - g_work_s.start, ++i);
		pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
		return (false);
	}
	return (true);
}

void			control(void)
{
	int		i;
	long	timer_d;

	timer_d = g_work_s.start + g_sphil.time_to_die;
	pth_sleep(timer_d);
	while (!g_work_s.kill)
	{
		i = -1;
		while (++i < g_sphil.nbr_of_phil)
		{
			pthread_mutex_lock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
			if (!cntr_chk(i))
				return ;
			pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
		}
		timer_d += DELTA_TIME * 2;
		pth_sleep(timer_d);
	}
}
