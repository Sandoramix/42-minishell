/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:33:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/13 12:44:44 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_rediout(t_command *cmds)
{
	t_list *current;
	int fd;
	char *out_file;

	current = cmds->out_redirects;
	out_file = NULL;
	while (current != NULL)
	{
		if (str_equals(current->val, ">") || str_equals(current->val, ">>"))
		{
			out_file = ft_free(out_file);
			out_file = str_dup(current->next->val);
			cmds->out_file = out_file;
			if (str_equals(current->val, ">"))
				fd = open(out_file, O_CREAT | O_RDWR | O_TRUNC, 0660);
			else
				fd = open(out_file, O_CREAT | O_RDWR | O_APPEND, 0660);
			if (fd == -1)
				ft_perror("open");
			close(fd);
		}
		current = current->next->next;
	}
}

int	open_fd_in(char *filein_name)
{
	int	fd;

	fd = open(filein_name, O_RDONLY);
	if (fd == -1)
		return (ft_perror("open"), KO);
	close(fd);
	return (OK);
}

int	ms_in_redir(t_command *cmd)
{
	char	*filein_name;
	t_list	*current;

	filein_name = NULL;
	current = cmd->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<"))
		{
			filein_name = ft_free(filein_name);
			filein_name = str_dup(current->next->val);
			cmd->in_file = filein_name;
			if (access(filein_name, F_OK) != 0)
				return (ft_perror(PROGNAME ": %s: No such file or directory\n",
						filein_name), KO);
			else if (current->next->next == NULL)
				return (open_fd_in(filein_name));
			current = current->next;
		}
		current = current->next;
	}
	return (OK);
}
