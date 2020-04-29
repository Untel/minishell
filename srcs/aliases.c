/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:11:55 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 19:04:06 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "line_edit.h"
#include "libft.h"
#include "get_next_line.h"
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

static void	init_index(char *key, char *str, int i[4])
{
	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	i[3] = 0;
	while (key[i[0]] && ft_isspace(key[i[0]]))
		++i[0];
	while (str[i[1]] && ft_isspace(str[i[1]]))
		++i[1];
	while (str[i[1] + i[3]] && !ft_isspace(str[i[1] + i[3]]))
		++i[3];
	while (key[i[0] + i[2]] && key[i[0] + i[2]] == str[i[1] + i[2]])
		++i[2];
}

char			*replace_aliases(char *str, t_alias	*head)
{
	t_alias	*tmp;
	int		s_key;
	int		s_str;
	int		i[4];

	tmp = head;
	while (tmp)
	{
		init_index(tmp->key, str, i);
		s_key = ft_strlen(tmp->key + i[0]);
		s_str = ft_strlen(str + i[1]);
		if (!ft_strncmp(str + i[1], tmp->key + i[0], i[3]))
			join_alias(ft_strdup(tmp->value), &str, i);
		tmp = tmp->next;
	}
	return (str);
}

char			*check_aliases(char *str)
{
	t_alias	*head;
	char	**cmds;
	char	*tmp;
	char	*sep;
	int		i;
	char	c[2];

	if (!str || str >= (char *)-3)
		return (str);
	head = load_alias();
	i = -1;
	sep = NULL;
	c[1] = 0;
	while (*(str + ++i))
	{
		if (!is_cmd_separator(*(str + i)))
			continue ;
		c[0] = *(str + i);
		tmp = ft_strjoin(sep, c);
		if (sep)
			free(sep);
		sep = tmp;
	}
	if ((cmds = ft_split_chst(str, ";|&")))
	{
		i = -1;
		while (cmds[++i])
		{
			cmds[i] = replace_aliases(cmds[i], head);
			if (sep && i < (int)ft_strlen(sep))
			{
				c[0] = sep[i];
				tmp = ft_strjoin(cmds[i], c);
				free(cmds[i]);
				cmds[i] = tmp;
			}
		}
		tmp = ft_strsjoin(i, cmds, " ");
		i = -1;
		while (cmds[++i])
			free(cmds[i]);
		free(cmds);
		free(str);
		str = tmp;
	}
	free(sep);
	free_alias_lst(&head, NULL, NULL);
	return (str);
}
