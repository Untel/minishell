/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 18:41:48 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/19 18:03:52 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_array(char **envp)
{
	int		i;

	i = -1;
	while (envp[++i])
		ft_memdel((void **)&envp[i]);
	ft_memdel((void **)&envp);
}

void	free_env_unset(void *content)
{
	ft_memdel((void **)&((t_key *)content)->key);
	ft_memdel((void **)&((t_key *)content)->value);
	ft_memdel((void **)&content);
}

void	free_env_var(t_list *lst_env)
{
	t_key	*var;

	var = (t_key *)(*lst_env).content;
	ft_memdel((void **)&var->key);
	ft_memdel((void **)&var->value);
	ft_memdel((void **)&var);
}

void	free_env_list(t_list **lst_env)
{
	ft_lstclear(lst_env, free_env_var);
}
