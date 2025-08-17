/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:58:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 18:14:07 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	get_current_time(void)
{
	struct timeval	timestamp;
	long			current_time;

	if (gettimeofday(&timestamp, NULL))
		return (FAILURE);
	// Milisegundos desde Unix epoch
	// AGREGAR L para evitar overflow en enteros grandes
	current_time = timestamp.tv_sec * 1000L + (timestamp.tv_usec / 1000L);
	return (current_time);
}