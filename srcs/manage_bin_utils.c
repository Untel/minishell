/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_bin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 19:48:08 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/24 19:54:16 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		fe_init(t_shell *sh, t_cmd *cmd, char **paths)
{
	char	*tmp[2];
	int		ret;
	int		i;

	ret = 0;
	tmp[1] = cmd->argv[0];
	i = -1;
	while (paths[++i])
	{
		tmp[0] = paths[i];
		ret = test_dir(tmp[0], tmp[1]);
		if (ret == 1)
			break ;
	}
	if (ret != 1)
	{
		free_env_array(paths);
		return (1);
	}
	ret = fork_exec(sh, cmd, tmp, 2);
	return (0);
}
