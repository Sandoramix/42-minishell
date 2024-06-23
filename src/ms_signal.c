/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:23:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/23 22:29:24 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		g_set_status(130);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	handle_child_ret_sig(int signal)
{
	if (signal == SIGINT)
		g_set_status(130);
	else if (signal == SIGQUIT)
		g_set_status(131);
}

void	handle_exec_sig(int signal)
{
	if (signal == SIGINT)
	{
		ft_fprintf(2, "\n");
		g_set_status(130);
	}
}