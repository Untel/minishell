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

char
	*handle_closing_char(t_env *env, char *typed, char sep)
{
	char	*buffer;
	char	*tmp;
	int		quotes_count;
	(void)env;

	quotes_count = 0;
	tmp = typed;
	while ((tmp = ft_strchr(tmp, sep)))
		++quotes_count && ++tmp;
	if (quotes_count > 0 && quotes_count % 2)
	{
		ft_printf("Quote? > ");
		get_next_line(0, &buffer);
		tmp = ft_strjoin(typed, buffer);
		free(buffer);
		free(typed);
		return handle_closing_char(env, tmp, sep);
	}
	return (typed);
}

int
	prompt_line(t_env *env)
{
	char *buffer;

	ft_printf("🔥  \033[0;32m%s\033[0m >> ", env->dir);
	get_next_line(0, &buffer);
	buffer = handle_closing_char(env, buffer, '"');
	buffer = handle_closing_char(env, buffer, '\'');
	if (ft_strncmp("exit", buffer, 5) == 0)
		env->stop = 1;
	if (env->stop)
		ft_printf("🖐  \033[0;31mGood bye!\033[0m\n");
	else
		prompt_line(env);
	return (SUC);
}
