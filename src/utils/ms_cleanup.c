/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/06/15 14:29:35 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <minishell.h>

void	*freeallcmds(t_list *commands_wrapper, bool free_val)
{
	t_list		*next;
	t_command	*command;

	while (commands_wrapper)
	{
		next = commands_wrapper->next;
		command = commands_wrapper->val;
		if (free_val)
		{
			if (command->in_fd > 2)
				file_close(command->in_fd);
			if (command->out_fd > 2)
				file_close(command->out_fd);
			if (command->last_heredoc_file)
				unlink(command->last_heredoc_file); // is it safe?
			free(command->last_heredoc_file);
			lst_free(&command->args, free);
			lst_free(&command->in_redirects, free);
			lst_free(&command->out_redirects, free);
			free(command);
		}
		free(commands_wrapper);
		commands_wrapper = next;
	}
	return (NULL);
}

int	cleanup(t_var *g, bool shouldexit, int status)
{
	(void)g;
	if (lst_size(g->history) > 0)
		clear_history();
	free(g->last_input);
	lst_free(&g->history, free);
	free(g->home_path);
	lst_free(&g->env, free);
	free(g->curr_path);
	freeallcmds(g->all_cmds, true);
	if (shouldexit)
		exit(status);
	return (status);
}
