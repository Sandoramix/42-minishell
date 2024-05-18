/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/18 17:17:44 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//#if DEBUG == false


// TODO REMOVE ME
t_var *tmpshell;
void	safe_exit(int signal)
{
	(void)signal;
	cleanup(tmpshell, true, 130);
}

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
	signal(SIGINT, safe_exit);
	tmpshell = &mshell;
	args = cmd_parse("unset LS_COLORS TERMINATOR XDG_DATA_DIRS \
	XDG_SESSION_PATH SESSION_MANAGER GIO_LAUNCHED_DESKTOP_FILE PAGER LESS \
	SHLVL  LANGUAGE GJS_DEBUG_TOPICS ZSH LOGNAME LANG");
	expand_and_clear(&mshell, args);
	ms_unset(&mshell, args);
	lst_free(&args, free);
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}

//#endif
