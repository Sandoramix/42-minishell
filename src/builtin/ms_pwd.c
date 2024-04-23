/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:36:35 by marboccu          #+#    #+#             */
/*   Updated: 2024/04/23 15:22:18 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <errno.h>

void ms_init_curr_path(t_var *mshell)
{
	char *init_path;

	init_path = getenv("PWD");
	if (init_path != NULL)
		mshell->curr_path = str_dup(init_path);
	else 
		mshell->curr_path = ft_calloc(10, sizeof(char));
}

char *ms_get_cwd(size_t size)
{
	char *curr_path;

	curr_path = ft_calloc(size, sizeof(char));
	if (!curr_path)
		return (NULL);
	while (getcwd(curr_path, size) == NULL)
	{
		if (errno == ERANGE)
		{
			size *= 2;
			char *new_buffer = realloc(curr_path, size * sizeof(char));
			if (!new_buffer)
			{
				free(curr_path);
				return (NULL);
			}
			curr_path = new_buffer;
		}
		else
		{
			free(curr_path);
			return (NULL);
		}
	}
	return (curr_path);
}

void ms_pwd(t_var *mshell)
{
	char *curr_path;

	if (mshell->curr_path)
	{
		free(mshell->curr_path);
		mshell->curr_path = NULL;
	}
	ms_init_curr_path(mshell);
	if (!mshell->curr_path)
	{
		pf_errcode(ERR_PATH_LOAD);
		cleanup(mshell, true, 1);
		return ;
	}
	curr_path = ms_get_cwd(10);
	if (!curr_path)
	{
		pf_errcode(ERR_PATH_LOAD);
		free(curr_path);
		cleanup(mshell, true, 1);
		return ;
	}
	free(mshell->curr_path);
	mshell->curr_path = curr_path;
	printf("%s\n", mshell->curr_path);
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
//	ms_pwd(&mshell);
//	return (cleanup(&mshell, true, 0));
//}
