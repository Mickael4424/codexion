/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:45:59 by mickael           #+#    #+#             */
/*   Updated: 2026/06/03 08:36:38 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	cond_1(t_dongle *d, t_coder *c, long now)
{
	if (d->available
		&& dq_peek_id(&d->waiters) == c->id
		&& d->available_at <= now)
	{
		dq_pop(&d->waiters);
		d->available = 0;
		pthread_mutex_unlock(&d->mutex);
		return (1);
	}
	return (0);
}

static void	dq_remove(t_dq *dq, int coder_id)
{
	int	i;

	i = 0;
	while (i < dq->size)
	{
		if (dq->data[i].coder_id == coder_id)
		{
			dq->size--;
			if (i < dq->size)
			{
				dq->data[i] = dq->data[dq->size];
				shift_down(dq, i);
				shift_up(dq, i);
			}
			return ;
		}
		i++;
	}
}

static int	check_stopped(t_dongle *d, t_coder *c)
{
	int	stopped;

	pthread_mutex_unlock(&d->mutex);
	pthread_mutex_lock(&d->rules->stop_mutex);
	stopped = d->rules->stopped;
	pthread_mutex_unlock(&d->rules->stop_mutex);
	pthread_mutex_lock(&d->mutex);
	if (stopped)
	{
		dq_remove(&d->waiters, c->id);
		pthread_cond_broadcast(&d->cond);
		pthread_mutex_unlock(&d->mutex);
		return (1);
	}
	return (0);
}

int	request_dongle(t_dongle *d, t_coder *c)
{
	long			priority;
	long			now;
	struct timespec	ts;

	if (is_stopped(c->rules))
		return (1);
	priority = get_priority(c->rules, c);
	if (pthread_mutex_lock(&d->mutex) != 0)
		return (1);
	dq_push(&d->waiters, c->id, priority);
	while (1)
	{
		if (check_stopped(d, c) == 1)
			return (1);
		now = get_time_ms();
		if (cond_1(d, c, now) == 1)
			return (0);
		if (!d->available || dq_peek_id(&d->waiters) != c->id)
			pthread_cond_wait(&d->cond, &d->mutex);
		else
			fill_timespec(&ts, d->available_at, d);
	}
	return (0);
}

void	release_dongle(t_dongle *d, t_rules *r)
{
	pthread_mutex_lock(&d->mutex);
	d->available = 1;
	d->available_at = get_time_ms() + r->dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
