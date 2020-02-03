/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 17:06:21 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ask_closing_quote(t_shell *sh)
{
	char	*tmp;
	char	*buffer;

	ft_lstclear(&sh->cmds, free_command);
	ft_printf("Quote? > ");
	get_next_line(0, &buffer);
	tmp = ft_strjoin(sh->input, buffer);
	free(buffer);
	free(sh->input);
	sh->input = tmp;
	return (sanitize_input(sh));
}

int
	handle_quote(t_shell *sh, t_reader *rd, int i)
{
	char *sub;

	if (sh->input[i] == '"' && !rd->simple_q)
		rd->double_q = !rd->double_q;
	else if (sh->input[i] == '\'' && !rd->double_q)
		rd->simple_q = !rd->simple_q;
	else if (sh->input[i] == '"' || sh->input[i] == '\'')
	{
		sub = ft_substr(&(sh->input[rd->idx + 1]), 0, i - rd->idx - 1);
		add_arg_to_last_cmd(sh, sub);
		rd->idx = i + 1;
	}
	return (SUC);
}

int
	sanitize_input(t_shell *sh)
{
	t_reader	rd;
	int			i;

	rd = (t_reader) { .simple_q = 0, .double_q = 0, .idx = 0 };
	i = -1;
	new_command(sh);
	while (sh->input[++i])
	{
		if (sh->input[i] == '\\' && !rd.simple_q)
			i++;
		else if (sh->input[i] == '"' || sh->input[i] == '\'')
			handle_quote(sh, &rd, i);
		if (!rd.double_q && !rd.simple_q)
		{
			if(sh->input[i] == ' ')
			{
				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
				while (sh->input[++i] == ' ')
					;
				rd.idx = i;
			}
			else if (sh->input[i] == ';')
			{
				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
				new_command(sh);
				rd.idx = i + 1;
			}
		}
	}
	if (rd.simple_q || rd.double_q)
		return ask_closing_quote(sh);
	add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
	return (SUC);
}