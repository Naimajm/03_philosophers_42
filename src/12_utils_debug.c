/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   12_utils_debug.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 11:21:34 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 01:13:34 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void 		print_philos_array(t_data *data);
static void print_forks_assignment(t_data *data, int index_philo);
static int 	get_fork_index(t_data *data, pthread_mutex_t *fork);
static char *get_state_string(t_state state);
void		print_arguments(char **argv);
void		print_strings_array(char **array);


void print_data(t_data *data)
{
	if (!data)
		return ;
	
	printf("┌────────────┐\n");
	printf("| DATA       |\n");		
	printf("└────────────┘\n");     
	
	printf("\t num_philos \t-> %i\n", data->num_philos);    
	printf("\t die_time \t-> %i\n", data->die_time);  
	printf("\t eat_time \t-> %i\n", data->eat_time);   
	printf("\t sleep_time \t-> %i\n", data->sleep_time);  
	printf("\t num_meals \t-> %i\n", data->num_meals);  
	printf("\t num_full \t-> %i\n", data->num_full_philos);  

	printf("\t start_time \t-> %li\n", data->start_time);  
	printf("\t keep_iter \t-> %d\n", data->program_active);  
	printf("\t ───────── \n\n");

	//print_philos_array(data);		
}

void print_philos_array(t_data *data)
{
	t_philo *philo;
	int index;

	if (!data || !data->philos)
    {
        printf("└──> philos array -> [ (null) ]\n");
        return ;
    } 
	index = 0;	
	while (index < data->num_philos)
	{
		philo = (t_philo *) &data->philos[index];
		printf("\t └───┐\n");
		printf("\t ┌────────────┐\n");
		printf("\t | philo [%i]  |\n", philo->id);		
		printf("\t └────────────┘\n");

		printf("\t\t num_meals \t\t-> %i\n", philo->num_meals);
		printf("\t\t last_eat_time \t\t-> %li\n", philo->last_meal_time);        
		printf("\t\t state \t\t\t-> %s\n", get_state_string(philo->state));   

		// MOSTRAR FORKS
		print_forks_assignment(data, index);
		//printf("\t ─────────────────── \n");
		index++;		
	}
	printf("\n");
}

static void print_forks_assignment(t_data *data, int index_philo)
{
    if (!data || !data->philos)
		return ;

	printf("\t\t └───┐\n");
	printf("\t\t ┌─────────────────┐\n");
	printf("\t\t | Forks philo [%i] |\n", data->philos[index_philo].id);
	printf("\t\t └─────────────────┘\n");

	printf("\t\t\t fork_left \t-> [%i]\n", get_fork_index(data, data->philos[index_philo].fork_left));  
	printf("\t\t\t fork_right \t-> [%i]\n", get_fork_index(data, data->philos[index_philo].fork_right));     
    printf("\n");
}

static int get_fork_index(t_data *data, pthread_mutex_t *fork)
{
    int index;
	int id_fork;

	index = 0;
	id_fork = index + 1;
    while (index < data->num_philos)
    {
        if (&data->mutex->forks[index] == fork)
            return (id_fork);
		id_fork++;
        index++;
    }
    return (-1);  // No encontrado
}

static char *get_state_string(t_state state)
{
	if (state == INITIAL)
		return ("INITIAL");
	else if (state == EATING)
		return ("EATING");
	else if (state == SLEEPING)
		return ("SLEEPING");
	else if (state == THINKING)
		return ("THINKING");
	else if (state == FINISHED)
		return ("FINISHED");
	else if (state == DEAD)
		return ("DEAD");		
	else
		return ("UNKNOWN");    
}

void	print_arguments(char **argv)
{
	int	index;

	if (!argv)
    {
        printf("└──> args -> [ (null) ]\n");
        return;
    }
	index = 0;
	printf("└──> args -> [ "); 
	while (argv[index])
	{        
		printf("%s ", argv[index]); 
		if (argv[index + 1])    
			printf(",");   
		index++;
	}
	printf("]\n");
}

void	print_strings_array(char **array)
{
	int	index;

	if (!array)
		return ;
	index = 0;
	while (array[index])
	{
		printf("%s\n", array[index]);
		index++;
	}
}