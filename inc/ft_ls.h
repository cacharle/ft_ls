/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:48:11 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 16:17:01 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <grp.h>
# include <pwd.h>
# include <time.h>

# include "libft.h"
# include "libft_printf.h"
# include "libft_dstr.h"
# include "libft_lst.h"
# include "libft_vec.h"

# define FLAG_LIST       1 << 0
# define FLAG_RECURSION  1 << 1
# define FLAG_ALL        1 << 2
# define FLAG_REVERSE    1 << 3
# define FLAG_TIME       1 << 4

# define FLAG_ERROR      1 << 7

/*
** Flag character *must* be in the same order as flags bit masks
*/

# define FLAGS_STRING "lRart"

typedef uint8_t	t_flags;

struct			s_file_type_letter
{
	mode_t		type;
	char		letter;
};

typedef struct
{
	size_t		size;
	t_ftvec		*names;
	struct stat	*stats;
}				t_files;

/*
** args.c
*/

t_flags			args_extract_flags(int argc, char **argv);

/*
** entry.c
*/

bool			entries_push(t_files *files, t_ftdstr *out, t_flags flags);

/*
** order.c
*/

void			order_files(t_files *files, t_flags flags);

/*
** files.c
*/

bool			files_init(t_files *files, t_ftvec *filenames);
void			files_quit(t_files *files);
unsigned int	files_reduce_stats(t_files *files, unsigned int (*f)(struct stat*, unsigned int));
t_ftvec			*files_extract_dirnames(t_files *files);
void			files_remove_dirs(t_files *files);

/*
** push.c
*/

bool			push_dirs(t_ftvec *dirnames, t_ftdstr *out, t_flags flags);
bool			push_files(t_ftvec *filenames, t_ftdstr *out, t_flags flags);

#endif
