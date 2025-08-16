/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:50:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/15 17:24:45 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include <pthread.h>	// FUNCIONES HILOS
# include <stdbool.h>	// bool

// ESTADOS PHILOS
typedef enum s_state
{
	INITIAL		= 0,
	EATING		= 1,
	SLEEPING 	= 2,
	THINKING  	= 3,
	FINISHED	= 4,
	DEAD 		= 5
}	t_state;

// PHILOS
typedef struct s_philo
{
	int				id;
	
	int				num_meals;
	pthread_mutex_t mutex_num_meals;

	int				last_eat;
	pthread_mutex_t	mutex_last_eat;

	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;	

	t_state	state;
	pthread_mutex_t	mutex_state;
	// conexion a data struct s_data (t_data)	*data;

}	t_philo;

typedef struct s_mutex
{
	pthread_mutex_t	die_time;			// se van a modficicar ??
	pthread_mutex_t	eat_time;			// se van a modficicar ??
	pthread_mutex_t	sleep_time;		// se van a modficicar ??

	pthread_mutex_t	start_time;		
	pthread_mutex_t	keep_iter;

	pthread_mutex_t	*forks;	
	//pthread_mutex_t	mutex_num_philos;	// ??
	//pthread_mutex_t	mutex_print;		// ??
}	t_mutex;


// GENERAL DATA
typedef struct s_data
{
	// CONFIGURACION INICIAL SISTEMA -> ARGS
	int			num_philos;	
	int			die_time;	
	int			eat_time;	
	int			sleep_time;	
	int			num_meals;
	int			num_full;

	// PROCCESS -----------
	int			start_time;				// TIEMPO INICIO PROCESO
	bool		keep_iter;
 
	// MUTEX
	t_mutex		*mutex;
	//pthread_mutex_t	mutex_die_time;
	//pthread_mutex_t	mutex_eat_time;
	//pthread_mutex_t	mutex_sleep_time;	
	//pthread_mutex_t	mutex_num_philos;	// ??
	//pthread_mutex_t	mutex_start_time;
	//pthread_mutex_t	mutex_keep_iter;		
	//pthread_mutex_t	*mutex_forks;	
	//pthread_mutex_t	mutex_print;		// ??

	// CONEXION ARRAY PHILOS
	t_philo		*philos;

	// THREADS
	pthread_t	monit_all_alive;
	pthread_t	monit_all_full;
	pthread_t	*philo_threads;	
	
}	t_data;

# endif

