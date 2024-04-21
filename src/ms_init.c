/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:33:15 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/21 12:49:40 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_loadenv(t_var *mshell)
{
	mshell->env = env_load(mshell->_main.envp);
	if (!mshell->env)
		return (pf_errcode(ERR_ENV_LOAD), cleanup(mshell, true, 1));
	mshell->cmds_paths = env_load_paths(mshell->env);
	return (0);
}

void ms_init(t_var *mshell)
{
	ms_loadenv(mshell);
	//lst_printstr(mshell->env);
}
