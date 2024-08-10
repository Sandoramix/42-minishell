/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 09:22:57 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*hd_line_read(t_var *mshell, char *heredoc_file, t_list *token)
{
	char	*line;
	char	*temp;

	temp = PROG_PROMPT;
	if (ft_isdebug())
		temp = str_join(heredoc_file, PROMPT);
	line = readline(temp);
	if (ft_isdebug())
		free(temp);
	track_lastsig(mshell);
	if (!line || str_equals(line, token->next->val)
		|| mshell->statuscode == 130)
	{
		if (!line)
			ft_perror(" "PROGNAME": warning: %s (wanted `%s`)",
				"here-document delimited by end-of-file", token->next->val);
		if (!line)
			ft_putstr_fd("\n", 2);
		return (free(line), NULL);
	}
	return (line);
}

static char	*heredoc_read(t_var *mshell, t_list *token, int count)
{
	char	*line;
	int		heredoc_fd;
	char	*heredoc_file;

	heredoc_file = gen_heredocs(mshell, count);
	if (!heredoc_file)
		return (ft_perror("Error: function gen_heredocs failed\n"), NULL);
	heredoc_fd = file_open_or_create(heredoc_file, O_RDWR | O_TRUNC);
	if (heredoc_fd == -1)
		return (free(heredoc_file), NULL);
	dbg_printf(CGRAY"\nheredoc_name=[%s]\n"CR, heredoc_file);
	while (42)
	{
		line = hd_line_read(mshell, heredoc_file, token);
		if (!line && mshell->statuscode == 130)
			return (close(heredoc_fd), free(heredoc_file), NULL);
		if (!line)
			break ;
		if (token->_prevent_expansion == false)
			line = heredoc_expand(mshell, &line);
		ft_putendl_fd(line, heredoc_fd);
		free(line);
	}
	return (close(heredoc_fd), heredoc_file);
}

static int	ms_heredoc(t_var *mshell, t_command *cmd, int *fd)
{
	t_list		*current;
	char		*name;
	int			count;

	current = cmd->in_redirects;
	count = 1;
	while (current != NULL)
	{
		if (str_equals(current->val, "<<"))
		{
			if (*fd > 2)
				file_close(*fd);
			name = heredoc_read(mshell, current, count++);
			if (!name)
				return (KO);
			*fd = open(name, O_RDONLY);
			if (current->next->next != NULL)
				unlink(name);
			else
				cmd->last_heredoc_file = str_dup(name);
			name = ft_free(name);
		}
		current = current->next;
	}
	return (OK);
}

t_state	ms_inredir_handle(t_var *mshell, t_command *command)
{
	int		input_fd;

	input_fd = -1;
	setstatus(mshell, 0);
	if (ms_heredoc(mshell, command, &input_fd) == KO)
		return (KO);
	if (ms_in_redir(mshell, command, &input_fd) == KO)
		return (KO);
	if (input_fd != -1)
		command->in_fd = input_fd;
	return (OK);
}
