/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 16:22:03 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char
	*handle_closing_char(t_shell *sh, char *typed, char sep)
{
	char	*buffer;
	char	*tmp;
	int		quotes_count;
	(void)sh;

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
		return handle_closing_char(sh, tmp, sep);
	}
	return (typed);
}

int
	to_command_list(t_shell *sh)
{
	(void)sh;
	return (SUC);
}

int
	prompt_line(t_shell *sh)
{
	char *buffer;

	getcwd(sh->dir, BUFFER_SIZE);
	ft_printf("🔥  \033[0;32m%s\033[0m$ ", sh->dir);
	get_next_line(0, &buffer);
	// to_command_list(sh);
	buffer = handle_closing_char(sh, buffer, '"');
	buffer = handle_closing_char(sh, buffer, '\'');
	if (ft_strncmp("exit", buffer, 5) == 0)
		sh->stop = 1;
	if (sh->stop)
		ft_printf("🖐  \033[0;31mGood bye!\033[0m\n");
	else
		prompt_line(sh);
	return (SUC);
}
