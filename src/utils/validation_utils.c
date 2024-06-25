/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:06:14 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/24 18:08:28 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_known_token(char *value)
{
	const char	*known_tokens[] = {"<", "<<", ">", ">>", "|", NULL};
	int			i;

	i = 0;
	while (known_tokens[i] && str_cmp(value, known_tokens[i]) != 0)
		i++;
	if (i == 5)
		return (false);
	return (true);
}

bool	is_builtin(char *cmd)
{
	const char	*valid[] = {"export", "unset", "echo", "cd", "pwd",
		"env", "history", "exit", NULL};

	return (str_array_includes((char **)valid, cmd));
}

bool	is_node_token(t_list *node, char *expected)
{
	if (!node)
		return (false);
	if (node->type != A_TOKEN)
		return (false);
	if (!expected)
		return (true);
	return (str_equals(node->val, expected) && node->type == A_TOKEN);
}
