/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:36:40 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/07 10:52:02 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void unlink_heredocs(t_command *cmd)
{
	t_list *current;

	current = cmd->heredocs;
	while (current)
	{
		if (current->val)
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

char *gen_heredocs(int count)
{
	char *suffix;
	char *prefixed_name;
	char *heredoc_file;

	suffix = ft_itoa(count);
	heredoc_file = file_gen_name("heredoc_", R_OK | W_OK);
	prefixed_name = str_join(heredoc_file, suffix);
	if (!prefixed_name)
		return (ft_perror("file_gen_name"), NULL);
	free(suffix);
	free(heredoc_file);
	return (prefixed_name);
}
