/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   10_free_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 09:28:37 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/19 17:50:59 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"


void 	cleanup_all_locks(t_data *data);
void 	cleanup_philosopher_locks(t_philo *philo);

void 	cleanup_data(t_data *data);
void	free_philos(t_data *data);
void	free_mutex(t_data *data);

// limpieza GLOBAL DATA 
void cleanup_data(t_data *data)
{
	if (!data)
		return ;

	// parar programa
	//stop_program(data);

	// LIBERAR LOCKS ACTIVOS PRIMERO
    //cleanup_all_locks(data);
	
	// MUTEX
	free_mutex(data);

	// PHILOS
	free_philos(data);
	
	// THREADS
	free(data->philo_threads);
	data->philo_threads 	= NULL;
	data->monitor_death 	= 0;
	data->monitor_meals 	= 0;	

	free(data);
	printf(FREE_ALL);
}

void cleanup_all_locks(t_data *data)
{
    int i;
    
    if (!data)
        return;
    
    printf("🧹 CLEANUP: Liberando todos los locks...\n");
    
    // 1. Liberar todos los forks (pthread_mutex_unlock puede fallar silenciosamente)
    if (data->mutex && data->mutex->forks)
    {
        i = 0;
        while (i < data->num_philos)
        {
            // Intentar unlock sin verificar si está tomado
            pthread_mutex_unlock(&data->mutex->forks[i]);
            i++;
        }
    }    
    // 2. Liberar mutexes principales
    if (data->mutex)
    {
		pthread_mutex_unlock(&data->mutex->program_active);
        pthread_mutex_unlock(&data->mutex->print_log);
    }    
    // 3. Resetear estados de filósofos
    if (data->philos)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_unlock(&data->philos[i].mutex_num_meals);
            pthread_mutex_unlock(&data->philos[i].mutex_last_meal_time);
            pthread_mutex_unlock(&data->philos[i].mutex_state);
            
            // Forzar estado a DEAD
            data->philos[i].state = DEAD;
            i++;
        }        
    }    
    printf("🧹 CLEANUP: Locks liberados\n");
}

void cleanup_philosopher_locks(t_philo *philo)
{
    if (!philo)
        return;
        
    printf("🧹 CLEANUP: Liberando locks del filósofo %d\n", philo->id);
    
    // Liberar forks del filósofo específico
    if (philo->left_fork)
        pthread_mutex_unlock(philo->left_fork);
    if (philo->right_fork)
        pthread_mutex_unlock(philo->right_fork);
}



// LIMPIEZA ESTRUCTURA PHILOS
void	free_philos(t_data *data)
{
	int	index;

	if (!data)
		return ;
	index = 0;
	while (index < data->num_philos)
	{
		pthread_mutex_destroy(&data->philos[index].mutex_num_meals);
		pthread_mutex_destroy(&data->philos[index].mutex_last_meal_time);
		pthread_mutex_destroy(&data->philos[index].mutex_state);
		// liberar punteros fork_left y fork_right -> Se liberarán en free_mutex()
		// state enum no necesta liberacion
		//free(&data->philos[index].data);
		index++;
	}
	free(data->philos);
	data->philos = NULL;
}

// LIMPIEZA ESTRUCTURA MUTEX
void	free_mutex(t_data *data)
{	
	int	index;

	if (!data || !data->mutex)
		return ;
	// DESTROY MUTEX
	//pthread_mutex_destroy(&data->mutex->num_full_philos);
	pthread_mutex_destroy(&data->mutex->program_active);
	pthread_mutex_destroy(&data->mutex->print_log);
	
	// ARRAY MUTEX FORKS
	index = 0;
	while (index < data->num_philos)
	{
		pthread_mutex_destroy(&data->mutex->forks[index]);
		index++;
	}
	free(data->mutex->forks);
	data->mutex->forks = NULL;	
	
	free(data->mutex);
	data->mutex = NULL;
}



