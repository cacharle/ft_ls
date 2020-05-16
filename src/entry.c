/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 15:07:46 by charles           #+#    #+#             */
/*   Updated: 2020/05/16 21:15:43 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define MODE_SIZE 11

static char					*st_basename(char *path)
{
	char	*slash_ptr;

	if (ft_strequ(path, "/"))
		return (path);
	if ((slash_ptr = ft_strrchr(path, '/')) == NULL)
		return (path);
	return slash_ptr + 1;
}

struct s_file_type_letter	g_file_types[] = {
	{S_IFREG, '-'},
	{S_IFDIR, 'd'},
	{S_IFLNK, 'l'},
	{S_IFBLK, 'b'},
	{S_IFCHR, 'c'},
	{S_IFIFO, 'p'},
	{S_IFSOCK, 's'}
};

static void					st_fill_mode(char mode_str[MODE_SIZE], mode_t mode)
{
	char	type_letter;
	size_t	i;

	i = -1;
	while (++i < sizeof(g_file_types) / sizeof(g_file_types[0]))
		if (g_file_types[i].type == (mode & S_IFMT))
		{
			type_letter = g_file_types[i].letter;
			break ;
		}
	mode_str[0] = type_letter,
	mode_str[1] = mode & S_IRUSR ? 'r' : '-';
	mode_str[2] = mode & S_IWUSR ? 'w' : '-';
	mode_str[3] = mode & S_IXUSR ? 'x' : '-';
	mode_str[4] = mode & S_IRGRP ? 'r' : '-';
	mode_str[5] = mode & S_IWGRP ? 'w' : '-';
	mode_str[6] = mode & S_IXGRP ? 'x' : '-';
	mode_str[7] = mode & S_IROTH ? 'r' : '-';
	mode_str[8] = mode & S_IWOTH ? 'w' : '-';
	mode_str[9] = mode & S_IXOTH ? 'x' : '-';
	mode_str[10] = '\0';
}

/*
** -l format
** permissiosn links user group size date filename [-> linked]
*/

bool						entry_push(char *filename, t_ftdstr *out, t_flags flags)
{
	char			*tmp;
	char			mode_str[MODE_SIZE];
	struct stat		statbuf;
	struct passwd	*usr_result;
	struct group	*grp_result;

	if (flags & FLAG_LIST)
	{
		if (stat(filename, &statbuf) < 0
			|| (usr_result = getpwuid(statbuf.st_uid)) == NULL
			|| (grp_result = getgrgid(statbuf.st_uid)) == NULL)
			return (false);
		st_fill_mode(mode_str, statbuf.st_mode);
		ft_asprintf(&tmp, " %d %s %s %lu %s ",
				statbuf.st_nlink,
				usr_result->pw_name,
				grp_result->gr_name,
				statbuf.st_size,
				"some date");
		ft_dstrpush(out, mode_str);
		ft_dstrpush(out, tmp);
	}
	ft_dstrpush(out, st_basename(filename));
	ft_dstrpush(out, "\n");
	return (true);
}
