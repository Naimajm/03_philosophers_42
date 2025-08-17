/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:37:48 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/17 17:58:19 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

# define FAILURE			1
# define SUCCESS			0

# define ERROR_ARGS_NUMBER 			"Error\n Wrong number of arguments\n"
# define ERROR_ARGS_PHILO_NUMBER 	"Error\n Number of philosophers must be at least 2\n"
# define ERROR_ARGS_TIME_DIE 		"Error\n Time to die must be at least 60 ms\n"
# define ERROR_ARGS_TIME_EAT 		"Error\n Time to eat must be at least 60 ms\n"
# define ERROR_ARGS_TIME_SLEEP 		"Error\n Time to sleep must be at least 60 ms\n"
# define ERROR_ARGS_MEALS_NUMBER 	"Error\n Number of meals must be at least 1\n"

# define ERROR_INVALID_INPUT	"Error\n Invalid input parameter\n"
# define ERROR_MEM_ALLOC		"Error\n Memory allocation failed\n"

# define ERROR_THREADS_CREATE		"Error\n Error creating thread\n"


# define FREE_ALL				"Free\n Total cleaning... OK\n"

// STATES PHILO
# define INIT	0
# define EAT 	1
# define SLEEP 	2
# define THINK 	3
# define FINISH	4
# define DIED  	-1

//SYSTEM LOGS
# define MSG_TAKE_FORKS "has taken a fork"
# define MSG_EATING		"is eating"
# define MSG_SLEEPING 	"is sleeping"
# define MSG_THINKING 	"is thinking"
# define MSG_DIED 		"died"

# define USLEEP_TIME	100 	// Microsegundos para sleep en loops
 
# endif