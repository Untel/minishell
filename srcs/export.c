/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/21 16:00:57 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	export_env(t_shell *sh, t_cmd *cmd)
{
	char	*pos;
	char	msg[BUFFER_SIZE];

	if (cmd->argc > 1)
	{
		pos = ft_strchr(cmd->argv[1], '=');
		if (!pos)
			return (EXIT_SUCCESS);
		*pos++ = 0;
		if (!set_value(&sh->env, cmd->argv[1], pos))
		{
			ft_fprintf(STDERR, MSG_ERRORN, "export: not an identifier: ");
			write(STDERR, cmd->argv[1], ft_strlen(cmd->argv[1]));
			write(STDERR, "\n", 1);
			return (EXIT_FAILURE);
		}
	}
	else
		return (ft_env(sh->env, 1));
	return (EXIT_SUCCESS);
}

int
	unset_env(t_shell *sh, t_cmd *cmd)
{
	int i;

	if (cmd->argc <= 1)
	{
		ft_fprintf(STDERR, MSG_ERROR, "unset: not enough arguments");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (++i < cmd->argc)
		unset_key(&sh->env, cmd->argv[i]);
	return (EXIT_SUCCESS);
}
