/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:38:19 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/21 16:38:40 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	str_freemtx(g->cmds_paths);
	lst_free(&g->env, free);
	free(g->status_code);
	// TODO: Implement cleanup
	if (shouldexit)
		exit(status);
	return (status);
}