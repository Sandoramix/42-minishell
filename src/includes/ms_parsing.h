/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 15:46:30 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/15 21:09:37 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSING_H
# define MS_PARSING_H
# include <libft.h>
//---------------------------PARSING-------------------------------------------

typedef enum e_argtype
{
	A_DEFAULT,
	A_TOKEN
}	t_argtype;

// TODO REMOVE
/**
# DEPRECATED
 * @brief Parse the string of a command.
 * It even handles internal quotes.
 * @param command command's string to parse
 * @return Array of args parsed.
 */
char	**cmd_parse(char *command);
char	**expand_and_clear_mtx(t_var *mshell, char **args);

t_list	*cmd_parse_new(char *raw);

char	*str_clearquotes(char **str);

t_list	*expand_and_clear(t_var *mshell, t_list *args);

bool	str_isvariable(char *s);

int		str_var_ending_idx(char *s, int start);
#endif
