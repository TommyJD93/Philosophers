/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:00:13 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/18 18:50:36 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(char *str)
{
	printf("%s\n", str);
	return (1);
}

void	message(char *str, t_philo *philo)
{
	uint64_t	time;
	pthread_mutex_lock(&philo->data->write);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->write);
		return ;
	}
	time = get_time() - philo->data->start_time;
	if (ft_strcmp(DIED, str) == 0)
		philo->data->dead = 1;
	printf("[%llu]: %d\t%s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

int	main(int argc, char **argv)
{
	t_data data;

	if (argc < 5 || argc > 6)

	if (input_checker(argv))
		return (1);
	if (init(&data, argv, argc))
		return (1);
	if (thread_init(&data))
		return (1);
	return (0);
}
