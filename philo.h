/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:04:22 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/18 18:13:23 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

# define ALLOC_ERR_1 "ERROR WHILE ALLOCATING THREADS IDs"
# define ALLOC_ERR_3 "ERROR WHILE ALLOCATING PHILOS"
# define ALLOC_ERR_2 "ERROR WHILE ALLOCATING FORKS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"
# define ERR_IN_1 "INVALID INPUT CHARACTER"
# define TIME_ERR "UNABLE TO RETRIVE UTC"
# define ERR_IN_2 "INVALID INPUT VALUES"

struct s_data;

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		t1;
	int				id;
	int				eat_cont;
	int				status; //if != 0 philo is dead

	uint64_t		time_to_die;

	pthread_mutex_t *r_fork;
	pthread_mutex_t *l_fork;
} t_philo;

typedef struct s_data
{
	pthread_t		*tid;
	int				philo_num;
	int				meals_nb;

	t_philo			*philos;

	uint64_t		death_time;
	uint64_t		eat_time;
	uint64_t		sleep_time;
	uint64_t		start_time;

	pthread_mutex_t	*forks;
	pthread_mutex_t	write;
	pthread_mutex_t	dead;
} t_data;

//	utils
long		ft_atoi(const char *str);
int			error(char *str);
uint64_t	get_time(void);
void		ft_usleep(uint64_t time);

//	init
int			init(t_data *data, char **argv, int argc);
int			thread_init(t_data *data);
//	checker
int			input_checker(char **argv);
#endif