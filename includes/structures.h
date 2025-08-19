/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:50:13 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 21:22:53 by juagomez         ###   ########.fr       */
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
	DEAD 		= 4
}	t_state;

// PHILOS
typedef struct s_philo
{
	int				id;	
	int				num_meals;
	pthread_mutex_t mutex_num_meals;

	long			last_meal_time;
	pthread_mutex_t	mutex_last_meal_time;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;	

	t_state			state;
	pthread_mutex_t	mutex_state;
	
	struct s_data	*data; 				// conexion nivel superior a data struct
}	t_philo;

typedef struct s_mutex
{	
	pthread_mutex_t	program_active;		
	pthread_mutex_t	*forks;	
	pthread_mutex_t	print_log;			// ACTIVAR ORDEN IMPRESION	
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

	// VARIABLES DE ESTADO
	long		start_time;			// TIEMPO INICIO PROCESO
	bool		program_active; 	// flag estado global programa -> activo
	
	t_mutex		*mutex;				// MUTEX	
	t_philo		*philos;			// CONEXION ARRAY PHILOS

	// THREADS
	pthread_t	monitor_death;		// Thread monitoreo muerte
	pthread_t	monitor_meals;		// Thread monitoreo cuando todos comieron suficiente 
	pthread_t	*philo_threads;	
}	t_data;

# endif

