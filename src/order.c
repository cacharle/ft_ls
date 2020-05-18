/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 17:55:36 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 16:13:12 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	st_sort_time(t_files *files)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (i < files->size)
	{
		j = i;
		while (j > 0 && files->stats[j - 1].st_mtim.tv_sec > files->stats[j].st_mtim.tv_sec)
		{
			ft_memswap(files->stats + j - 1, files->stats + j, sizeof(struct stat));
			ft_memswap(files->names->data + j - 1, files->names->data + j, sizeof(void*));
			j--;
		}
		i++;
	}
}

static void	st_sort_name(t_files *files)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (i < files->size)
	{
		j = i;
		while (j > 0 && ft_strcasecmp(files->names->data[j - 1], files->names->data[j]) > 0)
		{
			ft_memswap(files->names->data + j - 1, files->names->data + j, sizeof(void*));
			ft_memswap(files->stats + j - 1, files->stats + j, sizeof(struct stat));
			j--;
		}
		i++;
	}
}

void	order_files(t_files *files, t_flags flags)
{
	if (flags & FLAG_TIME)
		st_sort_time(files);
	else
		st_sort_name(files);
	if (flags & FLAG_REVERSE)
	{
		ft_reverse(files->stats, files->size, sizeof(struct stat));
		ft_vecreverse(files->names);
	}
}
