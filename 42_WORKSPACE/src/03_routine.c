/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:07:31 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 21:30:32 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int		thinking(t_philo *philo);
int		sleeping(t_philo *philo);
int		eating(t_philo *philo);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

int	thinking(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);	
	// 	PROCESO
	change_philo_state(philo, THINKING);					// DORMIR -> cambio status
	printing_logs(philo->data, philo->id, MSG_THINKING);	// log
	return (SUCCESS);
}

int	sleeping(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);	
	// 	PROCESO
	change_philo_state(philo, SLEEPING);					// DORMIR -> cambio status
	printing_logs(philo->data, philo->id, MSG_SLEEPING);	// log
	set_delay_time(philo->data->sleep_time);				// proceso comer discurrir tiempo de comida	
	return (SUCCESS);
}

int	eating(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);		
	// 	PROCESO
	change_philo_state(philo, EATING);		// COMER -> cambio status	
	printing_logs(philo->data, philo->id, MSG_EATING);	// log		
	set_delay_time(philo->data->eat_time);	// proceso comer discurrir tiempo de comida
	
	set_last_meal_time(philo);				// actualizar tiempo ultima comida		
	increment_num_meals(philo);				// incrementar numero comidas realizadas	
	return (SUCCESS);
}

// ACCIONES FORKS -------------------------------

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (!philo || !is_alive(philo))
		return ;	
	// GARANTIA SEGURA DEADLOCK -> orden por direcciones memoria ¡¡
	if (philo->left_fork < philo->right_fork)
	{
		first_fork 	= philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork 	= philo->right_fork;
		second_fork = philo->left_fork;
	}
	// accion coger + imprimir log
	pthread_mutex_lock(first_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
	pthread_mutex_lock(second_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
}

void	drop_forks(t_philo *philo)
{
	/* pthread_mutex_lock(&philo->data->mutex->print_log);
    printf("drop_forks() philo [%d]\n", philo->id);
    pthread_mutex_unlock(&philo->data->mutex->print_log); */
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}






