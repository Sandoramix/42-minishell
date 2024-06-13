/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/13 22:53:37 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#define PIPPO " > "

static char	*hd_line_read(char *heredoc_file, t_list *token)
{
	char	*line;
	char	*temp;

	temp = str_join(heredoc_file, PIPPO);
	line = readline(temp);
	free(temp);
	if (!line || str_equals(line, token->next->val))
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static char	*heredoc_read(t_var *mshell, t_list *token, int count)
{
	char	*line;
	int		heredoc_fd;
	char	*heredoc_file;

	heredoc_file = gen_heredocs(count);
	if (!heredoc_file)
		return (ft_perror("gen_heredocs"), NULL);
	heredoc_fd = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, 0660);
	if (heredoc_fd == -1)
		return (ft_perror("open"), free(heredoc_file), NULL);
	while (42)
	{
		line = hd_line_read(heredoc_file, token);
		if (!line)
			break ;
		if (token->_prevent_expansion == false)
		{
			line = heredoc_expand(mshell, &line);
			token->_prevent_expansion = false;
		}
		write(heredoc_fd, line, str_ilen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	return (close(heredoc_fd), heredoc_file);
}

static int	ms_heredoc(t_var *mshell, t_command *cmd, int count, char *heredoc_name)
{
	t_list	*current;

	current = cmd->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<<"))
		{
			heredoc_name = heredoc_read(mshell, current, count++);
			if (!heredoc_name)
			{
				cmd->in_file = NULL;
				return (KO);
			}
			if (current->next->next != NULL)
			{
				unlink(heredoc_name);
				heredoc_name = ft_free(heredoc_name);
			}
			current = current->next;
		}
		current = current->next;
	}
	return (OK);
}

int	ms_inredir_handle(t_var *mshell, t_command *cmds)
{
	char	*heredoc_name;
	int		count;

	count = 1;
	heredoc_name = NULL;
	if (ms_heredoc(mshell, cmds, count, heredoc_name) == KO)
		return (KO);
	if (heredoc_name != NULL)
		cmds->in_file = heredoc_name;
	if (ms_in_redir(cmds) == KO)
		return (KO);
	return (OK);
}
