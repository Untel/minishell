/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/04/24 17:59:09 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "line_edit.h"

int
	ft_read(t_shell *sh)
{
	char	*prompt;
	char	tmp[4096];

	prompt = NULL;
	ft_sprintf(tmp, sh->last_ret == EXIT_SUCCESS ?
		MSG_PROMPT : MSG_PROMPT_ERR, sh->printed_dir);
	append(&prompt, ft_strdup(tmp));
	sh->input = read_input(prompt, MULTI, ft_strlen(sh->printed_dir) + 6 +
			(sh->last_ret == EXIT_SUCCESS));
	ft_memdel((void **)&prompt);
	if (sh->input == ((char *)-1))
		sh->input = NULL;
	return (!!sh->input);
}

int
	prompt_line(t_shell *sh)
{
	if (!ft_read(sh))
		sh->stop = 1;
	if (!sh->stop)
	{
		if (sanitize(sh))
		{
			parse_input(sh);
			ft_memdel((void **)&sh->input);
		}
	}
	else
		ft_memdel((void **)&sh->input);
	clear_last_prompt(sh);
	return (SUC);
}
