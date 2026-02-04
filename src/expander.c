/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 00:30:43 by rida-cos          #+#    #+#             */
/*   Updated: 2026/01/25 23:25:02 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char(char *str, char c)
{
	char	*new_str;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}

char	*expand_and_join(char *new_str, char *str, int *i, t_setup env)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	(*i)++;
	if (str[*i] == '?')
	{
		var_value = ft_strdup("0");
		(*i)++;
	}
	else
	{
		var_name = extract_var_name(&str[*i]);
		*i += ft_strlen(var_name);
		var_value = ft_getenv(var_name, env.envp);
		if (!var_value)
			var_value = ft_strdup("");
		free(var_name);
	}
	prepare_to_split(var_value, env.state); //lOGICA PARA RETOKENIZAR
	temp = ft_strjoin(new_str, var_value);
	free(new_str);
	free(var_value);
	return (temp);
}

char	*handler_expansion(char *str, t_setup env)
{
	int		i;
	char	*new_str;

	i = 0;
	env.state = OUT_QUOTE;
	new_str = ft_strdup("");
	while (str[i])
	{
		env.state = update_state(str[i], env.state);
		if (str[i] == '$' && env.state != IN_SQUOTE
			&& (ft_isalnum(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			new_str = expand_and_join(new_str, str, &i, env);
		else
		{
			new_str = append_char(new_str, str[i]);
			i++;
		}
	}
	free(str);
	return (new_str);
}

void	expander(t_token *tokens, t_setup env)
{
	while (tokens)
	{
		if (tokens->type == WORD && ft_strchr(tokens->value, '$'))
		{
			tokens->value = handler_expansion(tokens->value, env);
		}
		tokens = tokens->next;
	}
}
