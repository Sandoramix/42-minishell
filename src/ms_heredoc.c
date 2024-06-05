/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 20:32:32 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/05 10:38:17 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// cat << EOF

static char *heredoc_read(t_var *mshell, char *delimiter)
{
	char	*line;
	int heredoc_fd;
	char *heredoc_file;

	heredoc_file = file_gen_name("heredoc", R_OK | W_OK);
	if (!heredoc_file)
		return (ft_perror("file_gen_name"), NULL);
	heredoc_fd = open(heredoc_file, O_CREAT | O_RDWR | O_TRUNC, 0600);
	//printf("heredoc fs: %d\n", heredoc_fd);
	if (heredoc_fd == -1)
		return (ft_perror("open"), free(mshell->heredoc_file), NULL);
	free(mshell->heredoc_file);
	mshell->heredoc_file = heredoc_file;
	while (42)
	{
		line = readline("heredoc > ");
		if (!line || !str_cmp(line, delimiter))
		{
			free(line);
			break ;
		}
		write(heredoc_fd, line, str_ilen(line));
		write(heredoc_fd, "\n", 1);
		free(line);
	}
	close(heredoc_fd);
	printf("heredoc file: %s\n", heredoc_file);
	return (heredoc_file);
}


char **ft_lst_to_cmd_array(t_list *cmd)
{
    int count;
    t_list *current; 
    int skip_next;

	count = 0;
	current = cmd;
	skip_next = 0;
    while (current)
    {
        if (skip_next)
            skip_next = 0;
        else if (str_equals(current->val, "<<"))
            skip_next = 1;
        else
            count++;
        current = current->next;
    }

    char **argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return NULL;

    current = cmd;
    int i = 0;
    skip_next = 0;
    while (current)
    {
        if (skip_next)
            skip_next = 0;
        else if (str_equals(current->val, "<<"))
            skip_next = 1;
        else
            argv[i++] = current->val;
        current = current->next;
    }
    argv[count] = NULL;
    return argv;
}

int exec_heredoc_cmd(t_var *mshell, t_list *args)
{
	int fd;
	pid_t pid;
	char **cmd;
	char *cmd_path;

	fd = open(mshell->heredoc_file, O_RDONLY);
	if (fd == -1)
		return (pf_errcode(ERR_SYNTAX), KO);
	pid = fork();
	if (pid == -1)
	{
		close(fd);
		return (pf_errcode(ERR_FORK), KO);
	}
	if (pid == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		cmd = ft_lst_to_cmd_array(args);
		cmd_path = sys_findcmdpath(mshell->cmds_paths, cmd[0]);
		if (!cmd_path)
			return (ft_perror("Command not found"), KO);
		execve(cmd_path, cmd, mshell->_main.envp);
		ft_perror("execve");
		exit(KO);
	}
	else
	{
		close(fd);
		wait(NULL);
	}
	return (OK);
}

int ms_heredoc(t_var *mshell, t_list *args)
{
	t_list	*current;
	char *delimiter;
	//char *heredoc_name;

	//heredoc_name = NULL;
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
	if (mshell->heredoc_file)
	{
		exec_heredoc_cmd(mshell, args);
		unlink(mshell->heredoc_file);
		free(mshell->heredoc_file);
		mshell->heredoc_file = NULL;
	}
	//printf("arg 0: %s\n", (char *)args->val);
	return (OK);
}

/*
cat << EOF
- se viene eseguito come primo comando, fallisce sempre. altrimenti funzia quando abbiamo un solo fd.

*/
