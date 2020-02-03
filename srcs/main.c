/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 19:03:12 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_printf.h"

__attribute__((destructor)) void quit(void)
{
	for(;;)
		;
}

int	main(int ac, char **av, char **envp)
{
	t_env	env;
	t_list	*lst_env;

	(void)ac;
	(void)av;

	// test
	lst_env = create_env_list(envp);

	set_value(&lst_env, "hello", "world");
	print_lst_env(lst_env);
	set_value(&lst_env, "USER", "test");
	envp = convert_env_list(lst_env);
	free_env_list(&lst_env);

	// print new envp test
	int		i = -1;
	while (envp[++i])
		printf("%s\n", envp[i]);
	//
	free_env_array(envp);

	env = (t_env) { .dir = "AShellM", .stop = 0 };
	prompt_line(&env);
	return (0);
}
