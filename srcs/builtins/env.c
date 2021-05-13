/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 01:53:59 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 02:10:44 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_env(t_shell *sh, t_cmd *cmd)
{
	t_key	*key;
	t_list	*lst_env;

	lst_env = sh->env;
	if (cmd->argc > 1)
	{
		ft_fprintf(STDERR, MSG_ERROR, "env: to many arguments");
		return (EXIT_FAILURE);
	}
	while (lst_env)
	{
		key = (t_key *)(lst_env->content);
		if (key->value)
			ft_printf("%s=%s\n", key->key, key->value);
		lst_env = lst_env->next;
	}
	return (EXIT_SUCCESS);
}
