/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_structs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 15:53:30 by odudniak          #+#    #+#             */
/*   Updated: 2024/08/10 09:08:41 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRUCTS_H
# define FT_STRUCTS_H
# include <libft.h>

typedef unsigned char	t_uchar;
typedef struct timeval	t_time;
typedef struct timeval	t_timeval;
typedef unsigned long	t_ulong;

/**
 * @brief `List` data structure
 * @param val value of the node.
 * @param key additional attribute which could be used as search param.
 * @param type Whatthetype of this variable :3
 * @param next pointer of the next node.
 * @param prev pointer of the previous node.
 * @param _is_lis (used on list of numbers) is the node a part of LIS ?
 * (Longest Increasing sequence).
 * @param _list_max LIS' max length that the current node can become.
 * @param _hidden Used in `ms_export`. If a variable
 * is declared without a value (export c), when running `export` it wont show
 * an equal sign + empty string (e.g. declare -x c)
 * @param _prevent_expansion Used for `A_TOKEN` types like `<<` (heredoc).
 * Tells if it should prevent the expansion because the next arg had a single
 * quote as first character.
 */
typedef struct s_list
{
	void				*val;
	void				*key;

	uint64_t			type;

	struct s_list		*next;
	struct s_list		*prev;

	bool				*_is_lis;
	int					*_lis_max;
	bool				_hidden;
	bool				_prevent_expansion;
}	t_list;

/**
 * @brief Struct for a point
 * @param x `x` coordinate
 * @param y `y` coordinate
 */
typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

/**
 * @brief Struct used for parsing commands from a string with correct arguments.
 * This struct holds information necessary for parsing commands from a string,
 * including handling quotes and counting arguments.
 *
 * @param escaped Boolean to check if the quote is escaped
 * (has a `\\` before it).
 * @param outside_quote Character of the current outer quote which determines
 * the whole argument.
 * @param inside_quote Character of the current internal quote.
 *
 * @param args_count Counter to track the number of arguments inside the string.
 *
 * @param start Index where the substring for the current argument should start.
 *
 * @param i Index of current position which traverses the string.
 *
 * @param res_idx Index for the final result to be returned.
 */
typedef struct s_cmdparse
{
	bool		escaped;

	char		outside_quote;
	char		inside_quote;

	int			args_count;

	int			start;
	int			i;

	int			res_idx;
}	t_cmdparse;

/**
 * @brief Status of the returned function (main included)
 * @param OK everything's ok.
 * @param KO an error occured.
 */
typedef enum e_state
{
	KO,
	OK,
	OK_EXIT
}	t_state;

#endif