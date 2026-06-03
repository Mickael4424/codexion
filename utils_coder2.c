/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_coder2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 08:35:50 by mbouyer           #+#    #+#             */
/*   Updated: 2026/06/03 08:38:48 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_priority(t_rules *r, t_coder *c)
{
	if (strcmp(r->scheduler, "fifo") == 0)
		return (get_time_ms());
	return (c->deadline);
}
