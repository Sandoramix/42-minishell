/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:38:19 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/01 16:05:04 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO refactor
void	*freeallcmds(t_list *cmds, bool free_val)
{
	t_list	*next;
	t_list	*cmd;

	while (cmds)
	{
		next = cmds->next;
		cmd = cmds->val;
		if (free_val)
			lst_free(&cmd, free);
		free(cmds);
		cmds = next;
	}
	return (NULL);
}

// TODO: Implement cleanup
int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	if (lst_size(g->history) > 0)
		clear_history();
	free(g->last_input);
	lst_free(&g->history, free);
	free(g->home_path);
	str_freemtx(g->cmds_paths);
	lst_free(&g->env, free);
	free(g->curr_path);
	freeallcmds(g->all_cmds, true);
	if (shouldexit)
		exit(status);
	return (status);
}
