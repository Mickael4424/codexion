/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dq.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:38:49 by mickael           #+#    #+#             */
/*   Updated: 2026/06/02 11:58:58 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_dq_node *a, t_dq_node *b)
{
	t_dq_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	dq_push(t_dq *dq, int coder_id, long priority)
{
	if (dq->size >= dq-> capacity)
		return (1);
	dq->data[dq->size].coder_id = coder_id;
	dq->data[dq->size].priority = priority;
	shift_up(dq, dq->size);
	dq->size++;
	return (0);
}

t_dq_node	dq_pop(t_dq *dq)
{
	t_dq_node	top;

	top = dq->data[0];
	dq->size--;
	if (dq->size > 0)
	{
		dq->data[0] = dq->data[dq->size];
		shift_down(dq, 0);
	}
	return (top);
}
