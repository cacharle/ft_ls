/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 13:59:02 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 16:13:55 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_ftvec	*st_dir_filenames(char *dirname, bool all)
{
	DIR				*dir;
	struct dirent	*entry;
	t_ftvec			*filenames;

	if ((dir = opendir(dirname)) == NULL
		|| (filenames = ft_vecnew(32)) == NULL)
	{
		closedir(dir);
		return (false);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (!all && entry->d_name[0] == '.')
			continue ;
		if (ft_vecpush_safe(filenames, ft_strjoin3(dirname, "/", entry->d_name)) == NULL)
		{
			ft_vecdestroy(filenames, free);
			closedir(dir);
			return (false);
		}
	}
	closedir(dir);
	return (filenames);
}

bool			push_dirs(t_ftvec *dirnames, t_ftdstr *out, t_flags flags)
{
	size_t			i;
	t_ftvec			*filenames;

	i = 0;
	while (i < dirnames->size)
	{
		if ((filenames = st_dir_filenames(dirnames->data[i], flags & FLAG_ALL)) == NULL)
			return (false);
		if (dirnames->size > 1)
		{
			if (ft_dstrpush(out, dirnames->data[i]) == NULL
				|| ft_dstrpush(out, ":\n") == NULL)
				return (false);
		}
		if (!push_files(filenames, out, flags))
			return (false);
		if (dirnames->size > 1 && i != dirnames->size - 1)
		{
			if (ft_dstrpush(out, "\n") == NULL)
				return (false);
		}
		i++;
	}
	return (true);
}

bool			push_files(t_ftvec *filenames, t_ftdstr *out, t_flags flags)
{
	t_ftvec		*dirnames;
	t_files		files;

	if (filenames->size == 0)
		return (true);
	if (!files_init(&files, filenames))
		return (false);
	order_files(&files, flags);
	if (flags & FLAG_RECURSION
		&& (dirnames = files_extract_dirnames(&files)) == NULL)
		{
			files_quit(&files);
			return (false);
		}
	if (!entries_push(&files, out, flags))
		return (false);
	if (flags & FLAG_RECURSION)
		return (push_dirs(dirnames, out, flags));
	return (true);
}

