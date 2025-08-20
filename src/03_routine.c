/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 22:07:31 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 18:13:31 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int		thinking(t_philo *philo);
int		sleeping(t_philo *philo);
int		eating(t_philo *philo);
int		take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

int	thinking(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);
	change_philo_state(philo, THINKING);
	printing_logs(philo->data, philo->id, MSG_THINKING);
	usleep(100);
	//set_delay_time(100);
	return (SUCCESS);
}

int	sleeping(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);
	change_philo_state(philo, SLEEPING);
	printing_logs(philo->data, philo->id, MSG_SLEEPING);
	set_delay_time(philo->data->sleep_time);
	return (SUCCESS);
}

int	eating(t_philo *philo)
{
	if (!philo || !is_alive(philo))
		return (FAILURE);
	take_forks(philo);
	change_philo_state(philo, EATING);
	printing_logs(philo->data, philo->id, MSG_EATING);
	set_delay_time(philo->data->eat_time);
	set_last_meal_time(philo);
	increment_num_meals(philo);
	drop_forks(philo);
	return (SUCCESS);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (!philo || !is_alive(philo))
		return (FAILURE);
	if (philo->left_fork < philo->right_fork)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
	pthread_mutex_lock(second_fork);
	printing_logs(philo->data, philo->id, MSG_TAKE_FORKS);
	return (SUCCESS);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
