/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 22:51:07 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int
	handle_separator(t_shell *sh, t_read *rd, int *i)
{
	t_operator op;

	op = get_operator(sh, i);
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer, rd);
	if (op == AND || op == OR)
		*i = *i + 1;
	if (op >= NONE)
	{
		exec_lines(sh);
		ft_lstclear(&sh->cmds, free_command);
	}
	new_command(sh, op);
	while (sh->input[*i + 1] == ' ')
		*i = *i + 1;
	if (is_cmd_separator(sh->input[*i + 1]) == 2)
		return (ft_fprintf(STDERR, MSG_ERROR, "parse error") && 0);
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (1);
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
		if (sh->input[*i + 1] == '<' && ((*i = *i + 1) || 1))
			rd->add_to = HEREDOC;
		else
			rd->add_to = IN_REDIR;
	}
	else if (sh->input[*i] == '>')
	{
		if (sh->input[*i + 1] == '>' && ((*i = *i + 1) || 1))
			rd->add_to = OUT_END_REDIR;
		else
			rd->add_to = OUT_REDIR;
	}
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (SUC);
}
