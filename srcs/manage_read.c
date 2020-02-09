/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:09 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 09:23:14 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

extern int	g_termx;
extern int	g_termy;

void	print_line(t_data *lst, int pos, int offset, int *match)
{
	int			i;
	int			del;

	del = 127;
	i = -1;
	if (pos < 0)
		ft_printf("\n%*s\e[A", *match - 1, " ");
	pos *= (pos < 0) ? -1 : 1;
	write(1, "\r", 1);
	while (--offset >= 0)
		write(1, "\e[C", 3);
	affiche_inv(lst);
	write(1, &del , 1);
	i = -1;
	while (++i < pos)
		write(1, "\e[D", 3);
}

char	*read_input(int offset, t_shell *sh)
{
	char	buff[3];
	int		ret;
	t_data *lst;
	int		pos;
	int		match;

	lst = malloc(sizeof(t_data));
	init_lst(lst);
	add_empty(lst, '\0');
	pos = 1;
	match = 0;
	while ((ret = read(0, buff, 1)))
	{
		if (buff[0] == 27)
			handle_arrows(buff, &pos, lst);
		else if (buff[0] == 21)
			handle_ctrl_u(lst, pos);
		else if (buff[0] == 127)
			handle_backspace(buff, &pos, lst);
		else if (buff[0] == 4)
		{
			if (handle_ctrl_d(buff, &pos, lst) == -1)
				return ((char *)-1);
		}
		else if (buff[0] == 10)
			return (convert_to_str(lst));
		else if (buff[0] == 9)
			match = print_match(sh, lst, pos);
		else if (lst->size < g_termx)
			add_after(lst, buff[0], pos);
		print_line(lst, pos * ((buff[0] != 9) ? -1 : 1), offset, &match);
	}
}
