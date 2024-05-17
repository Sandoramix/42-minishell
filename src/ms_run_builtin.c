/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/17 16:34:00 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_run_builtin(t_var *mshell, t_list *args)
{
	if (str_cmp(args->val, "export") == 0)
		return (ms_export(mshell, args));
	else if (str_cmp(args->val, "unset") == 0)
		return (ms_unset(mshell, args));
	else if (str_cmp(args->val, "echo") == 0)
		return (ms_echo(mshell, args));
	else if (str_cmp(args->val, "cd") == 0)
		return (ms_cd(mshell, args));
	else if (str_cmp(args->val, "pwd") == 0)
		return (ms_pwd(mshell, args), OK);
	else if (str_cmp(args->val, "env") == 0)
		return (ms_env(mshell, args), OK);
	else if (str_cmp(args->val, "history") == 0)
		return (ms_history(mshell), OK);
	else if (str_cmp(args->val, "exit") == 0)
		return (ms_exit(mshell, args));
	else
	{
		if (args)
			ft_perror(PROGNAME": Command not found: %s\n", args->val);
		return (KO);
	}
	return (OK);
}

void	parse_and_exec(t_var *mshell, char *input)
{
	t_list	*cmd_list;

	cmd_list = cmd_parse(input);
	expand_and_clear(mshell, cmd_list);
	if (cmd_list != NULL)
		ms_run_builtin(mshell, cmd_list);
	lst_free(&cmd_list, free);
}
