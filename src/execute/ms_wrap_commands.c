/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wrap_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:17:16 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/09 15:39:17 by odudniak         ###   ########.fr       */
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

static bool	is_known_redir(t_list *node, const char **whitelist)
{
	int	i;

	i = -1;
	if (!node || node->type != A_TOKEN)
		return (false);
	while (whitelist && whitelist[++i])
	{
		if (str_equals(node->val, (char *)whitelist[i]))
			return (true);
	}
	return (false);
}

static t_list	*extract_redirs(t_list **args, t_list **res, const char **tkns)
{
	t_list	*tmp;
	t_list	*node;

	tmp = *args;
	while (tmp)
	{
		if (is_known_redir(tmp, tkns))
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
			lst_addnode_tail(res, node);
		}
		else
			tmp = tmp->next;
	}
	return (*res);

}

bool	ms_wrap_commands(t_var *mshell)
{
	const char	*in_redirs[] = {"<", "<<", NULL};
	const char	*out_redirs[] = {">", ">>", NULL};
	t_list		*cmds;
	t_command	*container;

	cmds = mshell->all_cmds;
	while (cmds)
	{
		container = ft_calloc(1, sizeof(t_command));
		if (!container)
			return (ms_wrap_cleanup(mshell, cmds), false);
		container->args = cmds->val;
		extract_redirs(&container->args, &container->in_redirects, in_redirs);
		dbg_printf("args after in redirs extraction:\n");
		lst_printstr(container->args);
		extract_redirs(&container->args, &container->out_redirects, out_redirs);
		dbg_printf("args after out redirs extraction:\n");
		cmds->val = container;
		cmds = cmds->next;
		if (dbg_printf("") == -1)
			continue ;
		dbg_printf("Input redirects:\n");
		lst_printstr(container->in_redirects);
		dbg_printf("Output redirects:\n");
		lst_printstr(container->out_redirects);
	}
	return (true);
}
