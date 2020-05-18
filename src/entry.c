/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 15:07:46 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 15:36:28 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define MODE_SIZE 11

static char					*st_basename(char *path)
{
	char	*slash_ptr;

	if (path[0] == '/' && path[1] == '\0')
		return (path);
	if ((slash_ptr = ft_strrchr(path, '/')) == NULL)
		return (path);
	return (slash_ptr + 1);
}

char	*st_date_str(const time_t *timep)
{
	char	*date_str;

	date_str = ctime(timep);
	date_str = ft_strchr(date_str, ' ') + 1;
	if ((date_str = ft_strdup(date_str)) == NULL)
		return (NULL);
	*ft_strrchr(date_str, ':') = '\0';
	return (date_str);
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

bool						entry_push(
		char *filename, struct stat *statbuf, t_ftdstr *out, t_flags flags, unsigned int padding)
{
	char			*tmp;
	char			mode_str[MODE_SIZE];
	struct passwd	*usr_result;
	struct group	*grp_result;
	char			*date;

	if (flags & FLAG_LIST)
	{
		if ((usr_result = getpwuid(statbuf->st_uid)) == NULL
			|| (grp_result = getgrgid(statbuf->st_gid)) == NULL
			|| (date = st_date_str(&statbuf->st_mtim.tv_sec)) == NULL)
			return (false);
		st_fill_mode(mode_str, statbuf->st_mode);
		if (ft_asprintf(&tmp, " %d %s %s %*lu %s ",
				statbuf->st_nlink,
				usr_result->pw_name,
				grp_result->gr_name,
				padding,
				statbuf->st_size,
				date) == -1)
			return (false);
		if (ft_dstrpush(out, mode_str) == NULL
			|| ft_dstrpush(out, tmp) == NULL)
			return (false);
		free(tmp);
	}
	if (ft_dstrpush(out, st_basename(filename)) == NULL
		|| ft_dstrpush(out, "\n") == NULL)
		return (false);
	return (true);
}

static unsigned int			st_max_padding_func(struct stat *stat, unsigned int max_len)
{
	unsigned int	len;
	off_t			tmp_size;

	if (stat->st_size == 0)
		len = 1;
	else
	{
		len = 0;
		tmp_size = stat->st_size;
		while (tmp_size > 0)
		{
			tmp_size /= 10;
			len++;
		}
	}
	return (len > max_len ? len : max_len);
}

# define LS_BLOCK_SIZE 1024

static unsigned int			st_total_blocks_func(struct stat *stat, unsigned int blocks)
{
	return (blocks + (stat->st_blocks * stat->st_blksize / (8 * LS_BLOCK_SIZE)));
}

bool						entries_push(t_files *files, t_ftdstr *out, t_flags flags)
{
	size_t			i;
	unsigned int	padding;
	char			total_str[32];

	if (files->size == 0)
		return (true);
	padding = 0;
	if (flags & FLAG_LIST)
	{
		ft_sprintf(total_str, "total %u\n", files_reduce_stats(files, st_total_blocks_func));
		if (ft_dstrpush(out, total_str) == NULL)
			return (false);
		padding = files_reduce_stats(files, st_max_padding_func);
	}
	i = 0;
	while (i < files->size)
	{
		if (!entry_push(files->names->data[i], files->stats + i, out, flags, padding))
			return (false);
		i++;
	}
	return (true);
}
