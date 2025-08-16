/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_mutex_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:04:23 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/15 17:16:48 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	set_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_last_eat);
	philo->last_eat = my_watch();
	pthread_mutex_unlock(&philo->mutex_last_eat);
}