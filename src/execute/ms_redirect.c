/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:33:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/15 09:31:54 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_rediout(t_command *command)
{
	t_list	*current;
	char	*out_file;

	current = command->out_redirects;
	out_file = NULL;
	while (current != NULL)
	{
		if (str_equals(current->val, ">") || str_equals(current->val, ">>"))
		{
			if (command->out_fd > 2)
				close(command->out_fd);
			if (str_equals(current->val, ">"))
				command->out_fd = file_open_or_create(current->next->val,
						O_RDWR | O_TRUNC);
			else
				command->out_fd = file_open_or_create(current->next->val,
						O_RDWR | O_APPEND);
			if (command->out_fd == -1)
				return (KO);
		}
		current = current->next->next;
	}
	return (OK);
}

int	open_fd_in(char *filein_name, int *fd)
{
	int	file_fd;

	file_fd = open(filein_name, O_RDONLY);
	if (file_fd == -1)
		return (ft_perror("open"), KO);
	if (*fd > 2)
		close(*fd);
	*fd = file_fd;
	return (OK);
}

int	ms_in_redir(t_command *cmd, int *fd)
{
	t_list	*current;

	current = cmd->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<"))
		{
			if (access(current->next->val, F_OK) != 0)
				return (ft_perror(PROGNAME": %s: No such file or directory\n",
						current->next->val), KO);
			else if (current->next->next == NULL)
				return (open_fd_in(current->next->val, fd));
			current = current->next;
		}
		current = current->next;
	}
	return (OK);
}
