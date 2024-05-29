/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/29 15:29:54 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO REMOVE ME
t_var	*g_tmpshell;

void	safe_exit(int signal)
{
	(void)signal;
	cleanup(g_tmpshell, true, 130);
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
	signal(SIGINT, safe_exit);
	g_tmpshell = &mshell;
	// parse_and_exec(&mshell, "unset LS_COLORS TERMINATOR XDG_DATA_DIRS
	// XDG_SESSION_PATH SESSION_MANAGER GIO_LAUNCHED_DESKTOP_FILE PAGER LESS
	// SHLVL  LANGUAGE GJS_DEBUG_TOPICS ZSH LOGNAME LANG");
	// parse_and_exec(&mshell, "export a=l b=s");
	// parse_and_exec(&mshell, "$a$b");
	//parse_and_exec(&mshell, "export A='test'");
	//parse_and_exec(&mshell, "''ech'o'\"\" $'A' $A \"$A\" '$A' |'$'A|");
	//parse_and_exec(&mshell, "exit");
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}
