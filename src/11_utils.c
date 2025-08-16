/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   11_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juagomez <juagomez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 13:30:37 by juagomez          #+#    #+#             */
/*   Updated: 2025/08/15 12:02:22 by juagomez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_atoi(const char *str)
{
	int	index;
	int	result;
	int	sign_count;
	int	sign;

	index = 0;
	result = 0;
	sign_count = 0;
	sign = 1;
	while ((str[index] == ' ' || (str[index] >= '\t' && str[index] <= '\r')))
		index++;
	while ((str[index] == '+' || str[index] == '-'))
	{
		sign_count += 1;
		if (str[index] == '-' && sign_count <= 1)
			sign = -1;
		index++;
	}
	while (str[index] >= '0' && str[index] <= '9' && sign_count <= 1)
	{
		result = (result * 10) + (str[index] - '0');
		index++;
	}
	return (sign * result);
}

void	ft_putendl_fd(char *str, int fd)
{
	int	index;

	index = 0;
	if (str)
	{
		while (str[index] != '\0')
		{
			write(fd, &str[index], 1);
			index++;
		}
		write(fd, "\n", 1);
	}
}