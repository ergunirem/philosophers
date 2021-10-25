/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: icikrikc <icikrikc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/16 16:08:09 by icikrikc      #+#    #+#                 */
/*   Updated: 2021/10/22 13:15:28 by icikrikc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

# define ERROR 0
# define SUCCESS 1

typedef struct s_philo
{
	int				no;
	int				l_fork;
	int				r_fork;
	int				meal_count;
	int				is_eating;
	uint64_t		last_meal;
	pthread_t		monitor;
	pthread_t		thread;
	pthread_mutex_t	eat;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	t_philo			**philos;
	int				death;
	int				must_eat_count;
	int				total_must_eat;
	int				philo_num;
	uint64_t		start_time;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
}	t_data;

int			ft_atoi(const char *str);
int			ft_isnumber(char *str);
uint64_t	get_time_in_ms(void);
void		print_status(t_philo *philo, char *msg);
int			start_threads(t_data *data);
void		*routine(void *arg);
void		smart_sleep(uint64_t ms);

#endif
