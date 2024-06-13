/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:33:44 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/12 14:49:34 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// TODO: refactor


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
