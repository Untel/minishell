/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/09 18:23:03 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	export_env_noargs(t_list *lst_env)
{
	t_key	*key;

	while (lst_env && (key = (t_key *)(lst_env->content)))
	{
		ft_printf("declare -x %s", key->key);
		if (key->value)
			ft_printf("=\"%s\"\n", key->value);
		else
			write(STDOUT_FILENO, "\n", 1);
		lst_env = lst_env->next;
	}
}

int
	export_env(t_shell *sh, t_cmd *cmd)
{
	char	*pos;
	int		i;

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
	else
		export_env_noargs(sh->env);
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
