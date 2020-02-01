/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/13 17:29:54 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/26 20:12:39 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

typedef	struct		s_fds
{
	int				fd;
	int				eof;
	char			*data;
	struct s_fds	*next;
	struct s_fds	*prev;
}					t_fds;
int					get_next_line(int fd, char **line);
#endif
#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif
