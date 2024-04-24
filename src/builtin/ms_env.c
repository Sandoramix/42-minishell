/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:44:22 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/24 15:37:04 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_env(t_var *mshell)
{
	int		i;
	t_list	*env;

	i = 0;
	env = mshell->env;
	while (env)
	{
		printf("%s=%s\n", (char *)env->key, (char *)env->val);
		env = env->next;
	}
}

//int	main(int ac, char **av, char **envp)
//{
//	t_var			mshell;

//	(void)av;
//	(void)ac;
//	mshell = (t_var){0};
//	if (ac != 1)
//		return (pf_errcode(ERR_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
//	mshell._main.envp = envp;
//	ms_init(&mshell);
//	//ms_prompt(&mshell);
//	ms_env(&mshell);
//	return (cleanup(&mshell, true, 0));
//}
