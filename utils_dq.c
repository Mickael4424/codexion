/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dq.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 09:10:31 by mbouyer           #+#    #+#             */
/*   Updated: 2026/06/03 08:38:43 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dq_init(t_dq *dq, int cap)
{
	dq->data = malloc(sizeof(t_dq_node) * cap);
	if (!dq->data)
		return (1);
	dq->size = 0;
	dq->capacity = cap;
	return (0);
}

void	dq_free(t_dq *dq)
{
	free(dq->data);
	dq->data = NULL;
	dq->size = 0;
}

int	dq_peek_id(t_dq *dq)
{
	if (dq->size == 0)
		return (-1);
	return (dq->data[0].coder_id);
}

void	shift_up(t_dq *dq, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (dq->data[i].priority > dq->data[parent].priority)
			break ;
		swap_nodes(&dq->data[parent], &dq->data[i]);
		i = parent;
	}
}

void	shift_down(t_dq *dq, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < dq->size
			&& dq->data[left].priority < dq->data[smallest].priority)
			smallest = left;
		if (right < dq->size
			&& dq->data[right].priority < dq->data[smallest].priority)
			smallest = right;
		if (smallest == i)
			break ;
		swap_nodes(&dq->data[i], &dq->data[smallest]);
		i = smallest;
	}
}
