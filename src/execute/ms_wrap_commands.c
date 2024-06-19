/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_wrap_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 14:17:16 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/19 11:44:16 by odudniak         ###   ########.fr       */
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

static void	ms_wrapcmds_debug(t_command *cmd)
{
	if (!ft_isdebug())
		return ;
	dbg_printf(CBGRAY"Command + args:\n");
	lst_printstr(cmd->args);
	dbg_printf(CGRAY"Input redirects:\n");
	lst_printstr(cmd->in_redirects);
	dbg_printf(CGRAY"Output redirects:\n");
	lst_printstr(cmd->out_redirects);
}

bool	ms_wrap_commands(t_var *mshell)
{
	const char	*in_redirs[] = {"<", "<<", NULL};
	const char	*out_redirs[] = {">", ">>", NULL};
	t_list		*cmds;
	t_command	*command;

	cmds = mshell->all_cmds;
	while (cmds)
	{
		command = ft_calloc(1, sizeof(t_command));
		if (!command)
			return (ms_wrap_cleanup(mshell, cmds), false);
		command->in_fd = STDIN_FILENO;
		command->out_fd = STDOUT_FILENO;
		command->args = cmds->val;
		extract_redirs(&command->args, &command->in_redirects, in_redirs);
		extract_redirs(&command->args, &command->out_redirects, out_redirs);
		cmds->val = command;
		cmds = cmds->next;
		ms_wrapcmds_debug(command);
	}
	return (true);
}
