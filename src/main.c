/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:38:02 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 16:14:00 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define FT_LS_OUT_SIZE 1024

bool	entrypoint_push(t_ftvec *filenames, t_ftdstr *out, t_flags flags)
{
	t_ftvec		*dirnames;
	t_files		files;

	if ((filenames->size == 0
		&& ft_vecpush_safe(filenames, ft_strdup(".")) == NULL)
		|| !files_init(&files, filenames))
		return (false);
	order_files(&files, flags);
	if ((dirnames = files_extract_dirnames(&files)) == NULL)
		return (false);
	files_remove_dirs(&files);
	entries_push(&files, out, flags);
	return (push_dirs(dirnames, out, flags));
}

int main(int argc, char **argv)
{
	t_flags		flags;
	t_ftvec		*filenames;
	t_ftdstr	*out;
	int			i;

	if ((flags = args_extract_flags(argc, argv)) & FLAG_ERROR)
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
