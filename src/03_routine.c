/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:07:31 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/18 18:46:53 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int		eating(t_philo *philo);
void	take_forks(t_philo *philo);

// FUNCION PPAL EJECUCION HILO 
void	*daily_routine(void * philo_node)
{
	t_philo *philo;	
	
	philo 	= (t_philo *) philo_node;
	
	set_last_meal(philo);

	// PRIORIDAD A PHILOS IMPARES -> EVITAR BLOQUEO ESTADO INICIAL
	if (philo->id % 2 == 0)		
		set_delay_time(philo->data->eat_time - 10);

	while (is_alive(philo))
	{
		eating(philo);

		// verificacion objetivo comidas
		if (!has_eaten_enough(philo))
			break ;

	}    
    /* while (get_state(philo) != DIED)
    {        
        take_forks(philo);
        eat(philo);
        put_forks(philo);
        sleep_philo(philo);
		think(philo);
    } */
	return (NULL);
}

int	eating(t_philo *philo)
{
	if (!philo)
		return (FAILURE);	
	take_forks(philo);					// COGER TENEDORES
	
	change_philo_state(philo, EATING);	// COMER -> cambio status	
	
	printing_logs(philo->data, philo->id, MSG_EATING);	// log		
	
	set_last_meal(philo);				// actualizar tiempo ultima comida
	
	increment_num_meals_eaten(philo);	// incrementar numero comidas realizadas

	// SOLTAR TENEDORES

	return (SUCCESS);
}

// ACCIONES FORKS -------------------------------

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	(void) philo;
	printf("take_forks\n");
	
	// caso especial -> 1 filosofo
	/* if (philo->data->num_philos)
		case_one_philo() */

	// GARANTIA SEGURA DEADLOCK -> orden por direcciones memoria ¡¡
	if (philo->fork_left < philo->fork_right)
	{
		first_fork 	= philo->fork_left;
		second_fork = philo->fork_right;
	}
	else
	{
		first_fork 	= philo->fork_right;
		second_fork = philo->fork_left;
	}
	// accion coger + imprimir log
	pthread_mutex_lock(first_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
	pthread_mutex_lock(second_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
}

void	drop_forks(t_philo *philo)
{
	(void) philo;
	printf("drop_forks\n");



	

}




