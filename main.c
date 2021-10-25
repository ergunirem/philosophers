/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: icikrikc <icikrikc@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/16 15:54:05 by icikrikc      #+#    #+#                 */
/*   Updated: 2021/10/22 18:08:43 by icikrikc      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	clear_all(t_data *data)
{
	int	i;

	if (!data)
		return (EXIT_FAILURE);
	pthread_mutex_destroy(&data->print);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&data->forks[i]);
	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_destroy(&data->philos[i]->eat);
	if (data->philos)
	{
		i = -1;
		while (++i < data->philo_num && data->philos[i])
			free(data->philos[i]);
		free(data->philos);
	}
	free(data->forks);
	free(data);
	return (EXIT_FAILURE);
}

static int	init_philos_and_mutexes(t_data *data, int i)
{
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (ERROR);
	i = -1;
	while (++i < data->philo_num)
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (ERROR);
	data->philos = (t_philo **)malloc(sizeof(t_philo *) * data->philo_num);
	if (!data->philos)
		return (ERROR);
	i = -1;
	while (++i < data->philo_num)
	{
		data->philos[i] = (t_philo *)malloc(sizeof(t_philo));
		if (!data->philos[i])
			return (ERROR);
		data->philos[i]->data = data;
		data->philos[i]->no = i + 1;
		data->philos[i]->l_fork = i;
		data->philos[i]->r_fork = (i + 1) % data->philo_num;
		data->philos[i]->meal_count = 0;
		data->philos[i]->last_meal = 0;
		if (pthread_mutex_init(&data->philos[i]->eat, NULL) != 0)
			return (ERROR);
	}
	return (SUCCESS);
}

static t_data	*init_philo_data(int argc, char **argv)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->philo_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->death = 1;
	data->must_eat_count = 0;
	data->total_must_eat = 0;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (NULL);
	if (!init_philos_and_mutexes(data, 0))
		return (NULL);
	return (data);
}

static int	check_arguments(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (printf("Error: Argument amount is invalid\n"));
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0 || ft_isnumber(argv[i]))
			return (printf("Error: Invalid argument\n"));
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (check_arguments(argc, argv))
		return (EXIT_FAILURE);
	data = init_philo_data(argc, argv);
	if (!data)
		return (clear_all(data));
	if (!start_threads(data))
		return (clear_all(data));
	clear_all(data);
	// system("leaks philo");
	return (EXIT_SUCCESS);
}
