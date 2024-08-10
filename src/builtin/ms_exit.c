/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:21:48 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 08:55:20 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ms_exit_parsecode(t_var *mshell, t_list *args)
{
	long			*orig_code;

	orig_code = strict_atol(args->next->val);
	if (!orig_code)
	{
		setstatus(mshell, 2);
		ft_perror("exit: %s: numeric argument required\n", args->next->val);
		cleanup(mshell, true, mshell->statuscode);
		return (KO);
	}
	else
		setstatus(mshell, *orig_code);
	return (free(orig_code), OK);
}

t_state	ms_exit(t_var *mshell, t_list *args)
{
	const int	lst_len = lst_size(args);

	if (lst_size(mshell->all_cmds) == 1 && mshell->_main.argc == 1)
		ft_perror("exit\n");
	if (lst_len > 1)
		ms_exit_parsecode(mshell, args);
	if (lst_len > 2)
		return (setstatus(mshell, 1),
			ft_perror("exit: too many arguments\n"), KO);
	cleanup(mshell, true, mshell->statuscode);
	return (OK);
}
