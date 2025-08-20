/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:08:04 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 23:50:24 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_data		*initialize_data(int argc, char **argv);
void		initialize_philos(t_data *data);
int 		assign_forks_to_philos(t_data *data);
static void	initialize_mutex(t_data *data);

t_data	*initialize_data(int argc, char **argv)
{
	t_data	*data;

	if (!argc || !argv)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	data = (t_data *) malloc(sizeof(t_data));
	if (!data)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO), NULL);
	data->num_philos 	= (int) ft_atoi(argv[1]);
	data->die_time		= (int) ft_atoi(argv[2]);
	data->eat_time		= (int) ft_atoi(argv[3]);
	data->sleep_time	= (int) ft_atoi(argv[4]);	
	if (argc == 6)
		data->num_meals = (int) ft_atoi(argv[5]);
	else
		data->num_meals = -1;
	data->start_time	= 0;
	data->program_active 	= true;
	initialize_mutex(data);
	data->philos		= NULL;
	data->monitor_death = 0;
	data->monitor_meals	= 0;
	data->philo_threads	= NULL;
	return (data);
}

void	initialize_philos(t_data *data)
{
	int	index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));		
	data->philos = (t_philo *) malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO));	
	index = 0;
	while (index < data->num_philos)
	{
		data->philos[index].id			= index + 1;
		data->philos[index].num_meals	= 0;
		data->philos[index].state		= INITIAL;
		data->philos[index].last_meal_time = 0;
		pthread_mutex_init(&data->philos[index].mutex_num_meals, NULL);
		pthread_mutex_init(&data->philos[index].mutex_last_meal_time, NULL);
		pthread_mutex_init(&data->philos[index].mutex_state, NULL);
		data->philos[index].data = data;
		index++;
	}
}

int assign_forks_to_philos(t_data *data)
{
	int	index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	data->philos[0].left_fork 	= &data->mutex->forks[0];
	data->philos[0].right_fork  = &data->mutex->forks[data->num_philos - 1];
	index = 1;
	while (index < data->num_philos)
	{
		data->philos[index].left_fork 	= &data->mutex->forks[index];
		data->philos[index].right_fork 	= &data->mutex->forks[index - 1];
		index++;
	}
	return (SUCCESS);
}

static void	initialize_mutex(t_data *data)
{
	int	index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO));
	index = 0;
	data->mutex = (t_mutex *) malloc(sizeof(t_mutex));
	if (!data->mutex)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO));
	pthread_mutex_init(&data->mutex->program_active, NULL);
	pthread_mutex_init(&data->mutex->print_log, NULL);
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
