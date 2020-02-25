/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 18:12:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	is_cmd_separator(char c)
{
	if (c == ';')
		return (1);
	else if (c == '|' || c == '&')
		return (2);
	else
		return (0);
}

char
	*replace_vars(t_shell *sh, char *str)
{
	char	*ptr;
	int		key_len;
	char	*tmp;
	char	*key;
	char	*value;

	if ((ptr = ft_strchr(str, '$')))
	{
		tmp = ft_strndup(str, ptr - str);
		key_len = 1;
		if (*(ptr + 1) == '?')
		{
			value = ft_itoa(sh->last_ret);
			key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
			ft_memdel((void **)&value);
		}
		else
		{
			while (ptr[key_len] && !ft_strchr(" /$", ptr[key_len]))
				++key_len;
			key = ft_strndup(ptr + 1, key_len);
			value = get_value(sh->env, key, NULL);
			ft_memdel((void **)&key);
			key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
		}
		value = ft_strjoin(key, ptr + 1 + key_len);
		ft_memdel((void **)&tmp);
		ft_memdel((void **)&key);
		ft_memdel((void **)&str);
		return (replace_vars(sh, value));
	}
	return (str);
}

int
	copy_from_idx(t_shell *sh, t_read *rd, int idx)
{
	char *sub;
	char *tmp;

	sub = ft_substr(sh->input + rd->index, 0, idx - rd->index);
	sub = replace_vars(sh, sub);
	if (rd->buffer)
	{
		tmp = ft_strjoin(rd->buffer, sub);
		ft_memdel((void **)&sub);
		sub = tmp;
	}
	ft_memdel((void **)&rd->buffer);
	rd->buffer = sub;
	rd->index = idx + 1;
	return (1);
}

t_operator
	get_operator(t_shell *sh, int *i)
{
	if (sh->input[*i] == '&')
		return (sh->input[*i + 1] == '&' ? AND : JOB);
	else if (sh->input[*i] == '|')
		return (sh->input[*i + 1] == '|' ? OR : PIPE);
	else
		return (NONE);
}

int
	parse_input(t_shell *sh)
{
	int			i;
	char		c;
	t_read		rd;
	int			ret;

	i = -1;
	ret = 1;
	rd = (t_read) { .buffer = NULL, .index = 0,
		.add_to = ARGS, .input = NULL,
	};
	new_command(sh, NONE);
	while ((c = sh->input[++i]))
	{
		if (c == '\\' && (++i || 1))
			continue;
		else if (c == '\'')
			handle_simple_quote(sh, &rd, &i);
		else if (c == '"')
			handle_double_quote(sh, &rd, &i);
		else if (c == ' ')
			ret = handle_space(sh, &rd, &i);
		else if (is_cmd_separator(c))
			ret = handle_separator(sh, &rd, &i);
		else if (c == '<' || c == '>')
			ret = handle_redirections(sh, &rd, &i);
		if (!ret)
			return (ret);
	}
	ret = handle_separator(sh, &rd, &i);
	return (ret);
}
