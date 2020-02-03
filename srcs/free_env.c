/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:41:48 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/03 18:56:31 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_array(char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		free(envp[i]);
	free(envp);
}

void	free_env_var(t_list *lst_env)
{
	t_key	*var;

	var = (t_key *)(*lst_env).content;
	free(var->key);
	free(var->value);
	free(var);
}

void	free_env_list(t_list **lst_env)
{
	ft_lstclear(lst_env, free_env_var);
}
