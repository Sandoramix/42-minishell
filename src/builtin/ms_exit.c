/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:21:48 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/22 11:42:00 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_exit(t_var *mshell, char **args)
{
	unsigned char code;
	int mtx_len;
	mtx_len = ft_memmtxlen(args);
	
	printf("exit\n");
	if (mtx_len > 0)
		code = (unsigned char)ft_atoi(args[0]);	
	if (mtx_len > 1)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		*mshell->status_code = code;
		return ;
	}
	if (mtx_len == 0)
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

