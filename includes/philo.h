/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:07:34 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/15 17:41:31 by juagomez         ###   ########.fr       */
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
t_data	*initialize_data(int argc, char **argv);
void	initialize_philos(t_data *data);
void	initialize_mutex(t_data *data);

// 00_main.c 
 
// 04_mutex_functions.c 
void	set_last_meal(t_philo *philo);

// 08_time.c
int		my_watch(void);

// 11_utils.c
int		ft_atoi(const char *str);
void	ft_putendl_fd(char *str, int fd);

// 12_utils_debug.c
void 	print_data(t_data *data);
void 	print_philos_array(t_data *data);
void	print_arguments(char **argv);
void	print_strings_array(char **array);

# endif