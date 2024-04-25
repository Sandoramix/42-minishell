/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:21:48 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/25 18:56:03 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_exit(t_var *mshell, char **args)
{
	t_uchar		code;
	const int	mtx_len = str_mtxlen(args);

	ft_fprintf(2, "exit\n");
	if (mtx_len > 1)
		code = (t_uchar)ft_atoi(args[1]);
	if (mtx_len > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		*mshell->status_code = code;
		return ;
	}
	if (mtx_len == 1)
		cleanup(mshell, true, *mshell->status_code);
	cleanup(mshell, true, code);
}

//int	main(int ac, char **av, char **envp)
//{
//	t_var			mshell;

//	(void)ac;
//	mshell = (t_var){0};
	//if (ac != 1)
	//	return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
	//mshell._main.envp = envp;
	//ms_init(&mshell);
	//ms_prompt(&mshell);
	//pause();
//	ms_exit(&mshell, av + 1);
//	printf("status code: %d\n", *mshell.status_code);
//	return (cleanup(&mshell, true, 0));
//}
