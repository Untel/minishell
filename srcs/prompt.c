/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 18:41:43 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	print_commands(t_shell *sh)
{
	int		i;
	int		j;
	t_list	*lst;
	t_cmd	*cmd;

	lst = sh->cmds;
	j = 0;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		printf("Command %d => %d arguments\n", j, cmd->argc);
		i = -1;
		while (++i < cmd->argc)
			printf("%d => %s\n", i, cmd->argv[i]);
		lst = lst->next;
		j++;
	}
	return (SUC);
}

int
	ft_read(t_shell *sh)
{
	sh->term.size_prt = ft_strlen(sh->printed_dir) + 7;
	sh->input = read_input(sh);
	write(1, "\n", 1);
	return ((sh->input == (char *)-1) ? 0 : 1);
}

int
	prompt_line(t_shell *sh)
{
	ft_printf("\e[7m%%\e[0m%*s\r", g_sh.term.pos.x - 1, "");
	ft_printf(sh->last_ret == EXIT_SUCCESS ?
		MSG_PROMPT : MSG_PROMPT_ERR, sh->printed_dir);
	if (ft_read(sh) == 0)
		sh->stop = 1;
	if (!sh->stop && sanitize(sh))
	{
		add_to_history(sh);
		parse_input(sh);
		clear_last_prompt(sh);
	}
	if (sh->stop)
		ft_printf(MSG_EXIT);
	else
		prompt_line(sh);
	return (SUC);
}
