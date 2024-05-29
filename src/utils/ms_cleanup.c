/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:38:19 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/29 20:31:40 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: Implement cleanup
int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	if (lst_size(g->history) > 0)
		clear_history();
	lst_free(&g->history, free);
	free(g->home_path);
	str_freemtx(g->cmds_paths);
	lst_free(&g->env, free);
	free(g->curr_path);
	if (shouldexit)
		exit(status);
	return (status);
}
