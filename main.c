/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/23 22:34:25 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_uchar	g_status;

int	main(int argc, char **argv, char **envp)
{
	t_var	mshell;

	mshell = (t_var){0};
	g_set_status(0);
	mshell._main.argc = argc;
	mshell._main.envp = envp;
	mshell._main.argv = argv;
	ms_init(&mshell);
	if (argc > 1)
		return (ms_exec_script(&mshell), g_status);
	else
		ms_prompt(&mshell);
	return (cleanup(&mshell, true, g_status));
}
