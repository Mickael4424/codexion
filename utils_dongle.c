/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dongle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 09:09:26 by mbouyer           #+#    #+#             */
/*   Updated: 2026/06/03 08:38:30 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_init(t_dongle *d, t_rules *r)
{
	d->rules = r;
	d->available = 1;
	d->available_at = 0;
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (1);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (1);
	}
	if (dq_init(&d->waiters, r->nb_coders + 1) != 0)
	{
		pthread_cond_destroy(&d->cond);
		pthread_mutex_destroy(&d->mutex);
		return (1);
	}
	return (0);
}

void	dongle_destroy(t_dongle *d)
{
	dq_free(&d->waiters);
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->cond);
}

void	fill_timespec(struct timespec *ts, long target_time_ms, t_dongle *d)
{
	ts->tv_sec = target_time_ms / 1000;
	ts->tv_nsec = (target_time_ms % 1000) * 1000000;
	pthread_cond_timedwait(&d->cond, &d->mutex, ts);
}

int	is_valid_arg(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

void	special_usleep(t_rules *r, long duration_ms)
{
	long	end;

	end = get_time_ms() + duration_ms;
	while (!is_stopped(r) && get_time_ms() < end)
		usleep(1000);
}
