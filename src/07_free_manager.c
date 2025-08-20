/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   07_free_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:28:37 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 18:18:54 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void		cleanup_data(t_data *data);
static void	free_philos(t_data *data);
static void	free_mutex(t_data *data);

void	cleanup_data(t_data *data)
{
	if (!data)
		return ;
	free_mutex(data);
	free_philos(data);
	free(data->philo_threads);
	data->philo_threads = NULL;
	data->monitor_death = 0;
	data->monitor_meals = 0;
	free(data);
}

static void	free_philos(t_data *data)
{
	int	index;

	if (!data)
		return ;
	index = 0;
	while (index < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[index].mutex_num_meals);
		pthread_mutex_destroy(&data->philos[index].mutex_last_meal_time);
		pthread_mutex_destroy(&data->philos[index].mutex_state);
		index++;
	}
	free(data->philos);
	data->philos = NULL;
}

static void	free_mutex(t_data *data)
{
	int	index;

	if (!data || !data->mutex)
		return ;
	pthread_mutex_destroy(&data->mutex->program_active);
	pthread_mutex_destroy(&data->mutex->print_log);
	index = 0;
	while (index < data->num_philos)
	{
		pthread_mutex_destroy(&data->mutex->forks[index]);
		index++;
	}
	free(data->mutex->forks);
	data->mutex->forks = NULL;
	free(data->mutex);
	data->mutex = NULL;
}
