/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:41:06 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/31 12:57:32 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_uchar	g_status;

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		g_status = 130;
		rl_replace_line("", 0);
		ioctl(STDIN_FILENO, TIOCSTI,"\n");
		rl_on_new_line();
	}
}

// TODO refactor
void	*freeallcmds(t_list *cmds)
{
	t_list	*next;
	while (cmds)
	{
		next = cmds->next;
		free(cmds);
		cmds = next;
	}
	return (NULL);
}
int	main(int ac, char **av, char **envp)
{
	t_var	mshell;

	(void)av;
	mshell = (t_var){0};
	g_status = 0;
	if (ac != 1)
		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	mshell._main.envp = envp;
	ms_init(&mshell);
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
	ms_prompt(&mshell);
	return (cleanup(&mshell, true, 0));
}
