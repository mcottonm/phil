/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 22:58:03 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/05 18:52:29 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phil.h"

void	ft_exit(int i)
{
	if (i == 1)
		printf("ERROR: wrong number of arguments\n");
	if (i == 2)
		printf("ERROR: too many threads\n");
	exit(1);
}

int		ft_strncmp(const char *str1, const char *str2, size_t n)
{
	const unsigned char	*s1 = (unsigned char*)str1;
	const unsigned char	*s2 = (unsigned char*)str2;

	while (*s1 == *s2 && n && *s1)
	{
		s1++;
		s2++;
		n--;
	}
	if (!n)
		return (0);
	return (*s1 - *s2);
}

int		ft_atoi(const char *str)
{
	int			i;
	int			at;
	int			p;

	i = 1;
	at = 0;
	if (!ft_strncmp(str, "-2147483648", 12))
		return (-2147483648);
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '-')
		i = -1;
	if ((*str == '+') || (*str == '-'))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		p = at;
		at = at * 10 - '0' + *str++;
		if ((p != at / 10 || at < 0))
			return ((i > 0) ? -1 : 0);
	}
	return (at * i);
}

long	timer_now(void)
{
	struct timeval	val;

	gettimeofday(&val, NULL);
	return (val.tv_sec * 1000 + val.tv_usec / 1000);
}

void	pth_sleep(long time_to)
{
	while (true)
	{
		if (timer_now() >= time_to)
			return ;
		usleep(DELTA_TIME);
	}
}
