/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/07 10:51:54 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
	<< EOF << lol
TODO: unlink per bene dei file heredoc, considerando che puo essercene piu di uno, che dopo l' ultimo
ci puo essere un altro comando o token (es. > file) che non ha bisogno di heredoc e quindi in quel caso va chiuso (?)
TODO: salvarsi ultimo heredoc che non deve essere cancellato, quindi se il nodo dopo << del e' NULL, allora quello e' l'ultimo heredoc e non va chiuso
minishell> << l << m << n << o
heredoc > l
Added heredoc: l heredoc_0
heredoc > m
Added heredoc: m heredoc_1
heredoc > n
Added heredoc: n heredoc_2
heredoc > o
Added heredoc: o heredoc_3
unlinking heredoc_0
unlinking heredoc_1
unlinking heredoc_2
last heredoc: heredoc_3
minishell> << l < file
heredoc > ciao
heredoc > l
Added heredoc: l heredoc_4
last heredoc: heredoc_4
minishell> exit

TODO: risolvere no espansione se "" nel 2nd heredoc
TODO: refactor che così fa schif
TODO: Leakssss
*/

void add_heredoc(t_command *cmd, char *delimiter, char *heredoc_name)
{
	// char *heredoc_file;
	// char *delimiter_copy;
	// char *

	// heredoc_file = str_dup(heredoc_name);
	// if (!heredoc_file)
	// 	return ;
	// delimiter_copy = str_dup(delimiter);
	// if (!delimiter_copy)
	// 	return ;
	// heredoc_file = str_join(delimiter_copy, heredoc_file);
	// if (!heredoc_file)
	// 	return ;
	// return (heredoc_file);
	t_list *heredoc;

	heredoc = malloc(sizeof(t_list));
	if (!heredoc)
		return ;
	heredoc->key = str_dup(delimiter);
	heredoc->val = str_dup(heredoc_name);
	heredoc->next = NULL;
	heredoc->prev = NULL;
	if (!cmd->heredocs)
		cmd->heredocs = heredoc;
	else
		{
			t_list *current = cmd->heredocs;
			while (current->next)
				current = current->next;
			current->next = heredoc;
			heredoc->prev = current;
		}
	// 	printf("Added heredoc: %s %s\n", (char *)heredoc->key, (char *)heredoc->val);
}

static char *heredoc_read(t_var *mshell, const char *delimiter, t_command *cmd, int count)
{
	char	*line;
	int heredoc_fd;
	char *heredoc_file;

	heredoc_file = gen_heredocs(count);
	if (!heredoc_file)
		return (ft_perror("gen_heredocs"), NULL);
	heredoc_fd = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, 0660);
	if (heredoc_fd == -1)
		return (ft_perror("open"), free(heredoc_file), NULL);
	while (42)
	{
		line = readline("heredoc > ");
		if (!line || str_cmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (cmd->args->_prevent_expansion == false)
		{
			while (str_idxofchar(line, '$') != -1)
			{
				line = heredoc_expand(mshell, &line);
				printf("line: %s\n", line);
				cmd->args->_prevent_expansion = false;
			}
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
	cleanup_heredocs(cmds);
	current = cmds->args;
	while (current)
	{
		if (str_cmp(current->val, "<<") == 0)
		{
			delimiter = current->next->val;
			heredoc_name = heredoc_read(mshell, delimiter, cmds, count++);
			if (heredoc_name)
				add_heredoc(cmds, delimiter, heredoc_name);
			if (current->next->next != NULL)
				unlink_heredocs(cmds);
			current = current->next;
		}
		current = current->next;
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
