/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 11:49:54 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 09:24:17 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ------------------------------- dliste.h ------------------------------------
*/

#ifndef LIST_H
# define LIST_H

typedef struct	s_lst_in
{
	char			c;
	struct s_lst_in	*prev;
	struct s_lst_in	*next;
}				t_lst_in;

typedef struct	s_data
{
	t_lst_in	*head;
	t_lst_in	*end;
	int			size;
}				t_data;

/*
** ---------------------- initialisation de la liste ---------------------------
*/

void			init_lst(t_data *liste);

/*
** ------------------------------ INSERTION ------------------------------------
*/

int				add_empty(t_data *lst, char c);
int				add_begin(t_data *lst, char c);
int				add_end(t_data *lst, char c);
int				add_before(t_data *lst, char c, int pos);
int				add_after(t_data *lst, char c, int pos);

/*
**------------------------------- SUPPRESSION ----------------------------------
*/

int				delone(t_data *lst, int pos);

/*
** *****************************************************************************
*/

void			affiche_inv(t_data *lst);
char			*convert_to_str(t_data *lst);
void			free_all(t_data *lst);
#endif

/*
** ------------------------------ FIN liste.h ----------------------------------
*/
