/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:12:13 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/18 17:55:56 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->tid)
		return (error(ALLOC_ERR_1));
	data->forks = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->tid)
		return (error(ALLOC_ERR_2));
	data->philos = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->tid)
		return (error(ALLOC_ERR_3));
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 0;
	data->philos[0].l_fork = &data->forks[0];
	data->philos[0].r_fork = &data->forks[data->philo_num - 1];
	i = 1;
	while (i < data->philo_num)
	{
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);

	return (0);
}

void	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{

		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].time_to_die = data->death_time;
		data->philos[i].eat_cont = 0;
		data->philos[i].status = 0;
		i++;
	}
}

int init_data(t_data *data, char **argv, int argc)
{
	data->philo_num = (int) ft_atoi(argv[1]);
	data->death_time = (uint64_t) ft_atoi(argv[2]);
	data->eat_time = (uint64_t) ft_atoi(argv[3]);
	data->sleep_time = (uint64_t) ft_atoi(argv[4]);
	if (argc == 6)
		data->meals_nb = (int) ft_atoi(argv[5]);
	else
		data->meals_nb = -1;
	if (data->philo_num < 0 || data->philo_num > 200 || data->death_time < 0
		|| data->eat_time < 0 || data->sleep_time < 0)
		return (error(ERR_IN_2));
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->dead, NULL);
	return (0);
}

int init(t_data *data, char **argv, int argc)
{
	if (init_data(data, argv, argc))
		return (1);
	if (alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_philos(data);
	//----------------INPUT PRINT----------------
    int i;
    printf("----------------INPUT PRINT----------------\n");
    printf("num_philos: %d\n", data->philo_num);
    printf("times_must_eat: %d\n", data->meals_nb);
    printf("time_to_die: %llu\n", data->death_time);
    printf("time_to_eat: %llu\n", data->eat_time);
    printf("time_to_sleep: %llu\n", data->sleep_time);
    i = 0;
    printf("philos:\n");
    while (i < data->philo_num)
    {
        printf("id: %d\n", data->philos[i].id);
        printf("data_pointer: %p\n", data->philos[i].data);
        printf("times_must_eat: %d\n", data->philos[i].eat_cont);
        printf("time_to_die: %llu\n", data->philos[i].time_to_die);
        // printf("time_to_eat: %llu\n", data->philos[i].time_to_eat);
        // printf("time_to_sleep: %llu\n", data->philos[i].time_to_sleep);
        i++;
    }
    char c;
    printf("press ENTER to continue:");
    scanf("%c", &c);

	return (0);
}
