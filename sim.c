/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:27:29 by mickael           #+#    #+#             */
/*   Updated: 2026/06/02 12:13:55 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_init(t_rules *r, int ac, char **av)
{
	int		i;

	i = 0;
	if (parsing(r, ac, av))
		return (1);
	if (rules_init(r))
		return (1);
	if (coder_init(r))
		return (1);
	r->dongles = malloc(sizeof(t_dongle) * r->nb_coders);
	if (!r->dongles)
		return (1);
	while (i < r->nb_coders)
	{
		if (dongle_init(&r->dongles[i], r) != 0)
		{
			while (--i >= 0)
				dongle_destroy(&r->dongles[i]);
			free(r->dongles);
			free(r->coders);
			return (1);
		}
		i++;
	}
	return (0);
}

int	sim_run_create(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->nb_coders)
	{
		if (pthread_create(&r->coders[i].coder_thread, NULL,
				processing, &r->coders[i]) != 0)
		{
			printf("Error: failed to create coder thread");
			return (1);
		}
		i++;
	}
	if (pthread_create(&r->monitor_thread, NULL, monitor, r) != 0)
	{
		printf("Error: failed to create monitor thread");
		pthread_mutex_lock(&r->stop_mutex);
		r->stopped = 1;
		pthread_mutex_unlock(&r->stop_mutex);
		return (1);
	}
	return (0);
}

void	sim_run_join(t_rules *r)
{
	int	i;

	i = 0;
	while (i < r->nb_coders)
	{
		pthread_join(r->coders[i].coder_thread, NULL);
		i++;
	}
	pthread_mutex_lock(&r->stop_mutex);
	r->stopped = 1;
	pthread_mutex_unlock(&r->stop_mutex);
	pthread_join(r->monitor_thread, NULL);
}

void	sim_cleaning(t_rules *r)
{
	int		i;

	i = 0;
	while (i < r->nb_coders)
	{
		dongle_destroy(&r->dongles[i]);
		i++;
	}
	free(r->dongles);
	free(r->coders);
	pthread_mutex_destroy(&r->stop_mutex);
	pthread_mutex_destroy(&r->log_mutex);
}
