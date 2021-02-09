/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 18:50:33 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/09 23:18:04 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static bool		cntr_chk(int i)
{
	if ((g_work_s.phil_check -= DELTA_TIME * 2) < 0)
	{
		g_work_s.kill = true;
		printf("%-10ld %d is died\n", timer_now() - g_work_s.start, ++i);
		return (false);
	}
	return (true);
}

void			control(int mark)
{
	int			i;
	long		timer_d;
	pthread_t	phil;

	timer_d = g_work_s.start + g_sphil.time_to_die;
	if (pthread_create(&phil, NULL, phil_start, (void*)(long)mark))
		exit(1);
	pthread_detach(phil);
	pth_sleep(timer_d);
	while (!g_work_s.kill)
	{
		i = -1;
		sem_wait(g_work_s.log_sem);
		if (!cntr_chk(i))
			break ;
		sem_post(g_work_s.log_sem);
		timer_d += DELTA_TIME * 2;
		pth_sleep(timer_d);
	}
	exit(1);
}
