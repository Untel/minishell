/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:35:51 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 22:09:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ask_closing_quote(t_shell *sh)
{
	char	*tmp;
	char	*buffer;

	ft_printf("Quote? > ");
	get_next_line(0, &buffer);
	tmp = ft_strjoin(sh->input, buffer);
	free(buffer);
	free(sh->input);
	sh->input = tmp;
	return (sanitize_input(sh));
}

int
	sanitize_input(t_shell *sh)
{
	t_reader	rd;
	int			i;
	int			idx;
	char		*sub;

	rd = (t_reader) { .simple_q = 0, .double_q = 0 };
	i = -1;
	idx = 0;
	new_command(sh);
	while (sh->input[++i])
	{
		if (sh->input[i] == '\\' && !rd.simple_q)
			i++;
		else if (sh->input[i] == '"' && !rd.simple_q)
			rd.double_q = !rd.double_q;
		else if (sh->input[i] == '\'' && !rd.double_q)
			rd.simple_q = !rd.simple_q;
		else if (sh->input[i] == '"' || sh->input[i] == '\'' ||
			(!rd.double_q && !rd.simple_q && sh->input[i] == ' '))
		{
			sub = ft_substr(&(sh->input[idx]), 0, i - idx);
			add_arg_to_last_cmd(sh, sub);
			idx = i;
		}
	}
	if (rd.simple_q || rd.double_q)
		return ask_closing_quote(sh);
	sub = ft_substr(&(sh->input[idx]), 0, i - idx);
	add_arg_to_last_cmd(sh, sub);
	return (SUC);
}