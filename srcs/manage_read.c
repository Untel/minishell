/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:09 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/12 07:19:32 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

extern int	g_termx;
extern int	g_termy;

void	print_line(t_shell *sh, int *match)
{
	int			i;
	int			del;
	int			offset;

	del = 127;
	offset = sh->term.size_prt;
	if (sh->term.pos_str < 0)
	{
		i = -1;
		while (++i <= *match)
		{
			write(1, "\n", 1);
			ft_printf("%*s", g_termx, "");
		}
		i = -1;
		while (++i <= *match)
			write(1, "\e[A", 3);
	}
	sh->term.pos_str *= (sh->term.pos_str < 0) ? -1 : 1;
	write(1, "\r", 1);
	while (--offset >= 0)
		write(1, "\e[C", 3);
	affiche_inv(sh->term.input);
	i = -1;
	while (++i <= sh->term.old_s_in + sh->term.input->size - 1)
		write(1, &del, 1);
	i = -1;
	while (++i < sh->term.old_s_in + sh->term.input->size + sh->term.pos_str - 1)
		write(1, "\e[D", 3);
}

char	*read_input(int offset, t_shell *sh)
{
	char	buff[3];
	int		ret;
	int		match;

	sh->term.input = malloc(sizeof(t_data));
	init_lst(sh->term.input);
	add_empty(sh->term.input,'\0');
	match = 0;
	sh->term.pos_str = 1;
	while (match || (ret = read(0, buff, 1)))
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
			match = print_match(sh, buff);
			continue ;
		}
		else if (sh->term.input->size < g_termx  - sh->term.size_prt)
			add_after(sh->term.input, buff[0], sh->term.pos_str);
		sh->term.pos_str *= (buff[0] != 9) ? -1 : 1;
		print_line(sh, &match);
		match = 0;
	}
}
