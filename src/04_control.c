/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_control.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:04:23 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 18:13:02 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	printing_logs(t_data *data, int id_philo, char *message);
bool	is_program_active(t_data *data);
void	stop_program(t_data *data);
void	set_delay_time(long pause_time);
long	get_current_time(void);

void	printing_logs(t_data *data, int id_philo, char *message)
{
	long	relative_time;

	relative_time = get_current_time() - data->start_time;
	pthread_mutex_lock(&data->mutex->print_log);
	if (is_program_active(data))
		printf("%li %i %s\n", relative_time, id_philo, message);
	pthread_mutex_unlock(&data->mutex->print_log);
}

bool	is_program_active(t_data *data)
{
	bool	active;

	pthread_mutex_lock(&data->mutex->program_active);
	active = data->program_active;
	pthread_mutex_unlock(&data->mutex->program_active);
	return (active);
}

void	stop_program(t_data *data)
{
	if (!data || !data->mutex)
		return ;
	pthread_mutex_lock(&data->mutex->program_active);
	data->program_active = false;
	pthread_mutex_unlock(&data->mutex->program_active);
	usleep(USLEEP_MONITOR_TIME);
}

void	set_delay_time(long pause_time)
{
	long	initial_time;

	initial_time = get_current_time();
	while ((get_current_time() - initial_time) < pause_time)
		usleep(USLEEP_LOOPS_TIME);
}

long	get_current_time(void)
{
	struct timeval	timestamp;
	long			current_time;

	if (gettimeofday(&timestamp, NULL))
		return (FAILURE);
	current_time = timestamp.tv_sec * 1000 + (timestamp.tv_usec / 1000);
	return (current_time);
}
