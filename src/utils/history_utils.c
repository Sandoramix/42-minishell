/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:48:20 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/12 10:31:59 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_history(t_list *history)
{
	t_list	*current;
	int		i;

	i = 1;
	current = history;
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
}

void	add_cmd_history(t_var *mshell, char *cmd)
{
	t_list	*lst;
	char	*cmd_copy;

	cmd_copy = str_dup(cmd);
	if (!cmd_copy)
	{
		pf_errcode(E_MALLOC);
		free(cmd);
		cleanup(mshell, true, 1);
	}
	lst = lst_addnew_tail(&mshell->history, cmd_copy, NULL);
	if (!lst)
	{
		pf_errcode(E_MALLOC);
		free(cmd);
		free(cmd_copy);
		cleanup(mshell, true, 1);
	}
}
