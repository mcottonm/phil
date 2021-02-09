/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:55:35 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/09 22:14:21 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void		add_to_queue(int mark, int stat)
{
	long	time;

	sem_wait(g_work_s.log_sem);
	if (g_work_s.kill)
	{
		sem_post(g_work_s.log_sem);
		return ;
	}
	time = timer_now() - g_work_s.start;
	if (stat == 1)
		printf("%-10ld %d is thinking\n", time, mark + 1);
	if (stat == 2)
		printf("%-10ld %d is eating\n", time, mark + 1);
	if (stat == 3)
		printf("%-10ld %d is sleeping\n", time, mark + 1);
	sem_post(g_work_s.log_sem);
}
