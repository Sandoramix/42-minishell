/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:36:35 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/22 10:33:00 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*ms_pwd(t_var *mshell, t_list *args)
{
	if (!args || lst_size(args) > 1)
		return (g_set_status(1), ft_perror("pwd: too many arguments\n"), NULL);
	ms_update_cwd(mshell);
	ft_printf("%s\n", mshell->curr_path);
	return (NULL);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_var			mshell;
// 	t_list			*args;

// 	(void)av;
// 	(void)ac;
// 	mshell = (t_var){0};
// 	args = cmd_parse(mshell, "pwd");
// 	if (ac != 1)
// 		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);
// 	ms_pwd(&mshell, args);
// 	return (cleanup(&mshell, true, 0));
// }
