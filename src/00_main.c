/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:07:17 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/16 21:39:55 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// MAIN -----------------------------------------------------------------------

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;

	t_data	*data;
	//print_arguments(argv);

	// CHECK ARGS

	// INICIALIZACION ESTRUCTURAS
	data = initialize_data(argc, argv);		// DATA (MUTEX + FORKS)	
	//print_data(data);						// DEBUG
	
	initialize_philos(data);				// PHILOS	
	assign_forks_to_philos(data);	
	//print_philos_array(data);				// DEBUG

	// INICIALIZACION HILOS
	initialize_threads(data);				// THREADS

	// RUTINA PHILO
	//execute_routine(data);

	// ESPERA FINALIZACION HILOS
	wait_for_threads(data);	
	//print_philos_array(data);				// DEBUG

	// LIBERACION
	cleanup_data(data);
	return (SUCCESS);
}













