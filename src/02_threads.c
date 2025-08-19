/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_threads.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:06:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 21:29:40 by juagomez         ###   ########.fr       */
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
	
	if (!philo_node)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);	
	philo 	= (t_philo *) philo_node;	
	if (philo->data->num_philos == 1)			// caso especial -> 1 filosofo muere
	{	
		printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
		return (NULL);
	}	
	set_last_meal_time(philo);					// XQ ?

	// PRIORIDAD A PHILOS IMPARES -> EVITAR BLOQUEO ESTADO INICIAL
	if (philo->id % 2 == 0)		
		set_delay_time(philo->data->eat_time / 2);

	while (is_alive(philo) && is_program_active(philo->data))
	{
		thinking(philo);
		take_forks(philo);					// COGER TENEDORES
		eating(philo);
		drop_forks(philo);					// SOLTAR TENEDORES		
		if (has_eaten_enough(philo))		// verificacion OBJETIVO comidas
			break ;
		sleeping(philo);		
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

	if (!data_struct)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	data 	= data_struct;
	philo	= data->philos;	
	while (is_program_active(data))
	{
		index	= 0;		
		while (index < data->num_philos)		// CICLO MONITOREO PHILOS
		{
			current_time = get_current_time();
			time_since_last_meal = current_time - get_last_meal_time(&philo[index]);			
			if (time_since_last_meal >= data->die_time		// VERIFICAR MUERTE POR HAMBRE
					&& get_philo_state(&philo[index]) != EATING)
			{				
				change_philo_state(&philo[index], DEAD);	// PROCESO MUERTE POR HAMBRE -> IMPRIMIR + STATE	
				printing_logs(data, philo[index].id, MSG_DIED);				
				stop_program(data);							// terminar el ciclo de monitoreo	
				return (NULL);
			}		
			index++;
		}
		usleep(USLEEP_MONITOR_TIME);
	}	
	/* pthread_mutex_lock(&data->mutex->print_log);
    printf("FIN monitor_death()\n");
	pthread_mutex_unlock(&data->mutex->print_log); */
	return (NULL);
}

// MONITORIZAICON EXITOSA -> verifica si todos los filósofos han comido suficientes veces
void	*monitor_meals(void *data_struct)
{
	t_data	*data;	
	int		index;
	int		philos_satisfied;

	if (!data_struct)
		return (ft_putendl_fd(ERROR_INVALID_INPUT, STDERR_FILENO), FAILURE);
	data 	= data_struct;
	// CICLO EXTERNO VERIFICACION SUFICIENTES COMIDAS
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
		// CASO TODOS HAN COMIDO  -> OBJETIVO POR COMIDA . NO POR MUERTE
		if (philos_satisfied >= data->num_philos)
		{
			stop_program(data);

			/* pthread_mutex_lock(&data->mutex->print_log);
			printf("FIN monitor_meals() -> TODOS HAN COMIDO\n");
			pthread_mutex_unlock(&data->mutex->print_log); */
			return (NULL);
		}
		usleep(USLEEP_MONITOR_TIME);  	// ESPERA tiempo verificacion 
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
