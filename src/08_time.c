/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   08_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 16:58:50 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/15 17:02:44 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	my_watch(void)
{
	struct timeval	ms;

	if (gettimeofday(&ms, NULL))
		return (SUCCESS);
	return (ms.tv_sec * (int) 1000 + (ms.tv_usec / 1000));
}