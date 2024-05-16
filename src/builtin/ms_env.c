/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:44:22 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/16 18:42:14 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_env(t_var *mshell, t_list *args)
{

	if (!args)
		return ;
	if (str_cmp((char *)args->val, "env") != 0)
	{
		ft_perror("command not found: %s\n", (char *)args->val);
		*(mshell->status_code) = 127;
		return ;
	}
	if (lst_size(args) > 1)
	{
		ft_perror("env: '%s': No such file or directory\n", args->next->val);
		*(mshell->status_code) = 127;
		return ;
	}
	args = mshell->env;
	while (args)
	{
		if (args->_hidden == false)
			printf("%s=%s\n", (char *)args->key, (char *)args->val);
		args = args->next;
	}
}

// int main(int ac, char **av, char **envp)
// {
// 	t_var			mshell;
// 	t_list *args;

// 	(void)av;
// 	(void)ac;
// 	mshell = (t_var){0};
// 	if (ac != 1)
// 		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	args = cmd_parse("env");
// 	ms_env(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }
