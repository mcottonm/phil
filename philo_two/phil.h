/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 21:45:25 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/10 20:26:16 by mcottonm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHIL_H
# define PHIL_H

# include <stdlib.h>
# include <stddef.h>
# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <time.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <string.h>

# define TIME_TO_START 1000
# define DELTA_TIME 50
# define TIMES_FLAG -42
# define CHECK_TINE 20
# define SEM_NAME_F "fork"
# define SEM_NAME_LOG "log_semaphore"

typedef struct		s_phil
{
	int				nbr_of_phil;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times;
}					t_phil;

typedef struct		s_work_strc
{
	bool			kill;
	char			queue[16384];
	int				qu_i;
	long			start;
	int				phil_check[201];
	sem_t			*forks;
	sem_t			*log_sem;
}					t_work;

t_phil		g_sphil;
t_work		g_work_s;

void				add_to_queue(long mark, int stat);
void				*logger(void *vd);
void				*start_thread(void *void_ptr);
void				control(void);

void				ft_exit(int i);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
int					ft_atoi(const char *str);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
long				timer_now(void);
void				pth_sleep(long time_to);

#endif
