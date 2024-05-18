/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/18 13:50:29 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//#if DEBUG == false

int	main(int ac, char **av, char **envp)
{
	t_var	mshell;
	t_list	*args;

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	// args = cmd_parse("echo diopo");
	// expand_and_clear(&mshell, args);
	// ms_echo(&mshell, args);
	// lst_free(&args, free);
	// args = cmd_parse("history");
	// expand_and_clear(&mshell, args);
	// ms_history(&mshell, args);
	lst_free(&args, free);
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}

//#endif
