/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouyer <mbouyer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:54:30 by mickael           #+#    #+#             */
/*   Updated: 2026/06/03 08:36:28 by mbouyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>
# include <ctype.h>

# define LOG_TAKEN "has taken a dongle"
# define LOG_COMPILE "is compiling"
# define LOG_DEBUG "is debugging"
# define LOG_REFACTORE "is refactoring"
# define LOG_BURNOUT "burned out"

typedef struct s_rules	t_rules;

typedef struct s_dq_node
{
	int		coder_id;
	long	priority;
}	t_dq_node;

typedef struct s_dq
{
	t_dq_node	*data;
	int			size;
	int			capacity;
}	t_dq;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	long			available_at;
	t_dq			waiters;
	t_rules			*rules;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			left_dongle;
	int			right_dongle;
	int			nb_compiles;
	long		last_compiled;
	long		deadline;
	int			burned_out;
	pthread_t	coder_thread;
	t_rules		*rules;
}	t_coder;

typedef struct s_rules
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				nb_compiles_required;
	long			dongle_cooldown;
	char			*scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
	long			time_start;
	int				stopped;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor_thread;
}	t_rules;

// coder
void		stopped_update(t_rules *r);
void		*processing(void *arg);

// dongle
int			request_dongle(t_dongle *d, t_coder *c);
void		release_dongle(t_dongle *d, t_rules *r);

// dq
void		swap_nodes(t_dq_node *a, t_dq_node *b);
int			dq_push(t_dq *dq, int coder_id, long priority);
t_dq_node	dq_pop(t_dq *dq);

// init
int			coder_init(t_rules *r);
int			parsing(t_rules *r, int ac, char **av);
int			rules_init(t_rules *r);

// log
void		log_state(t_coder *coder, char *msg);
void		log_burnout(t_coder *coder, char *msg);

// monitor
void		*monitor(void *arg);

// sim
int			sim_init(t_rules *r, int ac, char **av);
int			sim_run_create(t_rules *r);
void		sim_run_join(t_rules *r);
void		sim_cleaning(t_rules *r);

// utils_coder
int			d1(t_coder *coder);
int			d2(t_coder *coder);
int			check_all_compiled(t_rules *r);
int			get_left_idx(t_coder *coder);
int			get_right_idx(t_coder *coder);

// utils coder 2
long		get_priority(t_rules *r, t_coder *c);

// utils_dongle
int			dongle_init(t_dongle *d, t_rules *r);
void		dongle_destroy(t_dongle *d);
void		fill_timespec(struct timespec *ts, long target_time_ms,
				t_dongle *d);
int			is_valid_arg(const char *str);
void		special_usleep(t_rules *r, long duration_ms);

// utils_dq
int			dq_init(t_dq *dq, int cap);
void		dq_free(t_dq *dq);
int			dq_peek_id(t_dq *dq);
void		shift_up(t_dq *dq, int i);
void		shift_down(t_dq *dq, int i);

// utils
int			coder_only(t_coder *coder);
long		get_time_ms(void);
long		elapsed_time_ms(long start);
int			is_stopped(t_rules *r);
void		check_compiles(t_rules *r);
int			ft_atoi(const char *str);

#endif
