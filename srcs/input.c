/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/05 19:22:16 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ask_closing_char(t_shell *sh, t_read *rd, char *ask)
{
	char	*tmp;
	char	*tmps[2];
	char	*buffer;

	ft_lstclear(&sh->cmds, free_command);
	ft_printf("%s> ", ask);
	get_next_line(0, &buffer);
	tmps[0] = sh->input;
	tmps[1] = buffer;
	tmp = ft_strmjoin(2, tmps, "\n");
	free(buffer);
	clear_last_prompt(sh);
	sh->input = tmp;
	free(rd->buffer);
	rd->buffer = NULL;
	return (sanitize_input2(sh));
}

// int
// 	handle_quote(t_shell *sh, t_reader *rd, int i)
// {
// 	char	*sub;
// 	int		should_cpy;

// 	should_cpy = 0;
// 	if (sh->input[i] == '"' && !(rd->simple_q))
// 	{
// 		should_cpy = rd->double_q == 1;
// 		rd->double_q = !(rd->double_q);
// 	}
// 	else if (sh->input[i] == '\'' && !(rd->double_q))
// 	{
// 		should_cpy = rd->simple_q == 1;
// 		rd->simple_q = !(rd->simple_q);
// 	}
// 	if (should_cpy)
// 	{
// 		sub = ft_substr(&(sh->input[rd->idx + 1]), 0, i - rd->idx - 1);
// 		add_arg_to_last_cmd(sh, sub);
// 		return (SUC);
// 	}
// 	return (0);
// }

// int
// 	sanitize_input(t_shell *sh)
// {
// 	t_reader	rd;
// 	int			i;

// 	rd = (t_reader) { .simple_q = 0, .double_q = 0, .idx = 0 };
// 	i = -1;
// 	new_command(sh);
// 	while (sh->input[++i])
// 	{
// 		if (sh->input[i] == '"' || sh->input[i] == '\'')
// 		{
// 			if (handle_quote(sh, &rd, i))
// 			{
// 				while (sh->input[i + 1] == ' ')
// 					i++;
// 				rd.idx = i + 1;
// 			}
// 			continue;
// 		}
// 		else if (sh->input[i] == '\\' && !rd.simple_q)
// 		{
// 			i++;
// 			continue;
// 		}
// 		else if (!rd.double_q && !rd.simple_q)
// 		{
// 			if(sh->input[i] == ' ')
// 			{
// 				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
// 				while (sh->input[i + 1] == ' ')
// 					i++;
// 				rd.idx = i + 1;
// 			}
// 			else if (sh->input[i] == ';')
// 			{
// 				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
// 				new_command(sh);
// 				while (sh->input[i + 1] == ' ')
// 					i++;
// 				rd.idx = i + 1;
// 			}
// 		}
// 	}
// 	if (rd.simple_q || rd.double_q)
// 		return ask_closing_quote(sh);
// 	if (rd.idx != i)
// 		add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
// 	return (SUC);
// }

char
	*replace_vars(t_shell *sh, char *str)
{
	char	*ptr;
	int		key_len;
	char	*tmp;
	char	*key;
	char	*value;

	if((ptr = ft_strchr(str, '$')))
	{
		tmp = ft_strndup(str, ptr - str);
		ptr++;
		if (*ptr == '?')
		{
			key_len = 1;
			value = ft_itoa(sh->last_ret);
			key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
			free(value);
			value = ft_strjoin(key, ptr + key_len);
		}
		else
		{
			while (ptr[key_len] && !ft_strchr(" /$", ptr[key_len]))
				key_len++;
			key = ft_strndup(ptr, key_len);
			value = get_value(sh->env, key, NULL);
			free(key);
			key = value ? ft_strjoin(tmp, value) : ft_strdup(tmp);
			value = ft_strjoin(key, ptr + key_len);
		}
		free(tmp);
		free(key);
		free(str);
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
		free(sub);
		sub = tmp;
	}
	free(rd->buffer);
	rd->buffer = sub;
	rd->index = idx + 1;
	return (1);
}

int
	handle_double_quote(t_shell *sh, t_read *rd, int *i)
{
	char *tmp;
	char *sub;
	char *vars;
	int	len;

	if (!(tmp = ft_strchr(sh->input + *i + 1, '"')))
		return (0);
	copy_from_idx(sh, rd, *i);
	len = tmp - (sh->input + *i + 1);
	sub = ft_substr(sh->input + *i + 1, 0, len);
	sub = replace_vars(sh, sub);
	if (rd->buffer)
	{
		tmp = ft_strjoin(rd->buffer, sub);
		free(sub);
		sub = tmp;
	}
	free(rd->buffer);
	rd->buffer = sub;
	*i = *i + len + 1;
	rd->index = *i + 1;
	return (1);
}

int
	handle_simple_quote(t_shell *sh, t_read *rd, int *i)
{
	char *tmp;
	char *sub;
	int	len;

	if (!(tmp = ft_strchr(sh->input + *i + 1, '\'')))
		return (0);
	copy_from_idx(sh, rd, *i);
	len = tmp - (sh->input + *i + 1);
	sub = ft_substr(sh->input + *i + 1, 0, len);
	if (rd->buffer)
	{
		tmp = ft_strjoin(rd->buffer, sub);
		free(sub);
		sub = tmp;
	}
	free(rd->buffer);
	rd->buffer = sub;
	*i = *i + len + 1;
	rd->index = *i + 1;
	return (1);
}

int
	handle_space(t_shell *sh, t_read *rd, int *i)
{
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i)
			&& add_arg_to_last_cmd(sh, rd->buffer);
	while (sh->input[*i + 1] == ' ')
		*i = *i + 1;
	rd->index = *i + 1;
	rd->buffer = NULL;
	return (1);
}

int
	handle_separator(t_shell *sh, t_read *rd, int *i)
{
	t_operator op;

	if (sh->input[*i] == '&')
		op = (sh->input[*i + 1] == '&' ? AND : JOB);
	else if (sh->input[*i] == '|')
		op = (sh->input[*i + 1] == '|' ? OR : PIPE);
	else
		op = NONE;
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer);	
	if (op == AND || op == OR)
		*i = *i + 1;
	new_command(sh, op);
	while (sh->input[*i] == ' ')
		*i = *i + 1;
	if (sh->input[*i + 1] == '&' || sh->input[*i + 1] == '|')
		return (ft_printf(MSG_ERROR, "parse error") && 0);
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (1);
}

int
	sanitize_input2(t_shell *sh)
{
	int			i;
	char		c;
	t_read		rd;
	int			ret;

	i = -1;
	ret = 1;
	rd = (t_read) { .buffer = NULL, .index = 0 };
	new_command(sh, NONE);
	while ((c = sh->input[++i]))
	{
		if (c == '\'')
		{
			if (!handle_simple_quote(sh, &rd, &i))
				return (ask_closing_char(sh, &rd, "quote"));
		}
		else if (c == '"')
		{
			if (!handle_double_quote(sh, &rd, &i))
				return (ask_closing_char(sh, &rd, "dquote"));
		}
		else if (c == '(' || c == ')')
		{
			if (!handle_double_quote(sh, &rd, &i))
				return (ask_closing_char(sh, &rd, "subsh"));
		}
		else if (c == ' ')
			ret = handle_space(sh, &rd, &i);
		else if (c == ';' || c == '&' || c == '|')
			ret = handle_separator(sh, &rd, &i);
		if (!ret)
			return (ret);
	}
	if (rd.index != i)
		copy_from_idx(sh, &rd, i);
	if (rd.buffer)
		add_arg_to_last_cmd(sh, rd.buffer);
	return (ret);
}
