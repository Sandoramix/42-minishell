/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parsing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 15:46:30 by odudniak          #+#    #+#             */
/*   Updated: 2024/05/31 15:00:00 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSING_H
# define MS_PARSING_H
# include <libft.h>
//---------------------------PARSING-------------------------------------------

// TODO: document this sh*t

typedef enum e_cmd_parseswitch
{
	CMDP_SPACE,
	CMDP_QUOTE,
	CMDP_TOKEN,
	CMDP_WORD
}	t_cmdp_switch;

/**
 * @brief Structure that contains the splitted string + information about it
 * @param tmp the newly created string.
 * @param edge start/end idx of the original string
 * end is for: QUOTE handling
 * start is for: WORD/TOKEN handling (updated on SPACE check)
 * @param i end idx of the original string
 */
typedef struct e_cmdp_arg
{
	t_list		*res;

	char		*tmp;
	int			edge;
	int			i;

	char		*str;
}	t_cmdp_arg;

typedef enum e_argtype
{
	A_DEFAULT,
	A_TOKEN
}	t_argtype;

/**
 * @brief Parse the string of a command.
 * @param command command's string to parse
 * @return List of args parsed.
 */
t_list	*cmd_parse(t_var *mshell, char *command);
t_list	*cmd_expand_clear(t_var *mshell, t_list *args);

char	*str_clearquotes(char **str);

bool	str_isvariable(char *s);

int		str_var_ending_idx(char *s, int start);

char	*arg_update(t_var *mshell, char **arg, int *d_idx, bool in_heredoc);

//---------------------------SYNTAX CHECKS--------------------------------------
bool	ms_closing_quotes_check(char *s);

bool	ms_token_syntax_check(t_list *args);

#endif
