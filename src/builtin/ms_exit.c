/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:21:48 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/11 14:17:10 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_exit(t_var *mshell, t_list *args)
{
	t_uchar		code;
	const int	lst_len = lst_size(args);

	ft_fprintf(2, "exit\n");
	if (lst_len > 1)
		code = (t_uchar)ft_atoi((char *)args->next->val);
	
		
	if (lst_len > 2)
	{
		ft_fprintf(2, "exit: too many arguments\n");
		*mshell->status_code = code;
		return (OK);
	}
	if (lst_len == 1)
		cleanup(mshell, true, *mshell->status_code);
	cleanup(mshell, true, code);
	return (OK);
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
