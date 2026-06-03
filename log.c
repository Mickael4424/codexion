/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:34:10 by mickael           #+#    #+#             */
/*   Updated: 2026/06/03 08:37:07 by mbouyer          ###   ########.fr       */
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
	coder->burned_out = 1;
	pthread_mutex_lock(&coder->rules->log_mutex);
	ts = elapsed_time_ms(coder->rules->time_start);
	printf("%ld %d %s\n", ts, coder->id, msg);
	pthread_mutex_unlock(&coder->rules->log_mutex);
	pthread_mutex_lock(&coder->rules->stop_mutex);
	coder->rules->stopped = 1;
	pthread_mutex_unlock(&coder->rules->stop_mutex);
	i = 0;
	while (i < r->nb_coders)
	{
		pthread_mutex_lock(&r->dongles[i].mutex);
		pthread_cond_broadcast(&r->dongles[i].cond);
		pthread_mutex_unlock(&r->dongles[i].mutex);
		i++;
	}
}
