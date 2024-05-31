/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/31 12:49:43 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_run_builtin(t_var *mshell, t_list *args, bool tofork)
{
	(void)tofork;
	if (str_equals(args->val, "export"))
		return (ms_export(mshell, args));
	else if (str_equals(args->val, "unset"))
		return (ms_unset(mshell, args));
	else if (str_equals(args->val, "echo"))
		return (ms_echo(mshell, args));
	else if (str_equals(args->val, "cd"))
		return (ms_cd(mshell, args));
	else if (str_equals(args->val, "pwd"))
		return (ms_pwd(mshell, args), OK);
	else if (str_equals(args->val, "env"))
		return (ms_env(mshell, args), OK);
	else if (str_equals(args->val, "history"))
		return (ms_history(mshell, args), OK);
	else if (str_equals(args->val, "exit"))
	{
		if (!tofork || lst_gettail(mshell->all_cmds)->val == args)
			return (ms_exit(mshell, args));
	}
	else
		ms_exec_cmd(mshell, args);
	return (OK);
}

bool	syntax_check(t_list *args)
{
	const char	*known_tokens[] = {"<", "<<", ">", ">>", "|", NULL};
	int			i;

	while (args)
	{
		if (args->type == A_TOKEN)
		{
			i = 0;
			while (known_tokens[i] && str_cmp(args->val, known_tokens[i]) != 0)
				i++;
			if (i == 5)
				return (false);
			if ((args->prev && args->prev->type == A_TOKEN)
				|| (args->next && args->next->type == A_TOKEN))
				return (false);
			if (!str_cmp("<<", args->val) && !args->next)
				return (false);
			if (str_equals(args->val, "|") && (!args->prev || !args->next))
				return (false);
		}
		args = args->next;
	}
	return (true);
}

bool	find_matching_final_quote(char *s)
{
	int		edge;
	int		i;

	i = -1;
	while (s[++i])
	{
		if (chr_isquote(s[i]))
		{
			edge = chr_quoteclose_idx(s, i);
			if (edge == -1)
				return (false);
			i = edge;
		}
	}
	return (true);
}

bool	is_builtin(char *cmd)
{
	const char	*valid[] = {"export", "unset", "echo", "cd", "pwd",
		"env", "history", "exit", NULL};
	int			i;

	i = -1;
	while (valid[++i])
		if (str_equals(cmd, (char *)valid[i]))
			return (true);
	return (false);
}

t_list	*lst_split_bystrval(t_list *all, char *val)
{
	t_list	*split;
	t_list	*tmp;
	t_list	*next;

	split = NULL;
	tmp = all;
	while (all)
	{
		next = all->next;
		if (next && str_equals(next->val, val))
		{
			next = all->next->next;
			lst_delnode(&all->next, free);
			all->next = NULL;
			if (!lst_addnew_tail(&split, tmp, NULL))
				return (lst_free(&split, free), lst_free(&next, free));
			tmp = next;
		}
		all = next;
	}
	if (!split || tmp)
		lst_addnew_tail(&split, tmp, NULL);
	return (split);
}

void	*ms_run_commands(t_var *mshell, t_list *all)
{
	t_list	*pipelined;
	int		size;
	t_list	*command;

	mshell->all_cmds = lst_split_bystrval(all, "|");
	if (!mshell->all_cmds)
		return (NULL);
	pipelined = mshell->all_cmds;
	size = lst_size(pipelined);
	dbg_printf("TOTAL LIST SIZE: %d\n", size);
	while (pipelined)
	{
		command = pipelined->val;
		ms_run_builtin(mshell, command, size > 1);
		pipelined->val = NULL;
		lst_free(&command, free);
		mshell->all_cmds = pipelined->next;
		free(pipelined);
		pipelined = mshell->all_cmds;
	}
	return (lst_free(&mshell->all_cmds, free), NULL);
}

void	*parse_and_exec(t_var *mshell, char *input)
{
	t_list	*cmd_list;
	bool	syntax;
	bool	quotes;
	int		status_code;

	cmd_list = cmd_parse(mshell, input);
	if (!cmd_list)
		return (lst_free(&cmd_list, free), input);
	quotes = find_matching_final_quote(input);
	if (!quotes)
		return (pf_errcode(ERR_SYNTAX), lst_free(&cmd_list, free), NULL);
	syntax = syntax_check(cmd_list);
	if (!syntax)
		return (pf_errcode(ERR_SYNTAX), lst_free(&cmd_list, free), NULL);
	ms_run_commands(mshell, cmd_list);
	while (wait(&status_code) != -1)
		*(mshell->status_code) = (t_uchar)status_code;
	return (input);
}
