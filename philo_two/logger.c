/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:55:35 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/09 17:01:32 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	add_messg(int stat)
{
	const char	*str_thnk = " is thinking\n";
	const char	*str_eat = " is eating\n";
	const char	*str_slp = " is sleeping\n";
	const char	*str;
	int			i;

	i = 0;
	if (stat == 1)
		str = str_thnk;
	if (stat == 2)
		str = str_eat;
	if (stat == 3)
		str = str_slp;
	while (str[i])
		g_work_s.queue[g_work_s.qu_i++] = str[i++];
}

static void	add_num(int num, int *width)
{
	int	tmp;
	int	len;

	tmp = num;
	len = 1;
	while (tmp /= 10)
		++len;
	tmp = len;
	while (len)
	{
		g_work_s.queue[g_work_s.qu_i + len - 1] = num % 10 + '0';
		num /= 10;
		--len;
		--*width;
	}
	g_work_s.qu_i += tmp;
}

void		add_to_queue(long mark, int stat)
{
	long	time;
	int		width;

	sem_wait(g_work_s.log_sem);
	if (g_work_s.kill)
	{
		sem_post(g_work_s.log_sem);
		return ;
	}
	width = 15;
	time = timer_now() - g_work_s.start;
	g_work_s.queue[g_work_s.qu_i++] = '[';
	add_num(time, &width);
	g_work_s.queue[g_work_s.qu_i++] = ']';
	memset(g_work_s.queue + g_work_s.qu_i, 32, width);
	g_work_s.qu_i += width;
	add_num(mark + 1, &width);
	add_messg(stat);
	sem_post(g_work_s.log_sem);
}

void		*logger(void *vd)
{
	long	timer;

	pth_sleep(g_work_s.start);
	timer = g_work_s.start;
	while (!g_work_s.kill)
	{
		if (g_work_s.kill)
			return (NULL);
		sem_wait(g_work_s.log_sem);
		if (g_work_s.qu_i)
			write(1, g_work_s.queue, g_work_s.qu_i);
		g_work_s.qu_i = 0;
		sem_post(g_work_s.log_sem);
		timer += DELTA_TIME * 2;
		pth_sleep(timer);
	}
	return (vd);
}
