/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/09 16:20:06 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#define PIPPO " > "

/*
	<< EOF << lol

TODO: refactor che così fa schif
TODO: Leakssss
*/

static char *heredoc_read(t_var *mshell, t_list *token, int count)
{
	char	*line;
	int heredoc_fd;
	char *heredoc_file;
	char *temp;

	heredoc_file = gen_heredocs(count);
	if (!heredoc_file)
		return (ft_perror("gen_heredocs"), NULL);
	heredoc_fd = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, 0660);
	if (heredoc_fd == -1)
		return (ft_perror("open"), free(heredoc_file), NULL);
	while (42)
	{
		temp = str_join(heredoc_file, PIPPO);
		line = readline(temp);
		free(temp);
		if (!line || str_equals(line, token->next->val))
		{
			free(line);
			break ;
		}
		if (token->_prevent_expansion == false)
		{
			line = heredoc_expand(mshell, &line);
			token->_prevent_expansion = false;
		}
		write(heredoc_fd, line, str_ilen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	return (heredoc_file);
}

void ms_heredoc(t_var *mshell, t_command *cmds)
{
	t_list	*current;
	char *delimiter;
	char *heredoc_name;
	int count;

	count = 1;
	heredoc_name = NULL;
	current = cmds->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<<"))
		{
			delimiter = current->next->val;
			heredoc_name = heredoc_read(mshell, current, count++);
			if (current->next->next != NULL)
			{
				unlink(heredoc_name);
				//heredoc_name = ft_free(heredoc_name);
			}
			current = current->next;
		}
		if (str_equals(current->val, "<"))
		{
			cmds->in_file = current->next->val;
			printf("in_file: %s\n", cmds->in_file);
			//current = current->next;
			//if (cmds)
		}
		current = current->next;
	}
	if (heredoc_name != NULL)
		cmds->in_file = heredoc_name;
}

