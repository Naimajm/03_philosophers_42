/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:06:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 01:50:45 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// FUNCIONES CREACION Y CIERRE HILOS

void	*daily_routine(void * philo_node);
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

// FUNCION PPAL EJECUCION HILO 
void	*daily_routine(void * philo_node)
{
	t_philo *philo;	
	
	philo 	= (t_philo *) philo_node;
	
	// caso especial -> 1 filosofo muere
	if (philo->data->num_philos == 1)
	{	
		set_delay_time(philo->data->die_time);
		change_philo_state(philo, DEAD);
		printing_logs(philo->data, philo->id, MSG_DIED);	// log
		return (NULL);
	}			

	set_last_meal_time(philo);					// XQ ?

	// PRIORIDAD A PHILOS IMPARES -> EVITAR BLOQUEO ESTADO INICIAL
	if (philo->id % 2 == 0)		
		set_delay_time(philo->data->eat_time / 2);

	while (is_alive(philo) && is_program_active(philo->data))
	{
		take_forks(philo);					// COGER TENEDORES
		eating(philo);
		drop_forks(philo);					// SOLTAR TENEDORES
		
		if (philo_has_eaten_enough(philo))		// verificacion OBJETIVO comidas
			break ;
		sleeping(philo);
		thinking(philo);
	}    	
	return (NULL);
}

// MONITORIZAICON FRACASO -> verifica si algun filósofo han muerto de hambre
void	*monitor_death(void *data_struct)
{
	t_data	*data;
	t_philo	*philo;
	int		index;
	long	current_time;
	long	time_since_last_meal;

	data 	= data_struct;
	philo	= data->philos;	

	pthread_mutex_lock(&data->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO monitor_death()\n");
	pthread_mutex_unlock(&data->mutex->print_log);	

	while (is_program_active(data))
	{
		index	= 0;
		// CICLO MONITOREO PHILOS
		while (index < data->num_philos)
		{
			current_time = get_current_time();
			time_since_last_meal = current_time - get_last_meal_time(&philo[index]);

			// VERIFICAR MUERTE POR HAMBRE
			if (time_since_last_meal > data->die_time 
					&& get_philo_state(&philo[index]) != EATING)
			{
				// PROCESO MUERTE POR HAMBRE -> IMPRIMIR + STATE
				change_philo_state(&philo[index], DEAD);		
				printing_logs(philo->data, philo->id, MSG_DIED);
				stop_program(data);			// program_active = false terminar el ciclo de monitoreo				
				return (NULL);
			}		
			index++;
		}
		usleep(USLEEP_MONITOR_TIME);
	}	
	pthread_mutex_lock(&data->mutex->print_log);
    printf("FIN monitor_death()\n");
	pthread_mutex_unlock(&data->mutex->print_log);
	return (NULL);
}

// MONITORIZAICON EXITOSA -> verifica si todos los filósofos han comido suficientes veces
void	*monitor_meals(void *data_struct)
{
	t_data	*data;
	t_philo	*philo;
	int		index;		

	data 	= data_struct;
	philo	= data->philos;
	index 	= 0;

	pthread_mutex_lock(&data->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO monitor_death()\n");
	pthread_mutex_unlock(&data->mutex->print_log);

	// CICLO VERIFICACION SUFICIENTES COMIDAS
	while (index < data->num_philos && is_program_active(data))
	{
		usleep(USLEEP_MONITOR_TIME);			// tiempo verificacion  Evitar consumir CPU excesivamente
		if (philo_has_eaten_enough(&philo[index]) == false)
			index = 0;							// reiniciar para verificar desde inicio
		index++;
	}
	// CASO TODOS HAN COMIDO  -> OBJETIVO POR COMIDA . NO POR MUERTE
	if (is_program_active(data))
	{
		stop_program(data);
		//program_is_over(data);

		pthread_mutex_lock(&data->mutex->print_log);
    	printf("FIN monitor_death() -> TODOS HAN COMIDO\n");
		pthread_mutex_unlock(&data->mutex->print_log);
	}	
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
