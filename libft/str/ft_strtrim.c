/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 18:24:41 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/22 21:27:14 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

char	*ft_strtrim(char const *s1, char const *set)
{
	int			startl;
	int			endl;

	if (!s1)
		return (NULL);
	startl = 0;
	endl = ft_strlen(s1);
	if (set)
	{
		while ((s1[startl] && ft_strchr(set, s1[startl])))
			startl++;
		while ((s1[endl - 1] && (endl - 1 >= startl)
			&& ft_strchr(set, s1[endl - 1])))
			endl--;
	}
	endl = endl - startl;
	return (ft_substr(s1, startl, endl > 0 ? endl : 0));
}
