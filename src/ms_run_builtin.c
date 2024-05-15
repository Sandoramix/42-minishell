/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/15 20:33:22 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_cmd_list(t_list *cmd_list)
{
	t_list	*current;

	current = cmd_list;
	while (current != NULL)
	{
		ft_printf("command: %s\n", (char *)current->val);
		current = current->next;
	}
}

void	print_history(t_list *history)
{
	t_list	*current;
	int		i;

	i = 1;
	current = history;
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
}

void	add_cmd_history(t_var *mshell, char *cmd)
{
	t_list	*lst;
	char	*cmd_copy;

	cmd_copy = str_dup(cmd);
	if (!cmd_copy)
	{
		pf_errcode(ERR_MALLOC);
		free(cmd);
		cleanup(mshell, true, 1);
	}
	lst = lst_addnew_tail(&mshell->history, cmd_copy, NULL);
	if (!lst)
	{
		pf_errcode(ERR_MALLOC);
		free(cmd);
		free(cmd_copy);
		cleanup(mshell, true, 1);
	}
}

int	ms_run_builtin(t_var *mshell, t_list *args)
{

	// print_cmd_list(lst);
	if (str_cmp(args->val, "export") == 0)
		return (ms_export(mshell, args));
	else if (str_cmp(args->val, "unset") == 0)
		return (ms_unset(mshell, args));
	// else if (str_cmp(args->val, "echo") == 0)
	// 	return (ms_echo(mshell, args));
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

	cmd_list = cmd_parse_new(input);
	expand_and_clear(mshell, cmd_list);
	// print_cmd_list(cmd_list);
	if (cmd_list != NULL)
		ms_run_builtin(mshell, cmd_list);
	lst_free(&cmd_list, free);
}

void	ms_loop(t_var *mshell)
{
	char	*input;

	input = readline(PROMPT " ");
	while (input)
	{
		if (str_ilen(input) > 0)
		{
			add_history(input);
			add_cmd_history(mshell, input);
			parse_and_exec(mshell, input);
		}
		free(input);
		input = readline(PROMPT " ");
	}
	cleanup(mshell, true, 0);
}
