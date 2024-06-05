/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/05 12:36:27 by odudniak         ###   ########.fr       */
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
		return (pf_errcode(ERR_SYNTAX), lst_free(&cmd_list, free), NULL);
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
		input = readline(PROMPT " ");
		if (!input)
			break ;
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
	cleanup(mshell, true, *mshell->status_code);
}
