/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/01 21:57:06 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	interpreter(t_env *env, char *typed)
{
	char	**quotes;
	int		quotes_count;
	(void)env;

	quotes_count = 0;
	quotes = ft_split(typed, '"');
	while (quotes[quotes_count])
	{
		printf("==> '%s'\n", quotes[quotes_count++]);
	}
	if (quotes_count % 2)
	
	return (SUC);
}

int
	prompt_line(t_env *env)
{
	char *buffer;

	ft_printf("🔥  \033[0;32m%s\033[0m >> ", env->dir);
	get_next_line(0, &buffer);
	interpreter(env, buffer);
	if (ft_strncmp("exit", buffer, 5) == 0)
		env->stop = 1;
	free(buffer);
	if (env->stop)
		ft_printf("🖐  \033[0;31mGood bye!\033[0m\n");
	else
		prompt_line(env);
	return (SUC);
}
