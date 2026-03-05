/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 15:25:19 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/04 20:37:14 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_state(char c, int state)
{
	if (c == '\'' && state == OUT_QUOTE)
		state = IN_SQUOTE;
	else if (c == '\"' && state == OUT_QUOTE)
		state = IN_DQUOTE;
	else if ((c == '\'' && state == IN_SQUOTE)
		|| (c == '\"' && state == IN_DQUOTE))
		state = OUT_QUOTE;
	return (state);
}

char	*extract_var_name(char *str)
{
	int	len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (ft_substr(str, 0, len));
}

void	prepare_to_split(char *var_value, int state)
{
	int	i;

	if (state == OUT_QUOTE)
	{
		i = 0;
		while (var_value && var_value[i])
		{
			if (var_value[i] == ' ')
				var_value[i] = 1;
			i++;
		}
	}
}

char	*ft_getenv(char *name, char **env)
{
	int		i;
	int		len;
	char	*value;

	i = 0;
	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
		{
			value = &env[i][len + 1];
			return (ft_strdup(value));
		}
		i++;
	}
	return (NULL);
}
