/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 21:40:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/15 22:19:34 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		add_alias_lst(t_shell *sh,
		t_alias **alias, char *key, char *value)
{
	while (*alias)
		alias = &((*alias)->next);
	if (!(*alias = malloc(sizeof(t_alias))))
		return ;
	(*alias)->key = key;
	(*alias)->value = replace_tilde(sh, value);
	(*alias)->value = replace_vars(sh, (*alias)->value);
	(*alias)->next = NULL;
}

static void		ignore_space(char *str, int end)
{
	int		i;

	i = 0;
	while (*(str + i) && ft_isspace(*(str + i)))
		++i;
	if (i > 0 && *(str + i))
		ft_strcpy(str, str + i);
	if (!end)
	{
		i = 0;
		while (*(str + i) && !ft_isspace(*(str + i)))
			++i;
		*(str + i) = 0;
	}
	else
	{
		i = ft_strlen(str);
		while (--i >= 0 && ft_isspace(*(str + i)))
			*(str + i) = 0;
	}
}

static void		parse_line(t_shell *sh, char **line, t_alias **alias)
{
	char	**strs;
	int		size;
	int		i;

	strs = ft_split((const char *)*line, ':');
	size = 0;
	while (strs && strs[size])
		++size;
	if (size == 2)
	{
		ignore_space(strs[0], 0);
		ignore_space(strs[1], 1);
	}
	if (size != 2 || !ft_strlen(strs[0]) || !ft_strlen(strs[1]))
	{
		i = 0;
		while (*(strs + i))
			free(*(strs + i++));
		free(strs);
		return ;
	}
	add_alias_lst(sh, alias, strs[0], strs[1]);
	free(strs);
}

t_alias			*load_alias(t_shell *sh)
{
	char	*filename;
	int		fd;
	char	*line;
	int		ret;
	t_alias	*tmp;

	filename = ".alias";
	tmp = NULL;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (NULL);
	while (1)
	{
		ret = get_next_line(fd, &line);
		parse_line(sh, &line, &tmp);
		if (ret >= 0)
			free(line);
		line = NULL;
		if (!(ret > 0))
			break ;
	}
	return (tmp);
}
