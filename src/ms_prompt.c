/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/24 19:24:03 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	ms_handleinput(t_var *mshell, char **input)
{
	t_list	*cmd_list;
	int		status_code;

	if (!ms_closing_quotes_check(*input))
		return (pf_errcode(E_SYNTAX), free(*input), g_set_status(1), KO);
	cmd_list = cmd_parse(mshell, input);
	free(*input);
	if (!cmd_list)
		return (lst_free(&cmd_list, free), KO);
	if (!ms_token_syntax_check(cmd_list))
		return (pf_errcode(E_SYNTAX), g_set_status(1),
			lst_free(&cmd_list, free), KO);
	ms_exec_commands(mshell, cmd_list);
	while (wait(&status_code) != -1)
	{
		if (WIFEXITED(status_code))
			g_set_status(WEXITSTATUS(status_code));
		else if (WIFSIGNALED(status_code))
			handle_child_ret_sig(WTERMSIG(status_code));
	}
	if (g_status == 131)
		ft_printf("Quit\n");
	return (OK);
}

t_state	ms_exec_script(t_var *mshell)
{
	int			file_fd;
	int			i;
	char		*line;

	i = -1;
	file_fd = file_open(mshell->_main.argv[1], O_RDONLY);
	if (file_fd == -1)
		cleanup(mshell, true, 127);
	mshell->script_file = ft_readfile(file_fd, false);
	while (g_status == 0 && mshell->script_file && mshell->script_file[++i])
	{
		signal(SIGINT, handle_sig);
		signal(SIGQUIT, SIG_IGN);
		line = mshell->script_file[i];
		if (!line || str_startswith(line, "#") || str_isblank(line))
			continue ;
		line = str_dup(mshell->script_file[i]);
		if (!line)
			return (KO);
		add_history(line);
		add_history_line(mshell, line);
		ms_handleinput(mshell, &line);
		mshell->all_cmds = NULL;
	}
	return (OK);
}

t_state	ms_prompt(t_var *mshell)
{
	char	*input;

	while (42)
	{
		signal(SIGINT, handle_sig);
		signal(SIGQUIT, SIG_IGN);
		input = readline(PROG_PROMPT " ");
		if (!input)
			break ;
		if (!str_isblank(input))
		{
			add_history(input);
			add_history_line(mshell, input);
			ms_handleinput(mshell, &input);
		}
		mshell->all_cmds = NULL;
	}
	ft_fprintf(2, "exit\n");
	return (OK);
}
