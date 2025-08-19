/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:07:34 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 02:01:50 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// OWN LIBRARIES
# include "macros.h"		// MACROS
# include "structures.h" 	// STRUCTS

// EXTERNAL LIBRARIES
# include <pthread.h>	// FUNCIONES HILOS
# include <stdio.h>  	// printf
# include <stdbool.h>	// bool
# include <unistd.h>	// STDERR_FILENO
# include <stdlib.h> 	// malloc
# include <sys/time.h>	// time utils -> gettimeofday

// FUNCTIONS ------------------------------------------

// 00_main.c 

//  01_init.c 
t_data	*initialize_data(int argc, char **argv);
void	initialize_philos(t_data *data);
int 	assign_forks_to_philos(t_data *data);
void	initialize_mutex(t_data *data);

// 02_threads.c
int		initialize_threads(t_data *data);
void 	*daily_routine(void * args);
void	*monitor_death(void *data);
void	*monitor_meals(void *data);
int 	wait_for_threads(t_data *data);

// 03_routine.c
int		thinking(t_philo *philo);
int		sleeping(t_philo *philo);
int		eating(t_philo *philo);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

// 03.1_monitor.c
//void	program_is_over(t_data *data);

// 04_core.c
bool 	has_eaten_enough(t_philo *philo);
bool 	is_alive(t_philo *philo);
 
// 05_mutex_functions.c
bool 	is_program_active(t_data *data);
void 	stop_program(t_data *data);

void	increment_num_meals(t_philo * philo);
int		get_num_meals(t_philo * philo);

void	set_last_meal_time(t_philo *philo);
long	get_last_meal_time(t_philo *philo);

void	change_philo_state(t_philo *philo, t_state new_state);
t_state	get_philo_state(t_philo *philo);

// 08_time.c
void	set_delay_time(long pause_time);
long	get_current_time(void);

// 09_logs.c
void 	printing_logs(t_data *data, int id_philo, char *message);

// 10_free_manager.c
void 	cleanup_data(t_data *data);
void	free_mutex(t_data *data);

// 11_utils.c
int		ft_atoi(const char *str);
void	ft_putendl_fd(char *str, int fd);

// 12_utils_debug.c
void 	print_data(t_data *data);
void 	print_philos_array(t_data *data);
void	print_arguments(char **argv);
void	print_strings_array(char **array);

# endif