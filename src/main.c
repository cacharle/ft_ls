/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:38:02 by charles           #+#    #+#             */
/*   Updated: 2020/05/17 17:18:38 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define FT_LS_OUT_SIZE 1024

static int	st_compar_time(const void *f1, const void *f2)
{
	(void)f1;
	(void)f2;
	return 0;
}

struct stat	*load_files(t_ftvec *filenames)
{
	size_t		i;
	struct stat	*stats;

	if ((stats = malloc(sizeof(struct stat) * filenames->size)) == NULL)
		return (NULL);
	i = 0;
	while (i < filenames->size)
	{
		if (stat(filename->data[i], stats + i) == -1)
		{
			free(stats);
			return (NULL);
		}
		i++;
	}
	return (stats);
}

t_ftvec	*extract_dirs(t_ftvec *files)
{
	size_t		i;
	t_ftvec		*dirs;
	struct stat	statbuf;

	if ((dirs = ft_vecnew(32)) == NULL)
		return (NULL);
	i = 0;
	while (i < files->size)
	{
		if (stat(files->data[i], &statbuf) == -1)
			return (NULL);
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
			if (ft_vecpush(dirs, files->data[i]) == NULL)
				return (NULL);
		i++;
	}
	return (dirs);
}

bool	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags);

bool	files_push(t_ftvec *files, t_ftdstr *out, t_flags flags)
{
	size_t	i;
	t_ftvec	*dirs;

	if (files->size == 0)
		return (true);
	if (flags & FLAG_RECURSION)
	{
		if ((dirs = extract_dirs(files)) == NULL)
			return (false);
	}
	ft_vecsort(files, /*flags & FLAG_TIME ? st_compar_time : */(t_ftcompar_func)ft_strcmp);
	if (flags & FLAG_REVERSE)
		ft_vecreverse(files);
	i = 0;
	entries_push(files, load_files(files));
	/* while (i < files->size) */
	/* { */
	/* 	if (!entry_push(files->data[i], out, flags)) */
	/* 		return (false); */
	/* 	i++; */
	/* } */
	if (flags & FLAG_RECURSION)
		return (dirs_push(dirs, out, flags));
	return (true);
}

bool	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			i;
	t_ftvec			*files;

	i = 0;
	while (i < dirs->size)
	{
		if ((dir = opendir(dirs->data[i])) == NULL
			|| (files = ft_vecnew(32)) == NULL)
			return (false);
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_name[0] == '.')
				continue ;
			if (ft_vecpush_safe(files, ft_strjoin3(dirs->data[i], "/", entry->d_name)) == NULL)
				return (false);
		}
		closedir(dir);
		if (ft_dstrpush(out, "\n") == NULL
			|| ft_dstrpush(out, dirs->data[i]) == NULL
			|| ft_dstrpush(out, ":\n") == NULL
			|| !files_push(files, out, flags))
			return (false);
		i++;
	}
	return (true);
}

bool	entrypoint_push(t_ftvec *files, t_ftdstr *out, t_flags flags)
{
	size_t	i;
	t_ftvec	*dirs;

	ft_vecsort(files, /*flags & FLAG_TIME ? st_compar_time : */(t_ftcompar_func)ft_strcmp);
	if (flags & FLAG_REVERSE)
		ft_vecreverse(files);
	if ((dirs = extract_dirs(files)) == NULL)
		return (false);
	i = 0;
	while (i < files->size)
	{
		if (!entry_push(files->data[i], out, flags))
			return (NULL);
		i++;
	}
	return (dirs_push(dirs, out, flags));
}

int main(int argc, char **argv)
{
	t_flags		flags;
	t_ftvec		*files;
	t_ftdstr	*out;
	int			i;

	if ((flags = flags_extract(argc, argv)) & FLAG_ERROR)
		return (1);
	if ((files = ft_vecnew(256)) == NULL
		|| (out = ft_dstrnew_empty(FT_LS_OUT_SIZE)) == NULL)
		return (1);
	i = 1;
	while (argv[i] != NULL)
	{
		if (argv[i][ft_strlen(argv[i]) - 1] == '/')
			argv[i][ft_strlen(argv[i]) - 1] = '\0';
		ft_vecpush(files, ft_strdup(argv[i++]));
	}
	if (!entrypoint_push(files, out, flags))
	{
		perror(NULL);
		return (1);
	}
	ft_putstr(out->str);
	ft_dstrdestroy(out);
	ft_vecdestroy(files, free);
	return 0;
}
