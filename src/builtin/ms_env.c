/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:44:22 by marboccu          #+#    #+#             */
/*   Updated: 2024/08/10 08:55:04 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_state	ms_env(t_var *mshell, t_list *args)
{
	if (lst_size(args) > 1)
		return (setstatus(mshell, 1),
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
