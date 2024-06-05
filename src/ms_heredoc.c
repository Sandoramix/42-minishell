/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/05 09:44:59 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// cat << EOF

static t_list *heredoc_read(t_var *mshell, char *delimiter)
{
	char	*line;
	//char *heredoc_content;


	while (42)
	{
		line = readline("heredoc > ");
		if (!line || !str_cmp(line, delimiter))
		{
			free(line);
			break ;
		}
		//lst_addnew_tail(&mshell->heredoc, lst_addnew_head(&mshell->heredoc, line, NULL), NULL);
		lst_addnode_tail(&mshell->heredoc, lst_addnew_head(&mshell->heredoc, line, NULL));
		// printf("list heredoc: %s\n", (char *)mshell->heredoc->val);
		// printf("heredoc: %s\n", line);
	}
	//print_history(mshell->heredoc);
	return (mshell->heredoc);
}

// static void heredoc_child(t_var *mshell, t_list *args, int *fd)
// {
// 	char *line;

// 	close(fd[0]);

// 	while (42)
// 	{
// 		line = readline("heredoc > ");
// 		if (!line || !str_cmp(line, args->next->val))
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd[1], line, str_ilen(line));
// 		write(fd[1], "\n", 1);
// 		free(line);
	
// 	}
// 	close(fd[1]);
// }

// static int heredoc_parent(t_list *args, int *fd)
// {
// 	int exit_code;

// 	wait(&exit_code);
// 	close(fd[1]);
// 	if (WIFEXITED(exit_code))
// 	{
// 		exit_code = WEXITSTATUS(exit_code);
// 		if (exit_code == 1)
// 			pf_errcode(ERR_EXECVE);
// 		else
// 			dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 	}
// 	return (exit_code);
// }

// int exec_heredoc_cmd(t_var *mshell, t_list *args)
// {
// 	pid_t	pid;
// 	int		fd[2];
// 	t_list *current;
// 	char *cmd_path;

// 	cmd_path = sys_findcmdpath(mshell->cmds_paths, args->val);
// 	if (pipe(fd) == -1)
// 		return (pf_errcode(ERR_PIPE), KO);
// 	pid = fork();
// 	if (pid == -1)
// 		return (pf_errcode(ERR_FORK), KO);
// 	if (pid == 0)
// 	{
// 		close(fd[1]);
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 		if (!execve(cmd_path, (char **)args->next->val, mshell->_main.envp))
// 			return (pf_errcode(ERR_EXECVE), KO);
// 	}
// 	else
// 	{
// 		close(fd[0]);
// 		current = mshell->heredoc;
// 		while (current)
// 		{
// 			write(fd[1], current->val, str_ilen(current->val));
// 			write(fd[1], "\n", 1);
// 			current = current->next;
// 		}
// 		close(fd[1]);
// 		waitpid(pid, NULL, 0);
// 	}
// 	return (OK);
// }

int exec_heredoc_cmd(t_var *mshell, t_list *args)
{
	int fd[2];
	pid_t pid;
	t_list *current;
	char **cmd;
	char *cmd_path;

	if (pipe(fd) == -1)
		return (pf_errcode(ERR_PIPE), KO);
	pid = fork();
	if (pid == -1)
		return (pf_errcode(ERR_FORK), KO);
	if (pid == 0)
	{
		close(fd[1]);
		//dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		cmd = ft_lst_to_array(args);
		cmd_path = sys_findcmdpath(mshell->cmds_paths, cmd[0]);
		
		// if (!ms_exec_cmd(mshell, args))
		// 	return (pf_errcode(ERR_EXECVE), KO);
		execve(cmd_path, cmd, mshell->_main.envp);
	}
	else
	{
		close(fd[0]);
		printf("loooô	\n");
		current = mshell->heredoc;
		print_history(current);
		while (current)
		{
			write(fd[1], current->val, str_ilen(current->val) + 1);
			write(fd[1], "\n", 1);
			current = current->next;
		}
		close(fd[1]);
		wait(NULL);
	}
	return (OK);
}

// need to execute the input with the heredoc

int ms_heredoc(t_var *mshell, t_list *args)
{
	t_list	*current;
	char *delimiter;

	current = args;
	while (current)
	{
		if (str_equals(current->val, "<<"))
		{
			delimiter = current->next->val;
			heredoc_read(mshell, delimiter);
			break ;
		}
		current = current->next;
	}
	if (mshell->heredoc)
	{
		exec_heredoc_cmd(mshell, args);
		lst_free(&mshell->heredoc, free);
	}
	//printf("arg 0: %s\n", (char *)args->val);
	return (OK);
}

// int exec_here_cmd(t_var *mshell, t_list *args)
// {
// 	int pid;
// 	int fd[2];
	
// 	if (pipe(fd) == -1)
// 		return (pf_errcode(ERR_PIPE), KO);
// 	pid = fork();
// 	if (pid == 0)
// 		heredoc_child(mshell, args, fd);
// 	else
// 		heredoc_parent(args, fd);
// 	return (OK);
// }

/*
cat << EOF
1. generare il o i file heredoc e aprirli in lettura
2. appena trovi >> allora leggi l'input e lo salvi

*/
