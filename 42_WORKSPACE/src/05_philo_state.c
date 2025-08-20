/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_philo_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:31:42 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 23:25:02 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool 	is_alive(t_philo *philo);
void	change_philo_state(t_philo *philo, t_state new_state);
t_state	get_philo_state(t_philo *philo);

// FUNCIONES GESTION STATE  

bool is_alive(t_philo *philo)
{
	if (get_philo_state(philo) != DEAD)
		return (true);
    return (false);
}

void	change_philo_state(t_philo *philo, t_state new_state)
{
	pthread_mutex_lock(&philo->mutex_state);
	if (philo->state != DEAD)
		philo->state = new_state;
	pthread_mutex_unlock(&philo->mutex_state);
}

t_state	get_philo_state(t_philo *philo)
{	
	t_state state;

	if (!philo)
		return (DEAD);
	pthread_mutex_lock(&philo->mutex_state);
	state = philo->state;
	pthread_mutex_unlock(&philo->mutex_state);
	return (state);
}


