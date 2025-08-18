/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:07:31 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/18 19:20:54 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int		eating(t_philo *philo);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

// FUNCION PPAL EJECUCION HILO 
void	*daily_routine(void * philo_node)
{
	t_philo *philo;	
	
	philo 	= (t_philo *) philo_node;
	
	set_last_meal(philo);

	// PRIORIDAD A PHILOS IMPARES -> EVITAR BLOQUEO ESTADO INICIAL
	if (philo->id % 2 == 0)		
		set_delay_time(philo->data->eat_time - 10);

	while (is_alive(philo) && is_program_active(philo->data))
	{
		take_forks(philo);						// COGER TENEDORES
		eating(philo);
		drop_forks(philo);					// SOLTAR TENEDORES

		// verificacion OBJETIVO comidas
		if (!has_eaten_enough(philo))
			break ;

		// sleeping(philo);
		// thinking(philo);
	}    	
	return (NULL);
}

int	eating(t_philo *philo)
{
	if (!philo)
		return (FAILURE);		
	
	change_philo_state(philo, EATING);		// COMER -> cambio status	
	printing_logs(philo->data, philo->id, MSG_EATING);	// log		
	set_delay_time(philo->data->eat_time);	// proceso comer discurrir tiempo de comida
	
	set_last_meal(philo);					// actualizar tiempo ultima comida		
	increment_num_meals(philo);				// incrementar numero comidas realizadas	

	return (SUCCESS);
}

// ACCIONES FORKS -------------------------------

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	(void) philo;
	printf("take_forks() philo [%i]\n", philo->id);
	
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
	printf("drop_forks() philo [%i]\n", philo->id);

	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}




