/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 12:36:32 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 16:09:43 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

bool			files_init(t_files *files, t_ftvec *filenames)
{
	size_t	i;

	if ((files->stats = malloc(sizeof(struct stat) * filenames->size)) == NULL)
		return (false);
	files->size = filenames->size;
	files->names = filenames;
	i = 0;
	while (i < files->size)
	{
		if (stat(filenames->data[i], files->stats + i) == -1)
		{
			files_quit(files);
			return (false);
		}
		i++;
	}
	return (true);
}

void			files_quit(t_files *files)
{
	ft_vecdestroy(files->names, free);
	free(files->stats);
}

unsigned int	files_reduce_stats(t_files *files, unsigned int (*f)(struct stat*, unsigned int))
{
	size_t			i;
	unsigned int	acc;

	acc = 0;
	i = 0;
	while (i < files->size)
	{
		acc = f(files->stats + i, acc);
		i++;
	}
	return (acc);
}

t_ftvec			*files_extract_dirnames(t_files *files)
{
	size_t		i;
	t_ftvec		*dirnames;

	if ((dirnames = ft_vecnew(files->size)) == NULL)
		return (NULL);
	i = 0;
	while (i < files->size)
	{
		if ((files->stats[i].st_mode & S_IFMT) == S_IFDIR
			&& ft_vecpush_safe(dirnames, ft_strdup(files->names->data[i])) == NULL)
			return (NULL);
		i++;
	}
	return (dirnames);
}

void			files_remove_dirs(t_files *files)
{
	size_t		i;

	i = 0;
	while (i < files->size)
	{
		if ((files->stats[i].st_mode & S_IFMT) == S_IFDIR)
		{
			ft_vecremove(files->names, i, free);
			ft_memmove(
				files->stats + i,
				files->stats + i + 1,
				(files->size - i - 1) * sizeof(struct stat));
			files->size--;
			i--;
		}
		i++;
	}
}
