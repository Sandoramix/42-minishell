/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:44:22 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/22 10:54:38 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	ms_env(t_var *mshell, t_list *args)
{
	if (lst_size(args) > 1)
		return (g_set_status(1),
			ft_perror("env: No arguments/options are allowed\n"), KO);
	args = mshell->env;
	while (args)
	{
		if (args->_hidden == false)
			ft_printf("%s=%s\n", (char *)args->key, (char *)args->val);
		args = args->next;
	}
	return (OK);
}

// int main(int ac, char **av, char **envp)
// {
// 	t_var			mshell;
// 	t_list *args;

// 	(void)av;
// 	(void)ac;
// 	mshell = (t_var){0};
// 	if (ac != 1)
// 		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	args = cmd_parse(mshell, "env");
// 	ms_env(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }
