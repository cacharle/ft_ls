/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:48:11 by charles           #+#    #+#             */
/*   Updated: 2020/05/17 18:28:27 by charles          ###   ########.fr       */
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

// typedef struct
// {
// 	char			*month_str;
// 	unsigned int	day;
// 	unsigned int	hour;
// 	unsigned int	min;
// }					t_date;

struct s_file_type_letter
{
	mode_t	type;
	char	letter;
};

/*
** flags.c
*/

t_flags				flags_extract(int argc, char **argv);


bool				entry_push(char *filename, struct stat *statbuf, t_ftdstr *out, t_flags flags, unsigned int padding);
bool				entries_push(t_ftvec *filenames, struct stat *stats, t_ftdstr *out, t_flags flags);

// void				date_init(const time_t *timep, t_date *date);
char				*date_str(const time_t *timep);

/*
** order.c
*/

void				order_filenames(t_ftvec *filenames, struct stat *stats, t_flags flags);

#endif
