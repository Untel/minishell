/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:50 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/24 18:44:45 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void
	print_history(t_shell *sh, t_term *term, int next)
{
	t_bilist		*idx;
	char			*str;

	if (!sh->history.input)
		sh->history.input = get_current_word(sh);
	str = sh->history.input;
	// if (sh->history.index)
	// 	sh->history.index = (next ? sh->history.index->next : sh->history.index->prev);
	if (!sh->history.index)
		sh->history.index = sh->history.elements;
	idx = next ? sh->history.index->next : sh->history.index->prev;
	if (!idx)
		idx = next ? sh->history.elements : (t_bilist *)ft_lstlast((t_list *)sh->history.elements);
	while (idx != sh->history.index)
	{
		if (match(idx->content, str))
		{
			sh->history.index = idx;
			add_str_to_lst(sh, str, idx->content);
			return ;
		}
		idx = next ? idx->next : idx->prev;
		if (!idx)
			idx = next ? sh->history.elements : (t_bilist *)ft_lstlast((t_list *)sh->history.elements);
	}
	add_str_to_lst(sh, str, "");
}

void
	handle_arrows(char buff[3], t_term *term)
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
	if (buff[1] == '[' && buff[2] == 'A')
		print_history(&g_sh, term, 0);
	else if (buff[1] == '[' && buff[2] == 'B')
		print_history(&g_sh, term, 1);
}

void
	handle_backspace(char buff[3], t_term *term)
{
	if (term->input->size > 1 && term->pos_str < term->input->size)
		delone(term->input, term->pos_str + 1);
	write(1, "\e[D", 3);
	write(1, buff, 1);
	write(1, "\e[D", 3);
}

int
	handle_ctrl_d(char buff[3], t_term *term)
{
	if (term->input->size == 1)
	{
		free_all(term->input);
		ft_memdel((void **)&term->input);
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

void
	handle_ctrl_u(t_term term)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i <= term.input->size - term.pos_str)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < term.input->size)
		write(1, &del, 1);
	while (term.input->size > 1 && term.pos_str < term.input->size)
		delone(term.input, term.pos_str + 1);
}

void
	handle_ctrl_c(t_term *term)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i <= term->input->size - term->pos_str)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < term->input->size)
		write(1, &del, 1);
	free_all(term->input);
	term->pos_str = 1;
	add_empty(term->input, 0);
}
