/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wrap_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:17:16 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/09 14:20:17 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	ms_wrap_cleanup(t_var *mshell, t_list *cmds_without_command)
{
	t_list	*next;
	t_list	*cmd;

	if (cmds_without_command->prev)
		cmds_without_command->prev->next = NULL;
	freeallcmds(mshell->all_cmds, true);
	while (cmds_without_command)
	{
		cmd = cmds_without_command->val;
		next = cmds_without_command->next;
		lst_free(&cmd, free);
		free(cmds_without_command);
		cmds_without_command = next->prev;
	}
}

bool	ms_wrap_commands(t_var *mshell)
{
	t_list		*cmds;
	t_command	*cmd_container;

	cmds = mshell->all_cmds;
	while (cmds)
	{
		cmd_container = ft_calloc(1, sizeof(t_command));
		if (!cmd_container)
			return (ms_wrap_cleanup(mshell, cmds), false);
		cmd_container->args = cmds->val;
		cmds->val = cmd_container;
		cmds = cmds->next;
	}
	return (true);
}
