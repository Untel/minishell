/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 21:55:11 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/26 03:31:21 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>

int			main(int ac, char **av)
{
	char	*line;
	int		mode;

	line = NULL;
	mode = SINGLE;
	while (--ac >= 1)
		if(!strcmp(av[ac], "--ml"))
			mode = MULTI;
	load_history(g_history.filename, &g_history);
	while (1)
	{
		line = read_input("example> ", mode);
		if (line == (char *)-1 || line == NULL)
			break ;
		ft_printf("return: [%s]\n", line);
		free(line);
		line = NULL;
	}
	return (0);
}
