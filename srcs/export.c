/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 17:28:35 by adda-sil         ###   ########.fr       */
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
