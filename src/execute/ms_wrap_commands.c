/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wrap_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:17:16 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/09 15:07:02 by odudniak         ###   ########.fr       */
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

static t_list	*extract_in_redirs(t_list **args, t_list **res)
{
	t_list	*tmp;
	t_list	*node;

	tmp = *args;
	while (tmp)
	{
		if (*args && tmp->type == A_TOKEN && (str_equals(tmp->val, "<")
				|| str_equals(tmp->val, "<<")))
		{
			node = tmp;
			if (!node->prev)
				*args = node->next->next;
			if (node->prev)
				node->prev->next = node->next->next;
			if (node->next->next)
				node->next->next->prev = node->prev;
			node->prev = lst_gettail(*res);
			tmp = node->next->next;
			node->next->next = NULL;
			if (!*res)
				*res = node;
			else
				lst_gettail(*res)->next = node;
		}
		else
			tmp = tmp->next;
	}
	return (*res);

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
		extract_in_redirs(&cmd_container->args, &cmd_container->in_redirects);
		cmds->val = cmd_container;
		cmds = cmds->next;
	}
	return (true);
}
