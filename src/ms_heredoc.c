/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/05 23:43:59 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// cat << EOF

//  funzione x leggere e aprire tanti heredoc quanti sono i << nell'input (conc ciò che segue il <<)
// salvarsi il nodo << delimitatore in un nodo di t_list heredocs dentro t_command

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
}

static char *heredoc_read(const char *delimiter)
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
		write(heredoc_fd, line, str_ilen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	return (prefixed_name);
}

void cleanup_heredocs(t_command *cmd)
{
	t_list *current;
	t_list *temp;

	current = cmd->heredocs;
	while (current)
	{
		if (current->val)
			unlink((char *)current->val);
		temp = current;
		current = current->next;
		free(temp->key);
		free(temp->val);
		free(temp);
	}
	cmd->heredocs = NULL;
}

int ms_heredoc(t_var *mshell, t_command *cmds)
{
	t_list	*current;
	char *delimiter;
	char *heredoc_name;
	t_list *temp;
	(void)mshell;


	// cleanup_heredocs(cmds);
	while (cmds->heredocs)
	{
		temp = cmds->heredocs;
		//printf("heredoc: %s\n", (char *)temp->val);
		cmds->heredocs = cmds->heredocs->next;
		free(temp->key);
		free(temp->val);
		free(temp);
	}
	current = cmds->args;
	while (current)
	{
		if (str_cmp(current->val, "<<") == 0 && current->next)
		{
			delimiter = current->next->val;
			//printf("delimiter %s", delimiter);
			heredoc_name = heredoc_read(delimiter);

			if (heredoc_name)
				add_heredoc(cmds, delimiter, heredoc_name);
			current = current->next;
			//break ;
		}
		current = current->next;
	}
	printf("ciaooooo\n");
	current = cmds->heredocs;
	while (current)
    {
        printf("Delimiter: %s, File: %s\n", (char *)current->key, (char *)current->val);
        current = current->next;
    }
	// if (mshell->heredoc_file)
	// {
	// 	exec_heredoc_cmd(mshell, args);
	// 	unlink(mshell->heredoc_file);
	// 	free(mshell->heredoc_file);
	// 	mshell->heredoc_file = NULL;
	// }
	//printf("arg 0: %s\n", (char *)args->val);
	return (OK);
}

/*
cat << EOF
- se viene eseguito come primo comando, fallisce sempre. altrimenti funzia quando abbiamo un solo fd.

*/
