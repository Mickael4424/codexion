/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:34:10 by mickael           #+#    #+#             */
/*   Updated: 2026/06/09 10:20:14 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, char *msg)
{
	long	ts;

	pthread_mutex_lock(&coder->rules->log_mutex);
	pthread_mutex_lock(&coder->rules->stop_mutex);
	if (coder->rules->stopped)
	{
		pthread_mutex_unlock(&coder->rules->stop_mutex);
		pthread_mutex_unlock(&coder->rules->log_mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->rules->stop_mutex);
	ts = elapsed_time_ms(coder->rules->time_start);
	printf("%ld %d %s\n", ts, coder->id, msg);
	pthread_mutex_unlock(&coder->rules->log_mutex);
}

void	log_burnout(t_coder *coder, char *msg)
{
	long	ts;
	int		i;
	t_rules	*r;

	r = coder->rules;
	pthread_mutex_lock(&r->log_mutex);
	if (coder->burned_out)
		return (pthread_mutex_unlock(&r->log_mutex), (void)0);
	coder->burned_out = 1;
	ts = elapsed_time_ms(r->time_start);
	printf("%ld %d %s\n", ts, coder->id, msg);
	pthread_mutex_unlock(&r->log_mutex);
	pthread_mutex_lock(&r->stop_mutex);
	r->stopped = 1;
	pthread_mutex_unlock(&r->stop_mutex);
	i = -1;
	while (++i < r->nb_coders)
	{
		pthread_mutex_lock(&r->dongles[i].mutex);
		pthread_cond_broadcast(&r->dongles[i].cond);
		pthread_mutex_unlock(&r->dongles[i].mutex);
	}
}
