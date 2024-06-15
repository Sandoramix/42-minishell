/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/15 14:13:08 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ms_handleinput(t_var *mshell, char *input)
{
	t_list	*cmd_list;
	int		status_code;

	cmd_list = cmd_parse(mshell, input);
	if (!cmd_list)
		return (lst_free(&cmd_list, free), input);
	if (!ms_closing_quotes_check(input) || !ms_token_syntax_check(cmd_list))
		return (pf_errcode(E_SYNTAX), lst_free(&cmd_list, free), NULL);
	ms_exec_commands(mshell, cmd_list);
	while (wait(&status_code) != -1)
		*(mshell->status_code) = (t_uchar)status_code;
	return (input);
}

void	ms_prompt(t_var *mshell)
{
	char	*input;

	while (42)
	{
		input = readline(PROG_PROMPT " ");
		if (!input)
			break ;
		if (str_ilen(input) > 0)
			mshell->last_input = str_trim(input, " \t\v\r\n");
		if (str_ilen(mshell->last_input) > 0)
		{
			free(mshell->last_input);
			mshell->last_input = input;
			add_history(input);
			add_cmd_history(mshell, input);
			ms_handleinput(mshell, input);
		}
		else
			free(mshell->last_input);
		free(input);
		mshell->last_input = NULL;
		mshell->all_cmds = NULL;
	}
}
