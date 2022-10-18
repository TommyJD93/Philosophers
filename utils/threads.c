/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:56:39 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/18 18:48:56 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	// pthread_mutex_lock(&philo->data->write);
	// printf("[%llu] hello from supervisor: %d\n", get_time() - philo->data->start_time, philo->id);
	// pthread_mutex_unlock(&philo->data->write);
	while (!philo->data->dead)
	{
		pthread_mutex_lock(&philo->data->dead_m);
		if (!philo->eating && get_time() > philo->data->death_time)
		{
			message(DIED, philo);
		}
		pthread_mutex_unlock(&philo->data->dead_m);
		ft_usleep(5);
	}


	return ((void *)0);
}

void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	// pthread_mutex_lock(&philo->data->write);
	// printf("[%llu] hello from philo: %d\n", get_time() - philo->data->start_time, philo->id);
	// pthread_mutex_unlock(&philo->data->write);
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))
		return ((void *)1);
	while (!philo->data->dead)
	{
		// eat(philo);
		message(THINKING, philo);
	}
	if (pthread_join(philo->t1, NULL))
		return ((void *)1);
	return ((void *)0);
}

int	thread_init(t_data *data)
{
	int	i;

	i = -1;
	data->start_time = get_time();
	while (++i < data->philo_num)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
			return (error(TH_ERR));
		ft_usleep(1);
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error(JOIN_ERR));
		i++;
	}
	return (0);
}