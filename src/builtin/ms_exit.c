/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:21:48 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/19 21:48:39 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ms_exit_parsecode(t_var *mshell, t_list *args)
{
	long			*orig_code;

	orig_code = strict_atol(args->next->val);
	if (!orig_code)
	{
		*(mshell->status_code) = 2;
		ft_perror("exit: %s: numeric argument required\n", args->next->val);
		cleanup(mshell, true, *(mshell->status_code));
		return (KO);
	}
	else
		*(mshell->status_code) = (t_uchar)(*orig_code);
	return (free(orig_code), OK);
}

int	ms_exit(t_var *mshell, t_list *args)
{
	const int	lst_len = lst_size(args);

	if (lst_size(mshell->all_cmds) == 1)
		ft_perror("exit\n");
	if (lst_len > 1)
		ms_exit_parsecode(mshell, args);
	if (lst_len > 2)
	{
		ft_perror("exit: too many arguments\n");
		*mshell->status_code = 1;
		return (OK);
	}
	cleanup(mshell, true, *mshell->status_code);
	return (OK);
}

//int	main(int ac, char **av, char **envp)
//{
//	t_var			mshell;

//	(void)ac;
//	mshell = (t_var){0};
	//if (ac != 1)
	//	return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	//mshell._main.envp = envp;
	//ms_init(&mshell);
	//ms_prompt(&mshell);
	//pause();
//	ms_exit(&mshell, av + 1);
//	printf("status code: %d\n", *mshell.status_code);
//	return (cleanup(&mshell, true, 0));
//}
