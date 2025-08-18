/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   09_logs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 11:39:33 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/18 17:36:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// FORMATO LOGS -> (now) ->timestamp_in_ms (id_philo)-> X (msg) -> "has taken a fork"

void printing_logs(t_data *data, int id_philo, char *message)
{
	long	relative_time;

	relative_time = get_current_time() - data->start_time;	// tiempo actual relativo a inicio
	pthread_mutex_lock(&data->mutex->print_log);
	if (is_program_active(data))
		printf("%li %i %s\n", relative_time, id_philo, message);
	pthread_mutex_unlock(&data->mutex->print_log);
}