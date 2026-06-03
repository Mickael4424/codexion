/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_coder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 20:10:43 by mickael           #+#    #+#             */
/*   Updated: 2026/05/20 11:27:28 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	d1(t_coder *coder)
{
	int		first;
	int		left;
	int		right;

	left = get_left_idx(coder);
	right = get_right_idx(coder);
	if (left < right)
		first = left;
	else
		first = right;
	return (first);
}

int	d2(t_coder *coder)
{
	int		second;
	int		left;
	int		right;

	left = get_left_idx(coder);
	right = get_right_idx(coder);
	if (left < right)
		second = right;
	else
		second = left;
	return (second);
}

int	check_all_compiled(t_rules *r)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	pthread_mutex_lock(&r->stop_mutex);
	while (i < r->nb_coders)
	{
		if (r->coders[i].nb_compiles < r->nb_compiles_required)
		{
			done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&r->stop_mutex);
	if (done)
		stopped_update(r);
	return (done);
}

int	get_left_idx(t_coder *coder)
{
	return (coder->id - 1);
}

int	get_right_idx(t_coder *coder)
{
	return (coder->id % coder->rules->nb_coders);
}
