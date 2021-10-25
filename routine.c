/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: icikrikc <icikrikc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/27 13:15:16 by icikrikc      #+#    #+#                 */
/*   Updated: 2021/10/22 13:48:50 by icikrikc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
Since usleep is very inaccurate, it may sleep more or less than you want.
This may lead to philosophers dying if you're unlucky.
If you usleep for tiny portions at a time, it'll increase accuracy.
*/

void	smart_sleep(uint64_t ms)
{
	uint64_t	entry;

	entry = get_time_in_ms();
	while ((get_time_in_ms() - entry) < ms)
		usleep(100);
}

void	print_status(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->print);
	printf("%lld %d %s", get_time_in_ms() - philo->data->start_time,
		philo->no, msg);
	pthread_mutex_unlock(&philo->data->print);
}

static void	eating(t_philo	*philo)
{
	pthread_mutex_t	*forks;

	forks = philo->data->forks;
	pthread_mutex_lock(&forks[philo->l_fork]);
	print_status(philo, "has taken a fork\n");
	pthread_mutex_lock(&forks[philo->r_fork]);
	print_status(philo, "has taken a fork\n");
	pthread_mutex_lock(&philo->eat);
	philo->last_meal = get_time_in_ms();
	philo->is_eating = 1;
	print_status(philo, "is eating\n");
	if (philo->data->death)
		smart_sleep(philo->data->time_to_eat);
	philo->meal_count++;
	philo->is_eating = 0;
	pthread_mutex_unlock(&philo->eat);
	pthread_mutex_unlock(&forks[philo->l_fork]);
	pthread_mutex_unlock(&forks[philo->r_fork]);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->data->death)
	{
		eating(philo);
		if (philo->meal_count == philo->data->must_eat_count)
		{
			philo->data->total_must_eat++;
			return (NULL);
		}
		print_status(philo, "is sleeping\n");
		smart_sleep(philo->data->time_to_sleep);
		print_status(philo, "is thinking\n");
	}
	return (NULL);
}
