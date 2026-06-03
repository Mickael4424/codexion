/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:37:16 by mickael           #+#    #+#             */
/*   Updated: 2026/06/02 15:02:40 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_init(t_rules *r)
{
	int	i;

	r->coders = malloc(sizeof(t_coder) * r->nb_coders);
	if (!r->coders)
	{
		printf("Error: malloc coders");
		return (1);
	}
	i = 0;
	while (i < r->nb_coders)
	{
		r->coders[i].id = i + 1;
		r->coders[i].left_dongle = i;
		r->coders[i].right_dongle = (i + 1) % r->nb_coders;
		r->coders[i].last_compiled = 0;
		r->coders[i].nb_compiles = 0;
		r->coders[i].rules = r;
		r->coders[i].burned_out = 0;
		r->coders[i].deadline = get_time_ms() + r->time_to_burnout;
		i++;
	}
	return (0);
}

static int	parsing_args_1(t_rules *r, int ac, char **av)
{
	if (ac != 9)
		return (1);
	if (ft_atoi(av[1]) < 1 || is_valid_arg(av[1]))
		return (1);
	r->nb_coders = atoi(av[1]);
	if (ft_atoi(av[2]) < 0 || is_valid_arg(av[2]))
		return (1);
	r->time_to_burnout = ft_atoi(av[2]);
	if (ft_atoi(av[3]) < 0 || is_valid_arg(av[3]))
		return (1);
	r->time_to_compile = ft_atoi(av[3]);
	if (ft_atoi(av[4]) < 0 || is_valid_arg(av[4]))
		return (1);
	r->time_to_debug = ft_atoi(av[4]);
	if (ft_atoi(av[5]) < 0 || is_valid_arg(av[5]))
		return (1);
	r->time_to_refactor = ft_atoi(av[5]);
	if (ft_atoi(av[6]) < 0 || is_valid_arg(av[6]))
		return (1);
	return (0);
}

static int	parsing_args_2(t_rules *r, char **av)
{
	r->nb_compiles_required = ft_atoi(av[6]);
	if (ft_atoi(av[7]) < 0 || is_valid_arg(av[7]))
		return (1);
	r->dongle_cooldown = ft_atoi(av[7]);
	if (strcmp(av[8], "fifo") != 0 && (strcmp(av[8], "edf") != 0))
		return (1);
	r->scheduler = av[8];
	r->stopped = 0;
	return (0);
}

int	parsing(t_rules *r, int ac, char **av)
{
	if (parsing_args_1(r, ac, av) || parsing_args_2(r, av))
	{
		printf("Error: invalid arguments. \n");
		printf("Usage: ");
		printf("./codexion nb_of_coders t_to_burnout t_to_compile ");
		printf("t_to_debug t_to_refactor nb_of_compiles_required ");
		printf("dongle_cooldown scheduler\n");
		return (1);
	}
	if (ft_atoi(av[1]) == 0)
		return (1);
	return (0);
}

int	rules_init(t_rules *r)
{
	r->time_start = get_time_ms();
	if (pthread_mutex_init(&r->log_mutex, NULL) != 0)
	{
		return (1);
	}
	if (pthread_mutex_init(&r->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&r->log_mutex);
		return (1);
	}
	return (0);
}
