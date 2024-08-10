/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 10:19:56 by odudniak          #+#    #+#             */
/*   Updated: 2024/08/10 09:18:30 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	g_setlastsig(int sig)
{
	g_lastsig = sig;
	return (g_lastsig);
}

t_uchar	setstatus(t_var *mshell, t_uchar code)
{
	mshell->statuscode = code;
	return (code);
}

t_state	track_lastsig(t_var *mshell)
{
	if (g_lastsig == INT_MIN)
		return (KO);
	if (g_lastsig == SIGINT)
		mshell->statuscode = 130;
	if (g_lastsig == SIGQUIT)
		mshell->statuscode = 131;
	return (OK);
}
