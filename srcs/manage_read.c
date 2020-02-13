/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:09 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/13 07:15:43 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

extern int	g_termx;
extern int	g_termy;

void	print_line(t_shell *sh)
{
	int			i;
	int			del;
	int			offset;

	del = 127;
	offset = sh->term.size_prt;
	write(1, "\r", 1);
	while (--offset >= 0)
		write(1, "\e[C", 3);
	affiche_inv(sh->term.input);
	ft_printf("%*c", sh->term.tmp + 1, 127);
	i = -1;
	while (++i < sh->term.pos_str + sh->term.tmp)
		write(1, "\e[D", 3);
//	write(1, "\r", 1);
//	while (--offset + sh->term.input->size > 0)
//		write(1, "\e[C", 3);
//	write(1, &del, 1);
//	write(1, &del, 1);
//	i = -1;
//	while (++i < sh->term.pos_str + 1)
//		write(1, "\e[D", 3);
}

char	*handle_input(t_shell *sh, int *match, char buff[3])
{
	if (buff[0] == 27)
		handle_arrows(buff, &sh->term);
	else if (buff[0] == 21)
		handle_ctrl_u(sh->term);
	else if (buff[0] == 127)
		handle_backspace(buff, &sh->term);
	else if (buff[0] == 4)
	{
		if (handle_ctrl_d(buff, &sh->term) == -1)
			return ((char *)-1);
	}
	else if (buff[0] == 10)
		return (convert_to_str(sh->term.input));
	else if (buff[0] == 9)
	{
		*match = print_match(sh, buff);
		return ((char *)1);
	}
	else if (sh->term.input->size < g_termx - sh->term.size_prt)
		add_after(sh->term.input, buff[0], sh->term.pos_str);
	return ((char *)2);
}

char	*read_input(t_shell *sh)
{
	char	buff[3];
	char	*ret;
	int		match;

	sh->term.input = malloc(sizeof(t_data));
	init_lst(sh->term.input);
	add_empty(sh->term.input, '\0');
	match = 0;
	sh->term.pos_str = 1;
	sh->term.tmp = 0;
	while (match || read(0, buff, 1) > 0)
	{
		ret = handle_input(sh, &match, buff);
		if (ret == (char *)1)
			continue ;
		if (ret == (char *)-1)
			return ((char *)-1);
		if (ret != (char *)2)
			return (ret);
		print_line(sh);
		match = 0;
	}
	return ((char *)-1);
}
