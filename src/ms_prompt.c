/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/24 15:36:32 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	ms_prompt(t_var *mshell)
{
	char	*line;

	(void)mshell;
	line = readline(PROMPT" ");
	while (line)
	{
		if (DEBUG)
			printf("line: %s\n", line);
		free(line);
		line = readline(PROMPT" ");
	}
}
