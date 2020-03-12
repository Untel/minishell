/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 16:28:28 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/12 17:01:44 by adda-sil         ###   ########.fr       */
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

char	*ft_strchr_escape(const char *s, int c, int escape)
{
	int i;
	int	esc;

	i = 0;
	esc = 0;
	while (s && s[i])
	{
		if (s[i] == escape)
		{
			i += 2;
			esc = 1;
		}
		else if (s[i] == c)
			return ((char *)&s[i]);
		else
			i++;
	}
	return (!c && !s[i] ? (char *)&s[i] : 0);
}
