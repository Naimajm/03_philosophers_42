/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:08:04 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/16 19:59:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_data	*initialize_data(int argc, char **argv);
void	initialize_philos(t_data *data);
int 	assign_forks_to_philos(t_data *data);
void	initialize_mutex(t_data *data);

t_data	*initialize_data(int argc, char **argv)
{
	t_data	*data;

	data = (t_data *) malloc(sizeof(t_data));
	if (!data)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO), NULL);

	data->num_philos 	= (int) ft_atoi(argv[1]);
	data->die_time		= (int) ft_atoi(argv[2]);
	data->eat_time		= (int) ft_atoi(argv[3]);
	data->sleep_time	= (int) ft_atoi(argv[4]);	
	if (argc == 6)							// configurar num comidas (opcional [5])
		data->num_meals = (int) ft_atoi(argv[5]);
	else
		data->num_meals = -1; 				// comidas infinitas
	data->num_full 		= 0;
	data->start_time	= 0;
	data->keep_iter 	= true;

	initialize_mutex(data);

	data->philos		= NULL;				// INIT ARRAY PHILOS
	
	data->monitor_death 	= 0;			// INIT THREADS	
	data->monitor_meals	= 0;
	data->philo_threads		= NULL;	
	
	return (data);
}

void	initialize_philos(t_data *data)
{
	int	index;
	t_philo	*philos;
		
	data->philos = (t_philo *) malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO));	

	index = 0;
	philos = data->philos;	
	while (index < data->num_philos)
	{
		philos->id 			= index + 1;
		philos->num_meals	= 0;
		philos->state		= INITIAL;
		pthread_mutex_init(&philos->mutex_num_meals, NULL);
		pthread_mutex_init(&philos->mutex_last_eat, NULL);
		pthread_mutex_init(&philos->mutex_state, NULL);	
		//philos_ptr->data		= data;
		set_last_meal(philos);
		philos->data = data;
		index++;
		philos++;
	}
}

int assign_forks_to_philos(t_data *data)
{
	int	index;
	t_philo	*philos_ptr;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	
	philos_ptr = data->philos;
	// CASO 1º PHILO
	philos_ptr->fork_left = &data->mutex->forks[0];
	philos_ptr->fork_right = &data->mutex->forks[data->num_philos - 1]; // ULTIMO TENEDOR !!
	philos_ptr++;

	index = 1;
	// RESTO CASOS
	while (index < data->num_philos)
	{
		philos_ptr->fork_left = &data->mutex->forks[index];
		philos_ptr->fork_right = &data->mutex->forks[index - 1];
		index++;
		philos_ptr++;
	}	
	return (SUCCESS);
}

void	initialize_mutex(t_data *data)
{
	int	index;

	index = 0;
	data->mutex = (t_mutex *) malloc(sizeof(t_mutex));
	if (!data->mutex)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO));
	// INIT MUTEX
	pthread_mutex_init(&data->mutex->die_time, NULL);
	pthread_mutex_init(&data->mutex->eat_time, NULL);
	pthread_mutex_init(&data->mutex->sleep_time, NULL);
	pthread_mutex_init(&data->mutex->start_time, NULL);
	pthread_mutex_init(&data->mutex->keep_iter, NULL);
	pthread_mutex_init(&data->mutex->print_log, NULL);
	
	// ARRAY MUTEX FORKS
	data->mutex->forks = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->mutex->forks)
	{
		free(data->mutex);
		ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO);
		return ;
	}
	while (index < data->num_philos)
	{
		pthread_mutex_init(&data->mutex->forks[index], NULL);
		index++;
	}
}