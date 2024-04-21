/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 12:40:27 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/21 12:46:29 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void ms_prompt(t_var *mshell)
{
	char *line;
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
