/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/16 11:59:55 by charles           #+#    #+#             */
/*   Updated: 2020/05/18 13:53:29 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_flags	args_extract_flags(int argc, char **argv)
{
	t_flags	flags;
	int		i;
	char	*match;
	char	*curr;

	flags = 0;
	i = 0;
	while (++i < argc)
		if (argv[i][0] == '-')
		{
			curr = &argv[i][1];
			while (*curr != '\0')
			{
				if ((match = ft_strchr(FLAGS_STRING, *curr)) == NULL || *match == '\0')
				{
					ft_dprintf(STDERR_FILENO, "%s: invalid option -- '%c'", argv[0], *curr);
					return (FLAG_ERROR);
				}
				flags |= 1 << (match - FLAGS_STRING);
				curr++;
			}
			ft_memmove(&argv[i], &argv[i + 1], sizeof(char*) * (argc - i));
			i--;
			argc--;
		}
	argv[argc] = NULL;
	return (flags);
}
