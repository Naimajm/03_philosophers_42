/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_philo_meal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:39:33 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 23:52:46 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool 	has_eaten_enough(t_philo *philo);
void	increment_num_meals(t_philo * philo);
int		get_num_meals(t_philo * philo);
void	set_last_meal_time(t_philo *philo);
long	get_last_meal_time(t_philo *philo);

bool has_eaten_enough(t_philo *philo)
{
	bool	has_eaten;

	has_eaten = false;
	if (philo->data->num_meals <= 0)
		has_eaten = false;
	else if (get_num_meals(philo) >= philo->data->num_meals)
		has_eaten = true;
	return (has_eaten);
}

void	increment_num_meals(t_philo * philo)
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
		return (FAILURE);
	pthread_mutex_lock(&philo->mutex_num_meals);
	num_meals = philo->num_meals;
	pthread_mutex_unlock(&philo->mutex_num_meals);
	return (num_meals);
}

void	set_last_meal_time(t_philo *philo)
{
	if (!philo)
		return ;
	pthread_mutex_lock(&philo->mutex_last_meal_time);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->mutex_last_meal_time);
}

long	get_last_meal_time(t_philo *philo)
{
	long	last_meal_time;
	
	if (!philo)
		return (0);
	pthread_mutex_lock(&philo->mutex_last_meal_time);
	last_meal_time = philo->last_meal_time;
	pthread_mutex_unlock(&philo->mutex_last_meal_time);
	return (last_meal_time);
}


