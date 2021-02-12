/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phil.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcottonm <mcottonm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/04 21:45:25 by mcottonm          #+#    #+#             */
/*   Updated: 2021/02/12 18:38:44 by mcottonm         ###   ########.fr       */
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

# define TIME_TO_START 1000
# define DELTA_TIME 50
# define CHECK_TINE 5
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
	long			start;
	long			phil_check;
	int				pid[202];
	pthread_t		phil;
	sem_t			*fork;
	sem_t			*log_sem;
}					t_work;

t_phil		g_sphil;
t_work		g_work_s;

void				add_to_queue(int mark, int stat);
void				*logger(void *vd);
void				*phil_start(void *void_ptr);
void				control(int mark);
bool				ft_exit(int i);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
int					ft_atoi(const char *str);
int					ft_strncmp(const char *str1, const char *str2, size_t n);
long				timer_now(void);
void				pth_sleep(long time_to);

#endif
