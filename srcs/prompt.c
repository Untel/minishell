/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/05/11 13:23:41 by user42           ###   ########.fr       */
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
	sh->input = read_input(prompt, MULTI, ft_strlen(sh->printed_dir) + 3);
	ft_memdel((void **)&prompt);
	if (sh->input == (char*)-3)
	{
		sh->input = ft_strdup("");
		sh->last_ret = 130;
	}
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
