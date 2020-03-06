/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/06 01:12:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	export_env(t_shell *sh, t_cmd *cmd)
{
	char	*pos;
	int		i;
	t_key	*key;
	t_list	*lst_env;

	i = 0;
	if (cmd->argc > 1)
		while (++i < cmd->argc)
		{
			pos = ft_strchr(cmd->argv[i], '=');
			if (pos)
				*pos++ = 0;
			if (!set_value(&sh->env, cmd->argv[i], pos))
			{
				ft_fprintf(STDERR, MSG_ERRORN, "export: not an identifier: ");
				write(STDERR, cmd->argv[i], ft_strlen(cmd->argv[i]));
				write(STDERR, "\n", 1);
				return (EXIT_FAILURE);
			}
		}
	else if ((lst_env = sh->env))
		while (lst_env && (key = (t_key *)(lst_env->content)))
		{
			ft_printf("declare -x %s", key->key);
			if (key->value)
				ft_printf("=\"%s\"\n", key->value);
			else
				write(STDOUT_FILENO, "\n", 1);
			lst_env = lst_env->next;
		}
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
