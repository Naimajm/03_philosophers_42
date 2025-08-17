/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:06:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/16 21:58:51 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// FUNCIONES CREACION Y CIERRE HILOS

void	*monitor_death(void *data);
void	*monitor_meals(void *data);
int 	wait_for_threads(t_data *data);

int	initialize_threads(t_data *data)
{
	int index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	index = 0;
	// RESERVA MEMORIA THREADS
	data->philo_threads = (pthread_t *) malloc(sizeof(pthread_t) * data->num_philos);
	if (!data->philo_threads)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO), FAILURE);	
	
	// INICIO TEMPORIZADOR 
	data->start_time = get_current_time();

	// CREACION THREADS PHILOS -> FUNCION PPAL + ESTRUCTURA PHILO [INDICE]
	while (index < data->num_philos)
	{
		if (pthread_create(&data->philo_threads[index], NULL, &daily_routine, &data->philos[index]))
			return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);	
		index++;
	}
	// CREACION THREADS MONITOR DEATH()
	if (pthread_create(&data->monitor_death, NULL, &monitor_death, data))
		return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);

	// CREACION THREADS MONITOR MEALS().-> se crea si hay limite en numero de comidas
	if (data->num_meals > 0 && pthread_create(&data->monitor_meals, NULL, &monitor_meals, data))
		return (ft_putendl_fd(ERROR_THREADS_CREATE, STDERR_FILENO), FAILURE);	
	return (SUCCESS);
}

void	*monitor_death(void *data)
{
	t_data	*data_ptr;

	data_ptr	= data;

	pthread_mutex_lock(&data_ptr->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO monitor_death()\n");
	pthread_mutex_unlock(&data_ptr->mutex->print_log);	
	
    usleep(100000);  										// simulacion 100ms delay 

	pthread_mutex_lock(&data_ptr->mutex->print_log);
    printf("FIN monitor_death()\n");
	pthread_mutex_unlock(&data_ptr->mutex->print_log);

	return (NULL);
}

void	*monitor_meals(void *data)
{
	t_data	*data_ptr;
	data_ptr = data;
	
	pthread_mutex_lock(&data_ptr->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO monitor_meals()\n");
	pthread_mutex_unlock(&data_ptr->mutex->print_log);	
	
    usleep(100000);  										// simulacion 100ms delay  
    
	pthread_mutex_lock(&data_ptr->mutex->print_log);
    printf("FIN monitor_meals()\n");
	pthread_mutex_unlock(&data_ptr->mutex->print_log);

	return (NULL);
}


// FUNCIÓN PARA ESPERAR TERMINACION HILOS
int wait_for_threads(t_data *data)
{
    int index;

	if (!data)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);    
	// ESPERA CIERRE HILO MONITOR DEATH()
	if (pthread_join(data->monitor_death, NULL))
		return (FAILURE);

	// ESPERA CIERRE HILO MONITOR MEALS()
	if (data->num_meals > 0 && pthread_join(data->monitor_meals, NULL))
		return (FAILURE);
	
	// ESPERA CIERRE HILOS PHILOS
	index = 0;
    while (index < data->num_philos)
    {
        pthread_join(data->philo_threads[index], NULL);
        index++;
    }
    return (SUCCESS);
}
