/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:38:02 by charles           #+#    #+#             */
/*   Updated: 2020/05/17 18:54:32 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define FT_LS_OUT_SIZE 1024

struct stat	*stats_new(t_ftvec *filenames)
{
	size_t		i;
	struct stat	*stats;

	if ((stats = malloc(sizeof(struct stat) * filenames->size)) == NULL)
		return (NULL);
	i = 0;
	while (i < filenames->size)
	{
		if (stat(filenames->data[i], stats + i) == -1)
		{
			free(stats);
			return (NULL);
		}
		i++;
	}
	return (stats);
}


t_ftvec	*extract_dirs(t_ftvec *filenames, struct stat *stats)
{
	size_t		i;
	t_ftvec		*dirs;

	if ((dirs = ft_vecnew(filenames->size)) == NULL)
		return (NULL);
	i = 0;
	while (i < filenames->size)
	{
		if ((stats[i].st_mode & S_IFMT) == S_IFDIR
			&& ft_vecpush(dirs, filenames->data[i]) == NULL)
			return (NULL);
		i++;
	}
	return (dirs);
}

bool	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags);

bool	files_push(t_ftvec *filenames, t_ftdstr *out, t_flags flags)
{
	t_ftvec		*dirs;
	struct stat	*stats;

	if (filenames->size == 0)
		return (true);
	if ((stats = stats_new(filenames)) == NULL)
		return (false);
	if (flags & FLAG_RECURSION)
	{
		if ((dirs = extract_dirs(filenames, stats)) == NULL)
			return (false);
	}
	entries_push(filenames, stats, out, flags);
	if (flags & FLAG_RECURSION)
		return (dirs_push(dirs, out, flags));
	return (true);
}

bool	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			i;
	t_ftvec			*filenames;

	i = 0;
	while (i < dirs->size)
	{
		if ((dir = opendir(dirs->data[i])) == NULL
			|| (filenames = ft_vecnew(32)) == NULL)
			return (false);
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] == '.')
				continue ;
			if (ft_vecpush_safe(filenames, ft_strjoin3(dirs->data[i], "/", entry->d_name)) == NULL)
				return (false);
		}
		closedir(dir);
		if (ft_dstrpush(out, "\n") == NULL
			|| ft_dstrpush(out, dirs->data[i]) == NULL
			|| ft_dstrpush(out, ":\n") == NULL
			|| !files_push(filenames, out, flags))
			return (false);
		i++;
	}
	return (true);
}

bool	entrypoint_push(t_ftvec *filenames, t_ftdstr *out, t_flags flags)
{
	t_ftvec		*dirs;
	struct stat	*stats;

	if (filenames->size == 0)
		if (ft_vecpush_safe(filenames, ft_strdup(".")) == NULL)
			return (false);
	if ((stats = stats_new(filenames)) == NULL)
		return (false);
	if ((dirs = extract_dirs(filenames, stats)) == NULL)
		return (false);
	entries_push(filenames, stats, out, flags);
	return (dirs_push(dirs, out, flags));
}

int main(int argc, char **argv)
{
	t_flags		flags;
	t_ftvec		*filenames;
	t_ftdstr	*out;
	int			i;

	if ((flags = flags_extract(argc, argv)) & FLAG_ERROR)
		return (1);
	if ((filenames = ft_vecnew(256)) == NULL
		|| (out = ft_dstrnew_empty(FT_LS_OUT_SIZE)) == NULL)
		return (1);
	i = 1;
	while (argv[i] != NULL)
	{
		if (argv[i][ft_strlen(argv[i]) - 1] == '/')
			argv[i][ft_strlen(argv[i]) - 1] = '\0';
		ft_vecpush(filenames, ft_strdup(argv[i++]));
	}
	if (!entrypoint_push(filenames, out, flags))
	{
		perror(NULL);
		return (1);
	}
	ft_putstr(out->str);
	ft_dstrdestroy(out);
	ft_vecdestroy(filenames, free);
	return 0;
}
