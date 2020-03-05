/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/05 15:12:24 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	is_cmd_separator(char c)
{
	if (c == ';')
		return (1);
	if (c == '|' || c == '&')
		return (2);
	else
		return (0);
}

int
	copy_from_idx(t_shell *sh, t_read *rd, int idx)
{
	char *sub;
	char *tmp;

	sub = ft_substr(sh->input + rd->index, 0, idx - rd->index);
	sub = replace_vars(sh, sub);
	sub = replace_tilde(sh, sub);
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
	t_read		rd;
	int			ret;

	ret = 1;
	rd = (t_read) { .buffer = NULL, .index = 0,
		.add_to = ARGS, .input = NULL, .i = -1, .c = 0,
		.fd = -1,
	};
	new_command(sh, NONE);
	while (sh->input[++(rd.i)])
	{
		rd.c = sh->input[rd.i];
		if (rd.c == '\\' && (++rd.i || 1))
			continue;
		else if (rd.c == '\'')
			handle_simple_quote(sh, &rd, &rd.i);
		else if (rd.c == '"')
			handle_double_quote(sh, &rd, &rd.i);
		else if (rd.c == ' ')
			ret = handle_space(sh, &rd, &rd.i);
		else if (is_cmd_separator(rd.c))
			ret = handle_separator(sh, &rd, &rd.i);
		else if (rd.c == '<' || rd.c == '>')
			ret = handle_redirections(sh, &rd, &rd.i);
		if (!ret)
			return (ret);
	}
	ret = handle_separator(sh, &rd, &rd.i);
	return (ret);
}
