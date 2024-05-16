/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:36:35 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/16 18:42:14 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ms_pwd(t_var *mshell, t_list *args)
{
	if (!args || lst_size(args) > 1)
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		*mshell->status_code = 1;
		return (NULL);
	}
	sys_update_cwd(mshell);
	if (str_cmp((char *)args->val, "pwd") == 0)
		printf("%s\n", mshell->curr_path);
	else
		ft_fprintf(2, "%s: command not found\n", (char *)args->val);
	return (NULL);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_var			mshell;
// 	t_list			*args;

// 	(void)av;
// 	(void)ac;
// 	mshell = (t_var){0};
// 	args = cmd_parse("pwd");
// 	if (ac != 1)
// 		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	ms_pwd(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }

