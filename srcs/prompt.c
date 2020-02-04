/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 14:34:34 by adda-sil         ###   ########.fr       */
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
	prompt_line(t_shell *sh)
{
	ft_printf(MSG_PROMPT, sh->printed_dir);
	get_next_line(0, &(sh->input));
	sanitize_input2(sh);
	// print_commands(sh);
	exec_lines(sh);
	clear_last_prompt(sh);
	if (sh->stop)
		ft_printf(MSG_EXIT);
	else
		prompt_line(sh);
	return (SUC);
}
