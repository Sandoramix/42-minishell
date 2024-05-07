/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:44:22 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/05 19:49:37 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_env(t_var *mshell, t_list *args)
{

	if (!args && str_cmp((char *)args->val, "env") != 0)
	{
		ft_fprintf(2, "env: %s: No such file or directory\n", (char *)args->val);
		return ;
	}
	if (lst_size(args) > 1)
	{
		ft_fprintf(2, "env: %s: Permission denied\n", (char *)args->val);
		mshell->status_code = (t_uchar *)126;
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
// 	args = cmd_parse_new("env");
// 	ms_env(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }
