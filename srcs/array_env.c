/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 19:11:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/02 20:41:25 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include <stdlib.h>

void	print_env(char **envp)
{
	int		i;

	i = -1;
	while (*(envp + ++i))
		ft_printf("%s\n", *(envp + i));
}

char	*set_value(char **envp, char *key, char *value)
{
	char	*tmp;
	char	*strs[2];
	int		i;
	int		t;

	t = 0;
	i = -1;
	while (*(envp + ++i))
		if (!ft_strncmp(*(envp + i), key, ft_strlen(key)) && (t = 1))
		{
			strs[0] = key;
			strs[1] = value;
			tmp = ft_strmjoin(3, strs, "=");
			free(*(envp + i));
			*(envp + i) = tmp;
		}
	return (NULL);

}

char	*get_value(char **envp, char *key)
{
	int		i;

	i = -1;
	while (*(envp + ++i))
		if (!ft_strncmp(*(envp + i), key, ft_strlen(key)))
			return (*(envp + i) + ft_strlen(key) + 1);
	return (NULL);
}
