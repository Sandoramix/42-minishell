/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 20:35:27 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/24 18:06:29 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	ms_exec_update_stds(t_var *mshell, t_command *command, int idx)
{
	const int	tot_cmds = lst_size(mshell->all_cmds);
	int			*curr_fds;
	int			*prev_fds;

	curr_fds = mshell->pipes[idx];
	prev_fds = NULL;
	if (idx > 0)
		prev_fds = mshell->pipes[idx - 1];
	if (idx > 0 && command->in_fd <= 2 && tot_cmds > 1)
		command->in_fd = prev_fds[0];
	if (command->out_fd <= 2 && idx < tot_cmds - 1)
		command->out_fd = curr_fds[1];
	if (command->in_fd > 2)
		dup2(command->in_fd, STDIN_FILENO);
	if (command->out_fd > 2)
		dup2(command->out_fd, STDOUT_FILENO);
	files_close((int [3]){command->out_fd, command->in_fd, curr_fds[0]}, 3);
	dbg_printf(CGREEN"cmd[%s]\tin=%d\tout=%d\n"CR, command->args->val,
		command->in_fd, command->out_fd);
	if (prev_fds)
		close(prev_fds[1]);
	close_pipes(mshell);
	return (OK);
}

t_list	*ms_split_pipelines(t_list *all)
{
	t_list	*split;
	t_list	*tmp;
	t_list	*next;

	split = NULL;
	tmp = all;
	while (all)
	{
		next = all->next;
		if (next && next->type == A_TOKEN && str_equals(next->val, "|"))
		{
			next = all->next->next;
			lst_delnode(&all->next, free);
			all->next = NULL;
			next->prev = NULL;
			if (!lst_addnew_tail(&split, tmp, NULL))
				return (lst_free(&split, free), lst_free(&next, free));
			tmp = next;
		}
		all = next;
	}
	if (!split || tmp)
		lst_addnew_tail(&split, tmp, NULL);
	return (split);
}
