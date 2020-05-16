/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:38:02 by charles           #+#    #+#             */
/*   Updated: 2020/05/16 21:14:02 by charles          ###   ########.fr       */
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

t_ftvec	*extract_dirs(t_ftvec *files)
{
	size_t	i;
	t_ftvec	*dirs;
	struct stat	statbuf;

	dirs = ft_vecnew(32);
	i = 0;
	while (i < files->size)
	{
		stat(files->data[i], &statbuf);
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
			ft_vecpush(dirs, files->data[i]);
		i++;
	}
	return (dirs);
}

void	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags);

void	files_push(t_ftvec *files, t_ftdstr *out, t_flags flags)
{
	size_t	i;
	t_ftvec	*dirs;

	if (flags & FLAG_RECURSION)
	{
		dirs = extract_dirs(files);
	}
	ft_vecsort(files, flags & FLAG_TIME ? st_compar_time : (t_ftcompar_func)ft_strcmp);
	if (flags & FLAG_REVERSE)
		ft_vecreverse(files);
	i = 0;
	while (i < files->size)
	{
		entry_push(files->data[i], out, flags);
		i++;
	}
	if (flags & FLAG_RECURSION)
		dirs_push(dirs, out, flags);
}

void	dirs_push(t_ftvec *dirs, t_ftdstr *out, t_flags flags)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			i;
	t_ftvec			*files;

	i = 0;
	while (i < dirs->size)
	{
		dir = opendir(dirs->data[i]);
		files = ft_vecnew(32);
		while ((entry = readdir(dir)) != NULL)
		{
			if (ft_strequ(entry->d_name, ".") || ft_strequ(entry->d_name, ".."))
				continue ;
			ft_vecpush_safe(files, ft_strjoin3(dirs->data[i], "/", entry->d_name));
		}
		closedir(dir);
		ft_dstrpush(out, "\n");
		ft_dstrpush(out, dirs->data[i]);
		ft_dstrpush(out, ":\n");
		files_push(files, out, flags);
		i++;
	}
}

void	entrypoint_push(t_ftvec *files, t_ftdstr *out, t_flags flags)
{
	size_t	i;
	t_ftvec	*dirs;

	dirs = extract_dirs(files);

	ft_vecsort(files, flags & FLAG_TIME ? st_compar_time : (t_ftcompar_func)ft_strcmp);
	if (flags & FLAG_REVERSE)
		ft_vecreverse(files);
	i = 0;
	while (i < files->size)
	{
		entry_push(files->data[i], out, flags);
		i++;
	}
	dirs_push(dirs, out, flags);
}

int main(int argc, char **argv)
{
	t_flags flags;
	t_ftvec	*files;

	if ((flags = flags_extract(argc, argv)) & FLAG_ERROR)
		return (1);

	files = ft_vecnew(256);
	int i = 1;
	while (argv[i] != NULL)
	{
		ft_vecpush(files, ft_strdup(argv[i]));
		i++;
	}
	t_ftdstr *out = ft_dstrnew("");
	entrypoint_push(files, out, flags);
	ft_putstr(out->str);
	return 0;
}
