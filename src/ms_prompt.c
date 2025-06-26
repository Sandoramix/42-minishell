/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 09:37:07 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	wait_for_all_cmds(t_var *mshell)
{
	int		status_code;

	if (waitpid(mshell->last_cmd_pid, &status_code, 0) != -1)
	{
		if (WIFEXITED(status_code))
			setstatus(mshell, WEXITSTATUS(status_code));
		else if (WIFSIGNALED(status_code))
			g_setlastsig(WTERMSIG(status_code));
	}
	track_lastsig(mshell);
	while (wait(&status_code) != -1)
		;
}

static t_state	ms_handleinput(t_var *mshell, char **input)
{
	t_list	*cmd_list;

	if (!ms_closing_quotes_check(*input))
		return (pf_errcode(E_SYNTAX), free(*input), setstatus(mshell, 2), KO);
	cmd_list = cmd_parse(mshell, input);
	free(*input);
	if (!cmd_list)
		return (lst_free(&cmd_list, free), KO);
	if (!ms_token_syntax_check(cmd_list))
		return (pf_errcode(E_SYNTAX), setstatus(mshell, 2),
			lst_free(&cmd_list, free), KO);
	ms_exec_commands(mshell, cmd_list);
	wait_for_all_cmds(mshell);
	if (mshell->statuscode == 131)
		ft_printf("Quit\n");
	return (OK);
}

static void	ms_resetsignals(void)
{
	g_setlastsig(INT_MIN);
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
}

t_state	ms_exec_script(t_var *mshell)
{
	const int	file_fd = file_open(mshell->_main.argv[1], O_RDONLY);
	int			i;
	char		*line;

	i = -1;
	if (file_fd == -1)
		cleanup(mshell, true, 127);
	mshell->script_file = ft_readfile(file_fd, false);
	track_lastsig(mshell);
	while (mshell->statuscode == 0
		&& mshell->script_file && mshell->script_file[++i])
	{
		ms_resetsignals();
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
		ms_resetsignals();
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
