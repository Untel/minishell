/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 19:34:17 by adda-sil         ###   ########.fr       */
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
	char	*sub;
	int		should_cpy;

	should_cpy = 0;
	if (sh->input[i] == '"' && !(rd->simple_q))
	{
		should_cpy = rd->double_q == 1;
		rd->double_q = !(rd->double_q);
	}
	else if (sh->input[i] == '\'' && !(rd->double_q))
	{
		should_cpy = rd->simple_q == 1;
		rd->simple_q = !(rd->simple_q);
	}
	if (should_cpy)
	{
		sub = ft_substr(&(sh->input[rd->idx + 1]), 0, i - rd->idx - 1);
		add_arg_to_last_cmd(sh, sub);
		return (SUC);
	}
	return (0);
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
		if (sh->input[i] == '"' || sh->input[i] == '\'')
		{
			if (handle_quote(sh, &rd, i))
			{
				while (sh->input[i + 1] == ' ')
					i++;
				rd.idx = i + 1;
			}
			continue;
		}
		else if (sh->input[i] == '\\' && !rd.simple_q)
		{
			printf("yo\n");
			i++;
			continue;
		}
		else if (!rd.double_q && !rd.simple_q)
		{
			if(sh->input[i] == ' ')
			{
				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
				while (sh->input[i + 1] == ' ')
					i++;
				rd.idx = i + 1;
			}
			else if (sh->input[i] == ';')
			{
				add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
				new_command(sh);
				while (sh->input[i + 1] == ' ')
					i++;
				rd.idx = i + 1;
			}
		}
	}
	if (rd.simple_q || rd.double_q)
		return ask_closing_quote(sh);
	if (rd.idx != i)
		add_arg_to_last_cmd(sh, ft_substr(&(sh->input[rd.idx]), 0, i - rd.idx));
	return (SUC);
}