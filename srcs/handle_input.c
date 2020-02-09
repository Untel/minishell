/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:50 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 09:30:23 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void	handle_arrows(char buff[3], int *pos, t_data *lst)
{
	read(0, buff + 1, 1);
	read(0, buff + 2, 1);
	if (!(buff[1] == '[' && buff[2] == 'A') &&
			!(buff[1] == '[' && buff[2] == 'B'))
	{
		if (buff[1] == '[' && buff[2] == 'C')
		{
			write(1, buff, 3 * (*pos > 1));
			*pos = (*pos > 1) ? *pos - 1 : *pos;
		}
		else if (buff[1] == '[' && buff[2] == 'D')
		{
			write(1, buff, 3 * (*pos < lst->size));
			*pos = (*pos < lst->size) ? *pos + 1 : *pos;
		}
	}
}

void	handle_backspace(char buff[3], int *pos, t_data *lst)
{
	if (lst->size > 1 && *pos < lst->size)
		delone(lst, *pos + 1);
	write(1, "\e[D", 3);
	write(1, buff, 1);
	write(1, "\e[D", 3);
}

int		handle_ctrl_d(char buff[3], int *pos, t_data *lst)
{
	if (lst->size == 1)
	{
		free_all(lst);
		free(lst);
		return (-1);
	}
	if (*pos > 1)
	{
		delone(lst, *pos);
		*pos -= 1;
	}
	write(1, buff, 1);
	return (1);
}

void	handle_ctrl_u(t_data *lst, int pos)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i < pos)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < lst->size)
		write(1, &del, 1);
	if (lst->size)
		free_all(lst);
	add_empty(lst, '\0');
}
