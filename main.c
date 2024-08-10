/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 08:52:46 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_lastsig;

int	main(int argc, char **argv, char **envp)
{
	t_var	mshell;

	mshell = (t_var){0};
	g_setlastsig(INT_MIN);
	mshell._main.argc = argc;
	mshell._main.envp = envp;
	mshell._main.argv = argv;
	ms_init(&mshell);
	if (argc > 1)
		return (ms_exec_script(&mshell), mshell.statuscode);
	else
		ms_prompt(&mshell);
	return (cleanup(&mshell, true, mshell.statuscode));
}
