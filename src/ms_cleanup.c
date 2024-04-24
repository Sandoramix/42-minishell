/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:38:19 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/24 15:36:39 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: Implement cleanup
int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	str_freemtx(g->cmds_paths);
	lst_free(&g->env, free);
	free(g->status_code);
	free(g->curr_path);
	if (shouldexit)
		exit(status);
	return (status);
}
