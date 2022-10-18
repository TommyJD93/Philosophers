/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tterribi <tterribi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 17:00:13 by tterribi          #+#    #+#             */
/*   Updated: 2022/10/18 18:08:53 by tterribi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
