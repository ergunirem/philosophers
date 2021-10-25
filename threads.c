/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   threads.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: icikrikc <icikrikc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/21 09:07:20 by icikrikc      #+#    #+#                 */
/*   Updated: 2021/10/22 13:16:55 by icikrikc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*death_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (philo->data->death)
	{
		usleep(2000);
		if (philo->data->total_must_eat == philo->data->philo_num)
		{
			philo->data->death = 0;
			return (NULL);
		}
		if (!philo->is_eating && get_time_in_ms() - philo->last_meal
			> philo->data->time_to_die)
		{
			pthread_mutex_lock(&philo->eat);
			if (philo->data->death)
				print_status(philo, "died\n");
			philo->data->death = 0;
			pthread_mutex_unlock(&philo->eat);
			return (NULL);
		}
	}
	return (NULL);
}

static int	start_monitor_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_create(&(data->philos[i]->monitor),
				NULL, (void *)death_monitor, (void *)data->philos[i]) != 0)
			return (ERROR);
		i++;
	}
	i = -1;
	while (++i < data->philo_num)
		pthread_join(data->philos[i]->monitor, NULL);
	return (SUCCESS);
}

static int	start_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i]->last_meal = get_time_in_ms();
		if (pthread_create(&(data->philos[i]->thread), NULL,
				(void *)routine, (void *)data->philos[i]) != 0)
			return (ERROR);
		pthread_detach(data->philos[i]->thread);
		usleep(100);
		i++;
	}
	return (SUCCESS);
}

int	start_threads(t_data *data)
{
	data->start_time = get_time_in_ms();
	if (!start_philo_threads(data))
	{
		printf("Thread creation error\n");
		return (ERROR);
	}
	if (!start_monitor_threads(data))
	{
		printf("Thread creation error\n");
		return (ERROR);
	}
	return (SUCCESS);
}
