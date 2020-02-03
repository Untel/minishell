/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 19:11:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/03 21:01:23 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "minishell.h"

char	**convert_env_list(t_list *lst_env)
{
	char	**envp;
	char	*tmp;
	int		size;
	t_key	*var;

	size = ft_lstsize(lst_env);
	if (!(envp = malloc(sizeof(char*) * (size + 1))))
		return (NULL);
	envp[size] = 0;
	while (--size >= 0)
	{
		var = (t_key *)(lst_env->content);
		envp[size] = ft_strjoin(var->key, "=");
		tmp = ft_strjoin(envp[size], var->value);
		free(envp[size]);
		envp[size] = tmp;
		lst_env = lst_env->next;
	}
	return (envp);
}

t_list	*create_env_list(char **envp)
{
	t_list	*lst_env;
	t_key	*var;
	int		split_pos;
	int		i;

	i = -1;
	while (*(envp + ++i))
	{
		split_pos = ft_strchr(envp[i], '=') - envp[i];
		if (!(var = malloc(sizeof(*var))))
			return (NULL);
		var->key = ft_substr(envp[i], 0, split_pos);
		var->value = ft_substr(envp[i], split_pos + 1, ft_strlen(envp[i] +
					split_pos));
		if (i == 0)
			lst_env = ft_lstnew(var, sizeof(var));
		else
			ft_lstadd_front(&lst_env, ft_lstnew(var, sizeof(var)));
	}
	return (lst_env);
}

void	*set_value(t_list **lst_env, char *key, char *value)
{
	t_key	*var;
	int		found;

	found = 0;
	while (*lst_env)
	{
		if ((*lst_env)->next && (var = (t_key *)(*lst_env)->next->content))
			if (!strcmp(var->key, key) && (found = 1))
				break ;
		lst_env = &(*lst_env)->next;
	}
	if (!found)
	{
		if (!(var = malloc(sizeof(*var))))
			return (NULL);
		var->key = ft_strdup(key);
		var->value = ft_strdup(value);
		ft_lstadd_front(lst_env, ft_lstnew(var, sizeof(var)));
		return (*lst_env);
	}
	free(var->value);
	var->value = ft_strdup(value);
	return (*lst_env);
}

void	*get_value(t_list *lst_env, char *key)
{
	int		found;

	found = 0;
	while (lst_env)
	{
		if (!strcmp(((t_key *)(*lst_env).content)->key, key) && (found = 1))
			break ;
		lst_env = lst_env->next;
	}
	if (found == 1)
		return (((t_key *)(*lst_env).content)->value);
	return (NULL);
}

void	print_lst_env(t_list *lst_env)
{
	t_key *key;

	while (lst_env)
	{
		key = (t_key *)(lst_env->content);
		ft_printf("\e[31;1m%d\e[0m \"%s=%s\"\n", lst_env->size,
				key->key, key->value);
		lst_env = lst_env->next;
	}
}
