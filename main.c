/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/21 12:46:18 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	str_freemtx(g->cmds_paths);
	lst_free(&g->env, free);
	// TODO: Implement cleanup
	if (shouldexit)
		exit(status);
	return (status);
}



int	main(int ac, char **av, char **envp)
{
	t_var	mshell;

	(void)av;
	mshell = (t_var){0};
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}
