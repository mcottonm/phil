/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:55:35 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/10 18:27:46 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

static void	add_messg(int stat)
{
	const char	*str;
	int			i;

	i = 0;
	if (stat == 1)
		str = " is thinking\n";
	if (stat == 2)
		str = " is eating\n";
	if (stat == 3)
		str = " is sleeping\n";
	if (stat == 4)
		str = " has taken a fork\n";
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

	pthread_mutex_lock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
	if (g_work_s.kill)
	{
		pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
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
	pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
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
		pthread_mutex_lock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
		if (g_work_s.qu_i)
			write(1, g_work_s.queue, g_work_s.qu_i);
		g_work_s.qu_i = 0;
		pthread_mutex_unlock(&(g_work_s.lock[g_sphil.nbr_of_phil + 1]));
		timer += DELTA_TIME * 2;
		pth_sleep(timer);
	}
	return (vd);
}
