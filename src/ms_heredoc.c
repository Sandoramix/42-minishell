/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/12 10:24:23 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

#define PIPPO " > "

/*
	<< EOF << lol

TODO: refactor che così fa schif
*/

static char	*heredoc_read(t_var *mshell, t_list *token, int count)
{
	char	*line;
	int		heredoc_fd;
	char	*heredoc_file;
	char	*temp;

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

int	ms_heredoc(t_var *mshell, t_command *cmds)
{
	t_list	*current;
	char	*heredoc_name;
	char	*filein_name;
	int		count;
	int		fd;

	count = 1;
	heredoc_name = NULL;
	filein_name = NULL;
	current = cmds->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<<"))
		{
			heredoc_name = heredoc_read(mshell, current, count++);
			if (!heredoc_name)
			{
				cmds->in_file = NULL;
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
	if (heredoc_name != NULL)
		cmds->in_file = heredoc_name;
	current = cmds->in_redirects;
	while (current != NULL)
	{
		if (str_equals(current->val, "<"))
		{
			filein_name = ft_free(filein_name);
			filein_name = str_dup(current->next->val);
			cmds->in_file = filein_name;
			if (access(filein_name, F_OK) != 0)
				return (ft_perror(PROGNAME": %s: No such file or directory\n",
						filein_name), KO);
			else if (current->next->next == NULL)
			{
				fd = open(filein_name, O_RDONLY);
				if (fd == -1)
					return (ft_perror("open"), KO);
				close(fd);
			}
			current = current->next;
		}
		current = current->next;
	}
	return (OK);
}
