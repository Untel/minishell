/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/09/29 15:42:22 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ft_escape_sequence(sub, '\\', "abdfenrtv", 1);
*/

int
	handle_double_quote(t_shell *sh, t_read *rd, int *i)
{
	char	*tmp;
	char	*sub;
	int		len;

	if (!(tmp = ft_strchr_escape(sh->input + *i + 1, '"', '\\')))
		return (0);
	copy_from_idx(sh, rd, *i);
	len = tmp - (sh->input + *i + 1);
	sub = ft_substr(sh->input + *i + 1, 0, len);
	sub = replace_vars(sh, sub);
	ft_escape_sequence(sub, '\\', "\\\"$", 0);
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
	if (!copy_to_cmd(sh, rd, i))
		return (FALSE);
	while (sh->input[*i] && sh->input[*i + 1] == ' ')
		*i = *i + 1;
	rd->index = *i + 1;
	rd->buffer = NULL;
	return (SUC);
}

int
	handle_separator(t_shell *sh, t_read *rd, int *i)
{
	t_operator op;

	op = get_operator(sh, i);
	if (!copy_to_cmd(sh, rd, i))
		return (FALSE);
	if (rd->add_to != ARGS)
		return (!ft_fprintf(STDERR, MSG_404_REDIR));
	if (op == AND || op == OR)
		*i = *i + 1;
	while (sh->input[*i] && sh->input[*i + 1] == ' ')
		*i = *i + 1;
	if (sh->input[*i] && sh->input[*i + 1] &&
		is_cmd_separator(sh->input[*i + 1]) == 2)
		return (!ft_fprintf(STDERR, SYN_ERR, sh->input[*i + 1], *i + 1));
	rd->buffer = NULL;
	rd->index = *i + 1;
	if (op >= NONE)
		exec_lines(sh);
	new_command(sh, op);
	return (SUC);
}

int
	handle_redirections(t_shell *sh, t_read *rd, int *i)
{
	if (*i > 0 && ft_isdigit(sh->input[*i - 1]))
		rd->fd = ft_rev_atoi_idx(sh->input, *i - 1);
	else if (!copy_to_cmd(sh, rd, i))
		return (FALSE);
	if (sh->input[*i] == '<')
	{
		if (sh->input[*i] && sh->input[*i + 1] == '<' && ((*i = *i + 1) || 1))
			rd->add_to = HEREDOC;
		else
			rd->add_to = IN_REDIR;
	}
	else if (sh->input[*i] == '>')
	{
		if (sh->input[*i] && sh->input[*i + 1] == '>' && ((*i = *i + 1) || 1))
			rd->add_to = OUT_END_REDIR;
		else
			rd->add_to = OUT_REDIR;
	}
	rd->buffer = NULL;
	rd->index = *i + 1;
	return (SUC);
}
