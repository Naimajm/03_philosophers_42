/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:03:02 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/18 18:09:15 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool has_eaten_enough(t_philo *philo);
bool is_alive(t_philo *philo);

bool has_eaten_enough(t_philo *philo)
{
	bool	has_eaten;

	has_eaten = false;
	if (philo->data->num_meals == -1)
		has_eaten = false;
	else if (get_num_meals(philo) >= philo->data->num_meals)
		has_eaten = true;
	return (has_eaten);	
}

bool is_alive(t_philo *philo)
{
	if (get_philo_state(philo) != DEAD)
		return (true);
    return (false);
}