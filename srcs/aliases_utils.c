/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:21:47 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 17:27:58 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "line_edit.h"
#include <stdlib.h>

static int		ft_strslen(int size, char **strs, char *sep)
{
	int		i;
	int		j;
	int		k;

	if (!size)
		return (0);
	i = -1;
	k = 0;
	while (sep[++k])
		;
	k *= size - 1;
	i = -1;
	while (++i < size)
	{
		j = -1;
		while (strs[i][++j])
			++k;
	}
	return (k);
}

char	*ft_strsjoin(int size, char **strs, char *sep)
{
	char	*str;
	int		i[3];
	int		size_str;

	size_str = ft_strslen(size, strs, sep);
	if (!(str = malloc(sizeof(*str) * (size_str + 1))))
		return (NULL);
	i[0] = -1;
	i[1] = -1;
	while (++i[0] < size)
	{
		i[2] = -1;
		while (strs[i[0]][++i[2]])
			str[++i[1]] = strs[i[0]][i[2]];
		if (i[0] < size - 1)
		{
			i[2] = -1;
			while (sep[++i[2]])
				str[++i[1]] = sep[i[2]];
		}
	}
	str[size_str] = 0;
	return (str);
}

void		join_alias(char *alias, char **str, int i[3])
{
	char	*tmp;

	if ((tmp = ft_strjoin(alias, " ")))
	{
		free(alias);
		alias = tmp;
		if ((tmp = ft_strjoin(alias, (*str) + i[2] + i[1])))
		{
			free(alias);
			alias = NULL;
			free(*str);
			*str = tmp;
		}
	}
	else
		*str = alias;
}

void		free_alias_lst(t_alias **alias, char *except, char **str)
{
	t_alias *tmp;

	(void)except;
	(void)str;
	while (*alias)
	{
		tmp = (*alias)->next;
		free((*alias)->value);
		(*alias)->value = NULL;
		free((*alias)->key);
		(*alias)->key = NULL;
		free(*alias);
		*alias = tmp;
	}
}
