/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/05 14:29:27 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*expand_and_clear(t_list *args)
{
	t_list	*arg;

	arg = args;
	while (arg)
	{
		dbg_printf(COLOR_CYAN"ex_clear of [%s]\n"CR, arg->val);
		// TODO add expansion
		if (arg->val && !str_clearquotes((char **)&arg->val))
			return (lst_free(&args, free));
		dbg_printf(COLOR_CYAN"\t[%s]\n"CR, arg->val);
		arg = arg->next;
	}
	return (args);
}

char	**expand_and_clear_mtx(char **args)
{
	int		i;

	i = -1;
	while (args && args[++i])
	{
		dbg_printf(COLOR_CYAN"ex_clear of [%s]\n"CR, args[i]);
		// TODO add expansion
		if (args[i] && !str_clearquotes(&(args[i])))
			return (str_freemtx(args));
		dbg_printf(COLOR_CYAN"\t[%s]\n"CR, args[i]);
	}
	return (args);
}