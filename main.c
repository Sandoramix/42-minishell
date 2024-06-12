/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/12 10:32:43 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_uchar	g_status;

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		g_status = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	main(int ac, char **av, char **envp)
{
	t_var	mshell;

	(void)av;
	mshell = (t_var){0};
	g_status = 0;
	if (ac != 1)
		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}
