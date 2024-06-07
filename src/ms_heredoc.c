/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/07 17:15:19 by marboccu         ###   ########.fr       */
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
			printf("line: %s\n", line);
			token->_prevent_expansion = false;
		}
		write(heredoc_fd, line, str_ilen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	return (heredoc_file);
}

int ms_heredoc(t_var *mshell, t_command *cmds)
{
	t_list	*current;
	char *delimiter;
	char *heredoc_name;
	int count;

	count = 1;
	heredoc_name = NULL;
	current = cmds->args;
	while (current)
	{
		if (str_cmp(current->val, "<<") == 0)
		{
			delimiter = current->next->val;
			heredoc_name = heredoc_read(mshell, current, count++);
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
	{
		cmds->in_file = heredoc_name;
		printf("heredoc_name: %s\n", cmds->in_file);
	}
	// 	exec_heredoc_cmd(mshell, args);
	return (OK);
}

// char **ft_lst_to_cmd_array(t_list *cmd)
// {
//     int count;
//     t_list *current; 
//     int skip_next;

// 	count = 0;
// 	current = cmd;
// 	skip_next = 0;
//     while (current)
//     {
//         if (skip_next)
//             skip_next = 0;
//         else if (str_equals(current->val, "<<"))
//             skip_next = 1;
//         else
//             count++;
//         current = current->next;
//     }

//     char **argv = malloc((count + 1) * sizeof(char *));
//     if (!argv)
//         return NULL;

//     current = cmd;
//     int i = 0;
//     skip_next = 0;
//     while (current)
//     {
//         if (skip_next)
//             skip_next = 0;
//         else if (str_equals(current->val, "<<"))
//             skip_next = 1;
//         else
//             argv[i++] = current->val;
//         current = current->next;
//     }
//     argv[count] = NULL;
//     return argv;
// }

// int exec_heredoc_cmd(t_var *mshell, t_list *args)
// {
// 	int fd;
// 	pid_t pid;
// 	char **cmd;
// 	char *cmd_path;

// 	fd = open(mshell->heredoc_file, O_RDONLY);
// 	if (fd == -1)
// 		return (pf_errcode(ERR_SYNTAX), KO);
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		close(fd);
// 		return (pf_errcode(ERR_FORK), KO);
// 	}
// 	if (pid == 0)
// 	{
// 		dup2(fd, STDIN_FILENO);
// 		close(fd);
// 		cmd = ft_lst_to_cmd_array(args);
// 		cmd_path = sys_findcmdpath(mshell->cmds_paths, cmd[0]);
// 		if (!cmd_path)
// 			return (ft_perror("Command not found"), KO);
// 		execve(cmd_path, cmd, mshell->_main.envp);
// 		ft_perror("execve");
// 		exit(KO);
// 	}
// 	else
// 	{
// 		close(fd);
// 		wait(NULL);
// 	}
// 	return (OK);
// }
