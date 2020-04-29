/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:21:47 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 13:32:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "line_edit.h"
#include <stdlib.h>

static void	init_index(char *key, char *str, int i[3])
{
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	while (ft_isspace(key[i[0]]))
		++i[0];
	while (ft_isspace(str[i[1]]))
		++i[1];
	while (key[i[0] + i[2]] && key[i[0] + i[2]] == str[i[1] + i[2]])
		++i[2];
}

void		join_alias(char *key, char **alias, char **str)
{
	int		i[3];
	char	*tmp;

	init_index(key, *str, i);
	if ((tmp = ft_strjoin(*alias, " ")))
	{
		free(*alias);
		*alias = tmp;
		if ((tmp = ft_strjoin(*alias, (*str) + i[2])))
		{
			free(*alias);
			*alias = NULL;
			free(*str);
			*str = tmp;
		}
	}
	else
		*str = *alias;
}

void		free_alias_lst(t_alias **alias, char *except, char **str)
{
	t_alias *tmp;

	while (*alias)
	{
		tmp = (*alias)->next;
		if ((*alias)->value != except)
		{
			free((*alias)->value);
			(*alias)->value = NULL;
		}
		else
		{
			join_alias((*alias)->key, &((*alias)->value), str);
		}
		free((*alias)->key);
		(*alias)->key = NULL;
		free(*alias);
		*alias = tmp;
	}
}
