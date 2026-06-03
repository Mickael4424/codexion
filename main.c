/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:33:33 by mickael           #+#    #+#             */
/*   Updated: 2026/06/03 09:20:26 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_rules	r;

	if (ac != 9)
	{
		printf("Error: invalid arguments. \n");
		printf("Usage: ");
		printf("./codexion nb_of_coders t_to_burnout t_to_compile ");
		printf("t_to_debug t_to_refactor nb_of_compiles_required ");
		printf("dongle_cooldown scheduler\n");
		return (1);
	}
	if (ft_atoi(av[6]) == 0 && is_valid_arg(av[6]) == 0)
		return (1);
	if (sim_init(&r, ac, av))
		return (1);
	if (sim_run_create(&r))
	{
		sim_cleaning(&r);
		return (1);
	}
	sim_run_join(&r);
	sim_cleaning(&r);
	return (0);
}
