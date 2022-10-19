/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 18:35:49 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/19 09:26:37 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	messages(TAKE_FORKS, philo);
	pthread_mutex_lock(philo->r_fork);
	messages(TAKE_FORKS, philo);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	messages(SLEEPING, philo);
	ft_usleep(philo->data->sleep_time);
}

void	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	messages(EATING, philo);
	philo->eat_cont++;
	ft_usleep(philo->data->eat_time);
	messages("suca", philo);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
}
