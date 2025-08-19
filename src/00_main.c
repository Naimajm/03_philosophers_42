/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 10:07:17 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 22:02:10 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	validate_limits(int argc, char **argv);
int	validate_syntax(int argc, char **argv);
int	validate_numeric_format(int argc, char **argv);

// MAIN -----------------------------------------------------------------------

int	main(int argc, char **argv)
{
	t_data	*data;
	if (validate_syntax(argc, argv))		// CHECK ARGS
		return (FAILURE);
	if (validate_limits(argc, argv))
		return (FAILURE);
		
	data = initialize_data(argc, argv);		// INICIALIZACION DATA (MUTEX + FORKS)	
	//print_data(data);						// DEBUG
	
	initialize_philos(data);				// PHILOS	
	assign_forks_to_philos(data);	
	//print_philos_array(data);				// DEBUG
	
	initialize_threads(data);				// INICIALIZACION THREADS	
	wait_for_threads(data);					// ESPERA FINALIZACION HILOS
	//print_philos_array(data);				// DEBUG	

	cleanup_data(data);						// LIBERACION	
	return (SUCCESS);
}

int validate_syntax(int argc, char **argv)
{
	if (!argc || !argv)
		return (FAILURE);
	if (argc < 5 || argc > 6)				// check num argumentos
	{
		ft_putendl_fd(ERROR_ARGS_NUMBER, STDERR_FILENO);
		return (FAILURE);
	}
	else if (validate_numeric_format(argc, argv))	// check char tipo numero
	{
		ft_putendl_fd(ERROR_ARGS_TYPE, STDERR_FILENO);
		return (FAILURE);
	}
	return (SUCCESS);
}

int validate_numeric_format(int argc, char **argv)
{
	int		index_arg;
	int 	index_char;
	char	*argument;

	if (!argc || !argv)
		return (FAILURE);
	index_arg = 1;
	while (index_arg < argc)
	{
		index_char = 0;
		argument = argv[index_arg];
		while (argument[index_char])
		{
			// verificacion de char 'numero'
			if (argument[index_char] < '0' || argument[index_char] > '9')				
				return (FAILURE);
			index_char++;
		}
		index_arg++;
	}
	return (SUCCESS);
}

int	validate_limits(int argc, char **argv)
{	
	if (!argc || !argv)
		return (FAILURE);
	if  (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > MAX_PHILOS)	// check num max philos
	{
		ft_putendl_fd(LIMIT_ARGS_PHILO_NUMBER, STDERR_FILENO);
		return (FAILURE);
	}	
	if  (ft_atoi(argv[2]) < MIN_TIME || 
			ft_atoi(argv[3]) < MIN_TIME ||
			ft_atoi(argv[4]) < MIN_TIME)	// check TIEMPO MINIMO
	{
		ft_putendl_fd(LIMIT_ARGS_TIME, STDERR_FILENO);
		return (FAILURE);
	}
	if  (argc == 6 && ft_atoi(argv[5]) < 1)				// check num minimo comidas
	{
		ft_putendl_fd(LIMIT_ARGS_MEALS_NUMBER, STDERR_FILENO);
		return (FAILURE);
	}
	return (SUCCESS);
}













