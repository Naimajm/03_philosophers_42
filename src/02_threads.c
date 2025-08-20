/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:06:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 23:47:12 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int			initialize_threads(t_data *data);
static void	*daily_routine(void * philo_node);
static void	*monitor_death(void *data);
static void	*monitor_meals(void *data);
int 		wait_for_threads(t_data *data);

int	initialize_threads(t_data *data)
{
	int index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	index = 0;
	data->philo_threads = (pthread_t *) malloc(sizeof(pthread_t) * data->num_philos);
	if (!data->philo_threads)
		return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);
	data->start_time = get_current_time();
	while (index < data->num_philos)
	{
        set_last_meal_time(&data->philos[index]);
		if (pthread_create(&data->philo_threads[index], NULL, &daily_routine, &data->philos[index]))
			return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);
		index++;
	}
	if (pthread_create(&data->monitor_death, NULL, &monitor_death, data))
		return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);
	if (data->num_meals > 0 && pthread_create(&data->monitor_meals, NULL, &monitor_meals, data))
		return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);
	return (SUCCESS);
}

static void	*daily_routine(void * philo_node)
{
	t_philo *philo;
	
	if (!philo_node)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	philo 	= (t_philo *) philo_node;
	if (philo->data->num_philos == 1)
	{
		printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		set_delay_time(philo->data->eat_time - 10);
	while (is_alive(philo) && is_program_active(philo->data))
	{
		thinking(philo);
		take_forks(philo);
		eating(philo);
		drop_forks(philo);
		if (has_eaten_enough(philo))
			break ;
		sleeping(philo);
	}
	return (NULL);
}

static void	*monitor_death(void *data_struct)
{
	t_data	*data;
	t_philo	*philo;
	int		index;
	long	current_time;
	long	time_since_last_meal;

	if (!data_struct)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	data 	= data_struct;
	philo	= data->philos;
	while (is_program_active(data))
	{
		index	= 0;
		while (index < data->num_philos)
		{
			current_time = get_current_time();
			time_since_last_meal = current_time - get_last_meal_time(&philo[index]);
			if (time_since_last_meal >= data->die_time)
					//&& get_philo_state(&philo[index]) != EATING)
			{				
				change_philo_state(&philo[index], DEAD);
				printing_logs(data, philo[index].id, MSG_DIED);
				stop_program(data);
				return (NULL);
			}
			index++;
		}
		usleep(USLEEP_MONITOR_TIME);
	}
	return (NULL);
}

static void	*monitor_meals(void *data_struct)
{
	t_data	*data;
	int		index;
	int		philos_satisfied;

	if (!data_struct)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), NULL);
	data 	= data_struct;
	while (is_program_active(data))
	{
		index 	= 0;
		philos_satisfied = 0;
		while (index < data->num_philos)
		{
			if (has_eaten_enough(&data->philos[index]))
				philos_satisfied++;
			index++;
		}
		if (philos_satisfied >= data->num_philos)
		{
			stop_program(data);
			return (NULL);
		}
		usleep(USLEEP_MONITOR_TIME);
	}
	return (NULL);
}

int wait_for_threads(t_data *data)
{
    int index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	if (pthread_join(data->monitor_death, NULL))
		return (FAILURE);
	if (data->num_meals > 0 && pthread_join(data->monitor_meals, NULL))
		return (FAILURE);
	index = 0;
    while (index < data->num_philos)
    {
        pthread_join(data->philo_threads[index], NULL);
        index++;
    }
    return (SUCCESS);
}
