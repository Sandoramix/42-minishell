/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 10:37:00 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/23 17:32:41 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	reset_stds(t_var *mshell)
{
	dup2(mshell->orig_stdin, STDIN_FILENO);
	dup2(mshell->orig_stdout, STDOUT_FILENO);
	return (OK);
}

void	*clean_cmds(t_list *commands_wrapper, bool free_val)
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
				unlink(command->last_heredoc_file);
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
	if (lst_size(g->history) > 0)
		clear_history();
	lst_free(&g->history, free);
	free(g->home_path);
	lst_free(&g->env, free);
	free(g->curr_path);
	files_close((int [2]){g->orig_stdin, g->orig_stdout}, 2);
	files_close((int [2]){STDIN_FILENO, STDOUT_FILENO}, 2);
	files_close(g->pipes[0], 2);
	files_close(g->pipes[1], 2);
	clean_cmds(g->all_cmds, true);
	str_freemtx(g->script_content);
	if (shouldexit)
		exit(status);
	return (status);
}
