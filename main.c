/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:00:13 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/19 09:27:36 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error(char *str)
{
	printf("%s\n", str);
	return (1);
}

void	messages(char *str, t_philo *philo)
{
	uint64_t	time;

	pthread_mutex_lock(&philo->data->write);
	time = get_time() - philo->data->start_time;
	if (ft_strcmp(DIED, str) == 0 && !philo->data->dead)
	{
		printf("[%llu]: %d\t%s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
	if (!philo->data->dead)
		printf("[%llu]: %d\t%s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (1);
	if (input_checker(argv))
		return (1);
	if (init(&data, argv, argc))
		return (1);
	if (thread_init(&data))
		return (1);
	return (0);
}
