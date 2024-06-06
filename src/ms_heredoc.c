/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/06 13:06:52 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*
	<< EOF << lol
TODO: unlink per bene dei file heredoc, considerando che puo essercene piu di uno, che dopo l' ultimo
ci puo essere un altro comando o token (es. > file) che non ha bisogno di heredoc e quindi in quel caso non va chiuso (?)
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

TODO: creare gen_heredocs x generare i file heredoc in modo incrementale e ritornare il nome del file
TODO: refactor che così fa schif
TODO: Leakssss
*/
char	*heredoc_expand(t_var *mshell, char **arg)
{
	int		dollar_idx;
	char	*res;

	dollar_idx = str_idxofchar(*arg, '$');
	while (dollar_idx != -1)
	{
		res = arg_update(mshell, arg, &dollar_idx, true);
		if (!res)
			return (NULL);
	}
	return (*arg);
}

void add_heredoc(t_command *cmd, char *delimiter, char *heredoc_name)
{
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
		printf("Added heredoc: %s %s\n", (char *)heredoc->key, (char *)heredoc->val);
}

static char *heredoc_read(t_var *mshell, const char *delimiter, t_command *cmd)
{
	char	*line;
	int heredoc_fd;
	char *heredoc_file;
	static int count = 0;
	char *suffix;
	char *prefixed_name;

	suffix = ft_itoa(count++);
	heredoc_file = file_gen_name("heredoc_", R_OK | W_OK);
	prefixed_name = str_join(heredoc_file, suffix);
	if (!prefixed_name)
		return (ft_perror("file_gen_name"), NULL);
	heredoc_fd = open(prefixed_name, O_CREAT | O_RDWR | O_TRUNC, 0660);
	if (heredoc_fd == -1)
		return (ft_perror("open"), free(prefixed_name), NULL);
	// heredoc_file = gen_heredocs(heredoc_file, heredoc_fd);
	// heredoc_fd = open(prefixed_name, O_CREAT | O_RDWR | O_TRUNC, 0660);
	// if (heredoc_fd == -1)
	// 	return (ft_perror("open"), free(prefixed_name), NULL);
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
	return (prefixed_name);
}

void unlink_heredocs(t_command *cmd)
{
	t_list *current;

	current = cmd->heredocs;
	while (current)
	{
		if (current->val && current->next != NULL)
		{
			printf("unlinking %s\n", (char *)current->val);
			unlink((char *)current->val);
		}
		else if (current->val && !current->next)
		{
			printf("last heredoc: %s\n", (char *)current->val);
			break ;
		}
		current = current->next;
	}
	cmd->heredocs = NULL;
}

void cleanup_heredocs(t_command *cmd)
{
	t_list *current;
	t_list *temp;

	current = cmd->heredocs;
	while (current)
	{
		temp = current;
		current = current->next;
		free(temp->key);
		free(temp->val);
		free(temp);
	}
}

int ms_heredoc(t_var *mshell, t_command *cmds)
{
	t_list	*current;
	char *delimiter;
	char *heredoc_name;


	cleanup_heredocs(cmds);
	current = cmds->args;
	while (current)
	{
		if (str_cmp(current->val, "<<") == 0)
		{
			delimiter = current->next->val;
			heredoc_name = heredoc_read(mshell, delimiter, cmds);
			if (heredoc_name)
				add_heredoc(cmds, delimiter, heredoc_name);
			if (current->next->next != NULL)
				unlink_heredocs(cmds);
			current = current->next;
		}
		current = current->next;
	}
	// unlink heredocs(cmds);
	// 	exec_heredoc_cmd(mshell, args);
	return (OK);
}

// int *gen_heredocs(char *heredoc_file, int count)
// {
// 	char *suffix;
// 	char *prefixed_name;
// 	int fd;
// 	char *base_name;

// 	count = 0;
// 	suffix = ft_itoa(count);
// 	base_name = file_gen_name("heredoc_", R_OK | W_OK);
// 	prefixed_name = str_join(base_name, suffix);
// 	if (!prefixed_name)
// 		return (ft_perror("file_gen_name"), NULL);
// 	fd = open(prefixed_name, O_CREAT | O_RDWR | O_TRUNC, 0660);
// 	if (fd == -1)
// 		return (ft_perror("open"), free(prefixed_name), NULL);
// 	heredoc_file = prefixed_name;
// 	return (fd);
// }

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
