/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 17:55:36 by charles           #+#    #+#             */
/*   Updated: 2020/05/17 20:04:28 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	st_compar_str_rev(const void *s1_p, const void *s2_p)
{
	return (-ft_strcasecmp(*(char**)s1_p, *(char**)s2_p));
}

static void	st_time_sort(t_ftvec *filenames, struct stat *stats, bool reverse)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (i < filenames->size)
	{
		j = i;
		while (j > 0 && reverse ? stats[j - 1].st_mtim.tv_sec < stats[j].st_mtim.tv_sec :
									stats[j - 1].st_mtim.tv_sec > stats[j].st_mtim.tv_sec)
		{
			ft_memswap(stats + j - 1, stats + j, sizeof(struct stat));
			ft_memswap(filenames->data + j - 1, filenames->data + j, sizeof(void*));
			j--;
		}
		i++;
	}
}

void	order_filenames(t_ftvec *filenames, struct stat *stats, t_flags flags)
{
	if (flags & FLAG_TIME)
		st_time_sort(filenames, stats, !(flags & FLAG_REVERSE));
	else
		ft_vecsort(filenames, flags & FLAG_REVERSE ?
				st_compar_str_rev : (t_ftcompar_func)ft_compar_str_case);

}
