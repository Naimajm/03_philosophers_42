/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macros.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:37:48 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 21:05:48 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROS_H
# define MACROS_H

# define FAILURE			1
# define SUCCESS			0

# define ERROR_ARGS_NUMBER 			"Error: Wrong number of arguments\n"
# define ERROR_ARGS_TYPE			"Error: Non-numeric argument or negative.\n"

# define LIMIT_ARGS_PHILO_NUMBER 	"Limit: Outside limits philos\n"
# define LIMIT_ARGS_TIME			"Limit: Time must be at least 30 ms\n"
# define LIMIT_ARGS_MEALS_NUMBER 	"Limit: Number of meals at least 1\n"

# define MAX_PHILOS					500
# define MIN_TIME					30

# define ERROR_INVALID_INPUT		"Error: Invalid input parameter\n"
# define ERROR_MEM_ALLOC			"Error: Memory allocation failed\n"
# define ERROR_THREAD_CREATE		"Error: Error creating thread\n"

# define FREE_ALL					"Free: Total cleaning... OK\n"

//SYSTEM MESSAGES  LOGS
# define MSG_TAKE_FORKS "has taken a fork"
# define MSG_EATING		"is eating"
# define MSG_SLEEPING 	"is sleeping"
# define MSG_THINKING 	"is thinking"
# define MSG_DIED 		"died"

# define USLEEP_LOOPS_TIME		500
# define USLEEP_MONITOR_TIME	1000
# endif
