/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 08:25:37 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/07 09:06:27 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

void    loop_pipe(char ***cmd, char **ev)
{
	int   p[2];
	pid_t pid;
	int   fd_in = 0;

	while (*cmd != NULL)
	{
		pipe(p);
		if ((pid = fork()) == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
		{
			dup2(fd_in, 0); //change the input according to the old one
			if (*(cmd + 1) != NULL)
			dup2(p[1], 1);
			close(p[0]);
			execve((*cmd)[0], *cmd, ev);
			exit(EXIT_FAILURE);
		}
		else
		{
			wait(NULL);
			close(p[1]);
			fd_in = p[0]; //save the input for the next command
			cmd++;
		}
	}
}

int main(int ac, char **av, char **ev)
{
	char *ls[] = {"/bin/ls", NULL};
	char *grep[] = {"/usr/bin/grep", "pipe", NULL};
	char **cmd[] = {ls, grep, NULL};

	loop_pipe(cmd, ev);
	return (0);
}
