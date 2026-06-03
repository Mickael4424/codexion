/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:27:54 by mickael           #+#    #+#             */
/*   Updated: 2026/06/02 16:57:03 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_only(t_coder *coder)
{
	t_rules	*r;

	r = coder->rules;
	if (r->nb_coders == 1)
	{
		if (request_dongle(&r->dongles[0], coder) != 0)
			return (1);
		log_state(coder, LOG_TAKEN);
		usleep(r->time_to_burnout * 1000);
		log_burnout(coder, LOG_BURNOUT);
		return (1);
	}
	return (0);
}

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((long)time.tv_sec * 1000 + (long)time.tv_usec / 1000);
}

long	elapsed_time_ms(long start)
{
	return (get_time_ms() - start);
}

int	is_stopped(t_rules *r)
{
	int	s;

	pthread_mutex_lock(&r->stop_mutex);
	s = r->stopped;
	pthread_mutex_unlock(&r->stop_mutex);
	return (s);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	if ((res * sign) > 2147483647 || (res * sign) < -2147483648)
		return (-1);
	return ((int)(res * sign));
}
