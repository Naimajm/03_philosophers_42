/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_mutex_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:04:23 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/18 18:14:37 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool 	is_program_active(t_data *data);

void	increment_num_meals_eaten(t_philo * philo);
int		get_num_meals(t_philo * philo);
void	set_last_meal(t_philo *philo);

void	change_philo_state(t_philo *philo, t_state new_state);
t_state	get_philo_state(t_philo *philo);

///// DATA -------------------------------------------

bool is_program_active(t_data *data)
{
	bool keep_itering;

	pthread_mutex_lock(&data->mutex->program_active);
	keep_itering = data->keep_iter;
	pthread_mutex_unlock(&data->mutex->program_active);
	return (keep_itering);
}


// PHILO ---------------------------------------------
// ---------------------------------------------

void	increment_num_meals_eaten(t_philo * philo)
{
	if (!philo)
		return ;
	pthread_mutex_lock(&philo->mutex_num_meals);
	philo->num_meals++;
	pthread_mutex_unlock(&philo->mutex_num_meals);
}

int		get_num_meals(t_philo * philo)
{
	int num_meals;

	if (!philo)
		return (NULL);
	pthread_mutex_lock(&philo->mutex_num_meals);
	num_meals = philo->num_meals;
	pthread_mutex_unlock(&philo->mutex_num_meals);
	return (num_meals);
}

void	set_last_meal(t_philo *philo)
{
	if (!philo)
		return ;
	pthread_mutex_lock(&philo->mutex_last_eat_time);
	philo->last_eat_time = get_current_time();
	pthread_mutex_unlock(&philo->mutex_last_eat_time);
}

// STATE ------------------------------------

void	change_philo_state(t_philo *philo, t_state new_state)
{
	if (!philo)
		return ;
	pthread_mutex_lock(&philo->mutex_state);
	if (philo->state != DEAD)
		philo->state = new_state;
	pthread_mutex_lock(&philo->mutex_state);
}

t_state	get_philo_state(t_philo *philo)
{	
	t_state state;

	if (!philo)
		return (NULL);
	pthread_mutex_lock(&philo->mutex_state);
	state = philo->state;
	pthread_mutex_unlock(&philo->mutex_state);
	return (state);
}

