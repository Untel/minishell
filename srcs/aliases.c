/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:11:55 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 13:44:15 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "libft.h"
#include "get_next_line.h"
#include "ft_printf.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static void		add_alias_lst(t_alias **alias, char *key, char *value)
{
	while (*alias)
		alias = &((*alias)->next);
	if (!(*alias = malloc(sizeof(t_alias))))
		return ;
	(*alias)->key = key;
	(*alias)->value = value;
	(*alias)->next = NULL;
}

static void		parse_line(char **line, t_alias **alias)
{
	char	**strs;
	int		size;
	int		i;

	strs = ft_split((const char *)*line, ':');
	size = 0;
	while (strs && strs[size])
		++size;
	if (size != 2)
	{
		i = 0;
		while (*(strs + i))
			free(*(strs + i++));
		free(strs);
		return ;
	}
	add_alias_lst(alias, strs[0], strs[1]);
	free(strs);
}

static t_alias	*load_alias(void)
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
		parse_line(&line, &tmp);
		if (ret >= 0)
			free(line);
		line = NULL;
		if (!(ret > 0))
			break ;
	}
	return (tmp);
}

char			*check_aliases(char *str)
{
	t_alias	*head;
	t_alias	*tmp;
	int		s_key;
	int		s_str;

	if (str == (char *)-1)
		return ((char *)-1);
	head = load_alias();
	tmp = head;
	while (tmp)
	{
		s_key = ft_strlen(tmp->key);
		s_str = ft_strlen(str);
		if (!ft_strncmp(str, tmp->key, s_key - (s_key - 1 == s_str)))
		{
			free_alias_lst(&head, tmp->value, &str);
			return (str);
		}
		tmp = tmp->next;
	}
	free_alias_lst(&head, NULL, NULL);
	return (str);
}
