/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/23 12:36:35 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_uchar	g_status;

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		g_set_status(130);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	if (signal == SIGQUIT)
		g_set_status(131);
}

int	main(int argc, char **argv, char **envp)
{
	t_var	mshell;

	mshell = (t_var){0};
	g_set_status(0);
	mshell._main.argc = argc;
	mshell._main.envp = envp;
	mshell._main.argv = argv;
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
	ms_init(&mshell);
	if (argc > 1)
		return (ms_exec_script(&mshell), g_status);
	ms_prompt(&mshell);
	ft_fprintf(2, "exit\n");
	return (cleanup(&mshell, true, g_status));
}
