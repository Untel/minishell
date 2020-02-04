/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 16:28:28 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 09:43:29 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	int i;

	i = 0;
	while (s && s[i])
		if (s[i] == c)
			return ((char *)&s[i]);
		else
			i++;
	return (!c && !s[i] ? (char *)&s[i] : 0);
}

char	*ft_strchr_escape(const char *s, int c)
{
	while (s && *s)
		if (*s == c)
			return ((char *)s);
		else
			s++;
	return (!c && !*s ? (char *)s : 0);
}
