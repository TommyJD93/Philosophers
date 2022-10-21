/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:56:39 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/20 13:32:50 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/philo.h"

void	*monitor(void *data_pointer)
{
	t_data	*data;
	int		i;

	data = (t_data *) data_pointer;
	while (data->dead == 0)
	{
		i = 0;
		pthread_mutex_lock(&data->lock);
		while (i < data->philo_num)
		{
			pthread_mutex_lock(&data->philos[i].lock);
			if (data->philos[i].eat_cont == data->meals_nb)
				data->finished++;
			pthread_mutex_unlock(&data->philos[i].lock);
			i++;
		}
		pthread_mutex_unlock(&data->lock);
	}
	return ((void *)0);
}

void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->time_to_die && philo->eating == 0)
			messages(DIED, philo);
		if (philo->data->finished == philo->data->philo_num)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	philo->time_to_die = philo->data->death_time + get_time();
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eat(philo);
		messages(THINKING, philo);
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
	if (data->meals_nb > 0)
		if (pthread_create(&data->tid[i], NULL, &monitor, &data))
			return(error(TH_ERR, data));
	while (++i < data->philo_num)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))
			return (error(TH_ERR, data));
		ft_usleep(1);
	}
	i = -1;
	while (++i < data->philo_num)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error(JOIN_ERR, data));
		// i++;
	}
	return (0);
}