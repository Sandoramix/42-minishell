/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_convert_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 10:59:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/12 11:09:03 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*recreate_env_string(t_list *variable)
{
	char	*res;

	res = NULL;
	res = str_freejoin(res, variable->key);
	if (!res)
		return (pf_errcode(E_MALLOC), NULL);
	res = str_freejoin(res, "=");
	if (!res)
		return (pf_errcode(E_MALLOC), ft_free(res), NULL);
	res = str_freejoin(res, variable->val);
	if (!res)
		return (pf_errcode(E_MALLOC), ft_free(res), NULL);
	return (res);
}


char	**lst_env_to_mtx(t_var *mshell)
{
	const int	lstsize = lst_size(mshell->env);
	char		**res;
	int			i;
	t_list		*lst;

	if (!mshell->env)
		return (NULL);
	res = ft_calloc(lstsize + 1, sizeof(char *));
	if (!res)
		return (pf_errcode(E_MALLOC), NULL);
	i = -1;
	lst = mshell->env;
	while (++i < lstsize)
	{
		if (!lst->_hidden)
		{
			res[i] = recreate_env_string(lst);
			if (!res[i])
				return (str_freemtx(res), NULL);
		}
		lst = lst->next;
	}
	return (res);
}
