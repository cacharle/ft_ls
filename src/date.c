/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: charles <charles.cabergs@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/17 16:31:42 by charles           #+#    #+#             */
/*   Updated: 2020/05/17 17:02:42 by charles          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/* static char	*g_month_strs[] = { */
/* 	"Jan", */
/* 	"Feb", */
/* 	"Mar", */
/* 	"Apr", */
/* 	"May", */
/* 	"Jun", */
/* 	"Jul", */
/* 	"Aug", */
/* 	"Sep", */
/* 	"Oct", */
/* 	"Nov", */
/* 	"Dec", */
/* }; */
/*  */
/* void	date_init(const time_t *timep, t_date *date) */
/* { */
/* 	char	*date_str; */
/*  */
/* 	date_str = ctime(timep); */
/* 	date_str = ft_strchr(date_str, ' ') + 1; */
	/* date->month_str = ft_strndup(date_str, 3);//g_month_strs[ft_atoi(date_str)]; */
	/* date_str = ft_strchr(date_str, ' ') + 1; */
	/* date->day = ft_atoi(date_str); */
	/* date_str = ft_strchr(date_str, ' ') + 1; */
	/* date->hour = ft_atoi(date_str); */
	/* date_str = ft_strchr(date_str, ':') + 1; */
	/* date->min = ft_atoi(date_str); */
/* } */

char	*date_str(const time_t *timep)
{
	char	*date_str;

	date_str = ctime(timep);
	date_str = ft_strchr(date_str, ' ') + 1;
	if ((date_str = ft_strdup(date_str)) == NULL)
		return (NULL);
	*ft_strrchr(date_str, ':') = '\0';
	return (date_str);
}
