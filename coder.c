/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 20:03:23 by mickael           #+#    #+#             */
/*   Updated: 2026/06/03 08:34:34 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	acquire_dongle(t_coder *coder)
{
	t_rules	*r;

	r = coder->rules;
	if (coder_only(coder) == 1)
		return (1);
	if (request_dongle(&r->dongles[d1(coder)], coder) != 0)
		return (1);
	log_state(coder, LOG_TAKEN);
	if (is_stopped(r))
	{
		release_dongle(&r->dongles[d1(coder)], r);
		return (1);
	}
	if (request_dongle(&r->dongles[d2(coder)], coder) != 0)
	{
		release_dongle(&r->dongles[d1(coder)], r);
		return (1);
	}
	log_state(coder, LOG_TAKEN);
	return (0);
}

static void	release_dongles(t_coder *coder)
{
	t_rules	*r;
	int		left;
	int		right;

	r = coder->rules;
	if (r->nb_coders == 1)
	{
		release_dongle(&r->dongles[0], r);
		return ;
	}
	left = get_left_idx(coder);
	right = get_right_idx(coder);
	release_dongle(&r->dongles[left], r);
	release_dongle(&r->dongles[right], r);
}

static int	do_compile(t_coder *coder)
{
	t_rules	*r;

	r = coder->rules;
	if (acquire_dongle(coder) != 0)
		return (1);
	if (is_stopped(r))
	{
		release_dongles(coder);
		return (1);
	}
	pthread_mutex_lock(&r->stop_mutex);
	coder->last_compiled = get_time_ms();
	coder->deadline = coder->last_compiled + r->time_to_burnout;
	coder->nb_compiles += 1;
	pthread_mutex_unlock(&r->stop_mutex);
	log_state(coder, LOG_COMPILE);
	special_usleep(r, coder->rules->time_to_compile);
	release_dongles(coder);
	return (0);
}

void	stopped_update(t_rules *r)
{
	int	i;

	pthread_mutex_lock(&r->stop_mutex);
	r->stopped = 1;
	pthread_mutex_unlock(&r->stop_mutex);
	i = 0;
	while (i < r->nb_coders)
	{
		pthread_mutex_lock(&r->dongles[i].mutex);
		pthread_cond_broadcast(&r->dongles[i].cond);
		pthread_mutex_unlock(&r->dongles[i].mutex);
		i++;
	}
}

void	*processing(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	while (1)
	{
		if (is_stopped(c->rules))
			break ;
		if (do_compile(c) != 0)
			break ;
		if (is_stopped(c->rules))
			break ;
		if (check_all_compiled(c->rules))
			break ;
		if (is_stopped(c->rules))
			break ;
		log_state(c, LOG_DEBUG);
		special_usleep(c->rules, c->rules->time_to_debug);
		if (is_stopped(c->rules))
			break ;
		log_state(c, LOG_REFACTORE);
		special_usleep(c->rules, c->rules->time_to_refactor);
	}
	return (NULL);
}
