/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:06:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/16 19:59:00 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void *daily_routine(void * args);
int wait_for_threads(t_data *data);

int	initialize_threads(t_data *data)
{
	int index;
	int	num_philos;
	t_philo	*philo;

	index = 0;
	num_philos = data->num_philos;
	philo = data->philos;
	// RESERVA MEMORIA THREADS
	data->philo_threads = (pthread_t *) malloc(sizeof(pthread_t) * num_philos);
	if (!data->philo_threads)
		return (ft_putendl_fd(ERROR_MEM_ALLOC, STDERR_FILENO), FAILURE);	
	
	// INICIO TEMPORIZADOR 
	data->start_time = my_watch();

	// CREACION THREADS -> FUNCION PPAL + ESTRUCTURA PHILO [INDICE]
	while (index < num_philos)
	{
		// INICIO HILOS PHILOS
		if (pthread_create(&data->philo_threads[index], NULL, daily_routine, philo))
			return (FAILURE);
		index++;
		philo++; 
	}
	return (SUCCESS);
}

// FUNCION PPAL EJECUCION HILO 
void *daily_routine(void * args)
{
	t_philo *philo = (t_philo *) args;
	t_data	*data	= philo->data;

	pthread_mutex_lock(&data->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO HILO [%i]\n", philo->id);
	pthread_mutex_unlock(&data->mutex->print_log);	
	
    usleep(100000);  // 100ms delay // Simular trabajo del filósofo
    
	pthread_mutex_lock(&data->mutex->print_log);
    printf("FIN HILO [%i]\n", philo->id);
	pthread_mutex_unlock(&data->mutex->print_log);

	t_philo *philo = (t_philo *)args;
    
    /* while (should_continue(philo))
    {
        think(philo);
        take_forks(philo);
        eat(philo);
        put_forks(philo);
        sleep_philo(philo);
    }
    return (NULL); */
	return (NULL);
}

// FUNCIÓN PARA ESPERAR TERMINACION HILOS
int wait_for_threads(t_data *data)
{
    int index = 0;
    
    while (index < data->num_philos)
    {
        pthread_join(data->philo_threads[index], NULL);
        index++;
    }
    return (SUCCESS);
}
