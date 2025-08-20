/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:50:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 21:14:53 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <pthread.h>
# include <stdbool.h>

typedef enum s_state
{
	INITIAL		= 0,
	EATING		= 1,
	SLEEPING	= 2,
	THINKING	= 3,
	DEAD		= 4
}	t_state;

typedef struct s_philo
{
	int				id;
	int				num_meals;
	pthread_mutex_t	mutex_num_meals;

	long			last_meal_time;
	pthread_mutex_t	mutex_last_meal_time;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;	

	t_state			state;
	pthread_mutex_t	mutex_state;

	struct s_data	*data;
}	t_philo;

typedef struct s_mutex
{
	pthread_mutex_t	program_active;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_log;
}	t_mutex;

typedef struct s_data
{
	int			num_philos;
	int			die_time;
	int			eat_time;
	int			sleep_time;
	int			num_meals;

	long		start_time;
	bool		program_active;

	t_mutex		*mutex;
	t_philo		*philos;

	pthread_t	monitor_death;
	pthread_t	monitor_meals;
	pthread_t	*philo_threads;
}	t_data;

#endif