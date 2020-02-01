/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/01 21:14:45 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	prompt_line(t_env *env)
{
	char *buffer;

	ft_printf("🔥 \033[0;32m%s\033[0m >> ", env->dir);
	get_next_line(0, &buffer);
	free(buffer);
	if (ft_strncmp("exit", buffer, 5) == 0)
		env->stop = 1;
	if (!env->stop)
		prompt_line(env);
	else
		ft_printf("🖐 \033[0;31mGood bye!\033[0m\n");
	return (SUC);
}
