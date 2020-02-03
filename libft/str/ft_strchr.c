/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 16:28:28 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 21:57:17 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	while (s && *s)
		if (*s == c)
			return ((char *)s);
		else
			s++;
	return (!c && !*s ? (char *)s : 0);
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
