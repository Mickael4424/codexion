/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 12:59:58 by mbouyer           #+#    #+#             */
/*   Updated: 2026/06/02 11:47:40 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_stopped(t_rules *r)
{
	int	stopped;

	pthread_mutex_lock(&r->stop_mutex);
	stopped = r->stopped;
	pthread_mutex_unlock(&r->stop_mutex);
	if (stopped)
		return (1);
	else
		return (0);
}

static int	check_deadline(long now, t_rules *r, int i)
{
	long	deadline;

	pthread_mutex_lock(&r->stop_mutex);
	deadline = r->coders[i].deadline;
	pthread_mutex_unlock(&r->stop_mutex);
	if (now >= deadline)
	{
		log_burnout(&r->coders[i], LOG_BURNOUT);
		return (1);
	}
	return (0);
}

static int	check_coders(t_rules *r, long now)
{
	int	i;
	int	skip;

	i = -1;
	while (++i < r->nb_coders)
	{
		pthread_mutex_lock(&r->stop_mutex);
		skip = r->coders[i].burned_out;
		pthread_mutex_unlock(&r->stop_mutex);
		if (!skip && check_deadline(now, r, i) == 1)
			return (1);
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_rules	*r;

	r = (t_rules *)arg;
	while (!check_stopped(r))
	{
		if (check_coders(r, get_time_ms()))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
