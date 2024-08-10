/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 22:23:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/08/10 09:11:48 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		g_setlastsig(SIGINT);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	handle_exec_sig(int signal)
{
	if (signal == SIGINT)
	{
		ft_fprintf(2, "\n");
		g_setlastsig(SIGINT);
	}
}
