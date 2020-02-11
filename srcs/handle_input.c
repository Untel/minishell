/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:50 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/10 19:20:03 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void	handle_arrows(char buff[3], t_term *term)
{
	read(0, buff + 1, 1);
	read(0, buff + 2, 1);
	if (!(buff[1] == '[' && buff[2] == 'A') &&
			!(buff[1] == '[' && buff[2] == 'B'))
	{
		if (buff[1] == '[' && buff[2] == 'C')
		{
			write(1, buff, 3 * (term->pos_str > 1));
			term->pos_str = (term->pos_str > 1) ? term->pos_str - 1 :
				term->pos_str;
		}
		else if (buff[1] == '[' && buff[2] == 'D')
		{
			write(1, buff, 3 * (term->pos_str < term->input->size));
			term->pos_str = (term->pos_str < term->input->size) ? term->pos_str
				+ 1 : term->pos_str;
		}
	}
}

void	handle_backspace(char buff[3], t_term *term)
{
	if (term->input->size > 1 && term->pos_str < term->input->size)
		delone(term->input, term->pos_str + 1);
	write(1, "\e[D", 3);
	write(1, buff, 1);
	write(1, "\e[D", 3);
}

int		handle_ctrl_d(char buff[3], t_term * term)
{
	if (term->input->size == 1)
	{
		free_all(term->input);
		free(term->input);
		return (-1);
	}
	if (term->pos_str > 1)
	{
		delone(term->input, term->pos_str);
		term->pos_str -= 1;
	}
	write(1, buff, 1);
	return (1);
}

void	handle_ctrl_u(t_term term, int sup)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i <= term.input->size - term.pos_str)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < term.input->size)
		write(1, &del, sup ? 1 : 0);
	if (term.input->size)
		free_all(term.input);
	add_empty(term.input, '\0');
}
