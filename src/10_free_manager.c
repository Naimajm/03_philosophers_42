/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_free_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:28:37 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 01:14:09 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	free_philos(t_data *data);
void	free_mutex(t_data *data);

// limpieza GLOBAL DATA 
void cleanup_data(t_data *data)
{
	if (!data)
		return ;
	
	// MUTEX
	free_mutex(data);

	// PHILOS
	free_philos(data);
	
	// THREADS
	free(data->philo_threads);
	data->philo_threads 	= NULL;
	data->monitor_death 	= 0;
	data->monitor_meals 	= 0;	

	free(data);
	printf(FREE_ALL);
}

// LIMPIEZA ESTRUCTURA PHILOS
void	free_philos(t_data *data)
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
		// liberar punteros fork_left y fork_right -> Se liberarán en free_mutex()
		// state enum no necesta liberacion
		//free(&data->philos[index].data);
		index++;
	}
	free(data->philos);
	data->philos = NULL;
}

// LIMPIEZA ESTRUCTURA MUTEX
void	free_mutex(t_data *data)
{	
	int	index;

	if (!data || !data->mutex)
		return ;
	// DESTROY MUTEX
	pthread_mutex_destroy(&data->mutex->num_full_philos);
	pthread_mutex_destroy(&data->mutex->program_active);
	pthread_mutex_destroy(&data->mutex->print_log);
	
	// ARRAY MUTEX FORKS
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



