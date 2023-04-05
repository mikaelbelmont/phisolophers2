/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:17 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/05 22:43:07 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_table *table)
{
	t_data	data;

	data = table->data;
	pthread_mutex_lock(&table->data.forks[data.fork_left]);
	printer(&data, data.first_timestamp, table->id, "has taken a fork");
	pthread_mutex_unlock(&table->data.forks[data.fork_left]);
	if (check_dead(&data, table))
		return (1);
	pthread_mutex_lock(&table->data.forks[data.fork_right]);
	printer(&data, data.first_timestamp, table->id, "has taken a fork");
	pthread_mutex_unlock(&table->data.forks[data.fork_right]);
	if (check_dead(&data, table))
		return (1);
	printer(&data, data.first_timestamp, table->id, "is eating");
	(table->x_ate)++;
	//pthread_mutex_lock(&(data.util));
	table->last_meal_t = times() - table->start;
	//pthread_mutex_unlock(&(data.util));
	sleeping(data.eat_time, &data);
	return (0);
}

void	*philo_thread(void *voidphil)
{
	t_table			*table;
	t_data			data;

	table = (t_table *)voidphil;
	data = table->data;
	if (table->id % 2)
		usleep(10000);
	//pthread_mutex_lock(&(data.util));
	//table->last_meal_t = times();
	//pthread_mutex_unlock(&(data.util));
	while (!check_dead(&data, table))
	{
		if (philo_eat(table))
			return (NULL);
		if (data.all_ate)
			break ;
		printer(&data, data.first_timestamp, table->id, "is sleeping");
		sleeping(data.sleep_time, &data);
		printer(&data, data.first_timestamp, table->id, "is thinking");
	}
	return (NULL);
}

int		check_dead(t_data *d, t_table *t)
{
	int	death;
	
	pthread_mutex_lock(&(d->util2));
	death = d->death;
	pthread_mutex_unlock(&(d->util2));
	if (death)
		return (1);
	//printf("%lld %lld - %lld > %d\n", t->last_meal_t, times(), t->start, d->die_time);
	//pthread_mutex_lock(&(d->util));
	if (time_diff(t->last_meal_t, (times() - t->start)) >= d->die_time)
	{
		//printf("%lldaqui\n\n\n", t->last_meal_t);
		pthread_mutex_lock(&(d->deathlock));
		d->death = 1;
		if (d->death == 1)
			printer(d, d->first_timestamp, t->id, "died");
		pthread_mutex_unlock(&(d->deathlock));
		return (1);
	}
	//pthread_mutex_unlock(&(d->util));
	return (0);
}

void	sleeping(long long time, t_data *data)
{
	long long	i;
	int			death;

	pthread_mutex_lock(&(data->util2));
	death = data->death;
	pthread_mutex_unlock(&(data->util2));
	i = times();
	while (!(death))
	{
		if (time_diff(i, times()) >= time)
			break ;
		usleep(50);
	}
}

int	work(t_data *data, t_table *table)
{
	int		i;

	i = -1;
	//data->first_timestamp = times();
	table->start = times();
	while (++i < data->philo_num)
	{
		//printf("aquiiiiii\n");
		table[i].last_meal_t = 0;
		if (pthread_create(&(table[i].thread_id), NULL, \
		philo_thread, (table + i)))
			return (1);
	}
	check_dead(data, table);
	exit_launcher(data, table);
	free(table);
	return (0);
}
