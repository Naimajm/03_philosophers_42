/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:07:34 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/20 21:16:35 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "macros.h"
# include "structures.h"

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

// 00_main.c
int		main(int argc, char **argv);

//  01_init.c
t_data	*initialize_data(int argc, char **argv);
void	initialize_philos(t_data *data);
int		assign_forks_to_philos(t_data *data);

// 02_threads.c
int		initialize_threads(t_data *data);
int		wait_for_threads(t_data *data);

// 03_routine.c
int		thinking(t_philo *philo);
int		sleeping(t_philo *philo);
int		eating(t_philo *philo);
int		take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

// 04_control.c
void	printing_logs(t_data *data, int id_philo, char *message);
bool	is_program_active(t_data *data);
void	stop_program(t_data *data);
void	set_delay_time(long pause_time);
long	get_current_time(void);

// 05_philo_state.c
bool	is_alive(t_philo *philo);
void	change_philo_state(t_philo *philo, t_state new_state);
t_state	get_philo_state(t_philo *philo);

// 06_philo_meal.c
bool	has_eaten_enough(t_philo *philo);
void	increment_num_meals(t_philo *philo);
int		get_num_meals(t_philo *philo);
void	set_last_meal_time(t_philo *philo);
long	get_last_meal_time(t_philo *philo);

// 07_free_manager.c
void	cleanup_data(t_data *data);

// 08_utils_basic.c
int		ft_atoi(const char *str);
void	ft_putendl_fd(char *str, int fd);

// 09_utils_debug.c
void	print_data(t_data *data);
void	print_philos_array(t_data *data);

#endif