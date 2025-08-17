/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:07:31 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 18:35:53 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// FUNCION PPAL EJECUCION HILO 
void	*daily_routine(void * philo_node)
{
	t_philo *philo;	
	
	philo 	= (t_philo *) philo_node;
	


	/* t_data	*data;
	data	= (t_data *) philo->data;
	pthread_mutex_lock(&data->mutex->print_log);		// ACTIVAR MUTEX
	printf("INICIO PHILO [%i]\n", philo->id);
	pthread_mutex_unlock(&data->mutex->print_log);	
	
    usleep(100000);  // 100ms delay // Simular trabajo del filósofo
    
	pthread_mutex_lock(&data->mutex->print_log);
    printf("FIN PHILO [%i]\n", philo->id);
	pthread_mutex_unlock(&data->mutex->print_log); */

	    
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

