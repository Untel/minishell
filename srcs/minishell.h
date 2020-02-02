/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:32:31 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 16:26:58 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"
# include <sys/wait.h>
# include <unistd.h>
# include <stdio.h>
# define FALSE 0
# define SUC 1
# define ERR -1

typedef enum	s_sep
{
	PIPE = '|',
	END = ';',
}				t_sep;

typedef struct
typedef struct	s_shell
{
	char		*input;				//malloced
	char		dir[BUFFER_SIZE];
	int			stop;
}				t_shell;

int		prompt_line(t_shell *sh);
#endif