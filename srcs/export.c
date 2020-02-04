/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 20:18:23 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	export_env(t_shell *sh, t_cmd *cmd)
{
	char	*pos;

	if (cmd->argc > 1)
	{
		pos = ft_strchr(cmd->argv[1], '=');
		if (!pos)
			return (0);
		*pos++ = 0;
		if (!set_value(&sh->env, cmd->argv[1], pos))
			ft_printf(MSG_ERROR, "export: not an identifier: 8ab");
	}
	else
		return (ft_env(sh->env, 1));
	return (0);
}

int
	unset_env(t_shell *sh, t_cmd *cmd)
{
	int i;

	if (cmd->argc <= 1)
	{
		ft_printf(MSG_ERROR, "unset: not enough arguments");
		return (0);
	}
	i = 0;
	while (++i < cmd->argc)
		unset_key(&sh->env, cmd->argv[i]);
	return (0);
}