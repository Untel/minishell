/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/21 16:38:32 by adda-sil         ###   ########.fr       */
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

int
	handle_double_quote(t_shell *sh, t_read *rd, int *i)
{
	char	*tmp;
	char	*sub;
	char	*vars;
	int		len;

	if (!(tmp = ft_strchr_escape(sh->input + *i + 1, '"', '\\')))
		return (0);
	copy_from_idx(sh, rd, *i);
	len = tmp - (sh->input + *i + 1);
	sub = ft_substr(sh->input + *i + 1, 0, len);
	ft_escape(sub, '\\');
	sub = replace_vars(sh, sub);
	if (rd->buffer)
	{
		tmp = ft_strjoin(rd->buffer, sub);
		ft_memdel((void **)&sub);
		sub = tmp;
	}
	ft_memdel((void **)&rd->buffer);
	rd->buffer = sub;
	*i = *i + len + 1;
	rd->index = *i + 1;
	return (1);
}

int
	handle_simple_quote(t_shell *sh, t_read *rd, int *i)
{
	char	*tmp;
	char	*sub;
	int		len;

	if (!(tmp = ft_strchr(sh->input + *i + 1, '\'')))
		return (0);
	copy_from_idx(sh, rd, *i);
	len = tmp - (sh->input + *i + 1);
	sub = ft_substr(sh->input + *i + 1, 0, len);
	if (rd->buffer)
	{
		tmp = ft_strjoin(rd->buffer, sub);
		ft_memdel((void **)&sub);
		sub = tmp;
	}
	ft_memdel((void **)&rd->buffer);
	rd->buffer = sub;
	*i = *i + len + 1;
	rd->index = *i + 1;
	return (1);
}

int
	handle_space(t_shell *sh, t_read *rd, int *i)
{
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer, rd);
	while (sh->input[*i + 1] == ' ')
		*i = *i + 1;
	rd->index = *i + 1;
	rd->buffer = NULL;
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
	handle_separator(t_shell *sh, t_read *rd, int *i)
{
	t_operator op;

	op = get_operator(sh, i);
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer, rd);
	if (op == AND || op == OR || op == REDIR_OUT_END)
		*i = *i + 1;
	if (op >= NONE)
	{
		exec_lines(sh);
		ft_lstclear(&sh->cmds, free_command);
	}
	new_command(sh, op);
	while (sh->input[*i] == ' ')
		*i = *i + 1;
	if (is_cmd_separator(sh->input[*i + 1]) == 2)
		return (ft_fprintf(STDERR, MSG_ERROR, "parse error") && 0);
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (1);
}

int
	get_heredoc(t_shell *sh, t_read *rd, int *i)
{
	int		j = 0;
	char	*label;
	while (sh->input[*i + 1] && sh->input[*i + 1] == ' ')
		*i++;
	while (sh->input[*i + 1 + j] && sh->input[*i + 1 + j] != ' ')
		j++;
	if (j > 0)
	{
		label = ft_substr(&sh->input[*i + 1], 0, j);
		*i += j;
	}
}

int
	handle_redirections(t_shell *sh, t_read *rd, int *i)
{
	int		j;
	char	*str;

	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer, rd);
	if (sh->input[*i] == '<')
	{
		if (sh->input[*i + 1] == '<' && (*i++ || 1))
			rd->add_to = HEREDOC;
		else
			rd->add_to = IN_REDIR;
	}
	else if (sh->input[*i] == '>')
	{
		if (sh->input[*i + 1] == '>' && (*i++ || 1))
			rd->add_to = OUT_REDIR;
		else
			rd->add_to = OUT_END_REDIR;
	}
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (SUC);
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
		if (c == '\\')
		{
			++i;
			continue;
		}
		else if (c == '\'')
		{
			handle_simple_quote(sh, &rd, &i);
		}
		else if (c == '"')
		{
			handle_double_quote(sh, &rd, &i);
		}
		// else if (c == '(' || c == ')')
		// {
		// 	if (!handle_double_quote(sh, &rd, &i))
		// 		return (ask_closing_char(sh, &rd, "subsh"));
		// }
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
