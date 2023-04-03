/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbarreto <mbarreto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:10:14 by mbarreto          #+#    #+#             */
/*   Updated: 2023/04/03 16:37:12 by mbarreto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h" 

int checknum(char *arg)
{
	char	str[12] = "+-0123456789";
	size_t	i;
	size_t	j;

	i = -1;
	if (!arg)
		return (-1);
	while (arg[++i])
	{
		j = -1;
		while (str[++j])
		{
			if (arg[i] == str[j])
				break ;
		}
		if (arg[i] == str[j])
			continue ;
		return (-1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	d;
	int	i;

	i = ac - 1;
	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments\n");
		exit(0);
	}
	while (i > 0)
	{
		if (checknum(av[i]) == -1)
		{
			printf("Wrong arguments, insert only numbers\n");
			return (0);
		}
		i--;
	}
	if (ft_init(&d, ac, av) == -1)
		return (0);
	if (d.philo_num > 1)
		if (work(&d))
			return (printf("Error while creating the threads\n"));
	return (0);
}
