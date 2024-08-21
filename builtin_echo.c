/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:09:32 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/19 12:09:32 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* echo_command: prints all the subsequent string							  */
/* if 1 str - prints new line, returns 1									  */
/* if 2 strs - if str2 = "-n", returns 1, else prints str2, returns 1		  */
/* if more strs - prints all strings, adds new line if str2 = "-n", returns 1 */
bool	select_valid_env(t_env *env, char *line, int start)
{
	char	*env_name;

	env_name = find_env_name(env, line + start);
	if (!env_name)
		return (false);
	if (valid_env_name(env, env_name))
	{
		free(env_name);
		return (true);
	}
	free(env_name);
	return (false);
}

char	*get_echo_value(t_env *env, char *line, int *start)
{
	char	*env_name;
	char	*env_value;
	char	*tmp;
	int		i;

	i = *start;
	while (line[i] && line[i] != ' ' && line[i] != '$')
		i++;
	env_name = ft_substr(line, *start, i - *start);
	if (!env_name)
		return (NULL);
	tmp = find_env_value(env, env_name);
	free(env_name);
	env_value = remove_quotes(tmp);
	*start = i;
	return (env_value);
}

void	process_dollar(char *line, t_env *env, int *i)
{
	char	*env_value;

	(*i)++;
	if (line[*i] == '?')
		env_value = ft_itoa(g_sig.exit_status);
	else if (line[*i] == '$')
		env_value = find_env_value(env, "SYSTEMD_EXEC_PID");
	else if (select_valid_env(env, line, *i))
		env_value = get_echo_value(env, line, i);
	else if (!select_valid_env(env, line, *i) && (line[*i] || line[*i] == ' '))
		env_value = ft_strdup("");
	else
		env_value = ft_strdup("$");
	ft_putstr_fd(env_value, 1);
	free(env_value);
	if (line[*i] == ' ')
		(*i)--;
}

void	print_echo(char *line, t_env *env)
{
	int		i;
	bool	quote;

	i = 0;
	quote = false;
	while (line[i])
	{
		if (line[i] == '\'')
			quote = true;
		else if (!quote && line[i] == '$')
			process_dollar(line, env, &i);
		else if (line[i] != '\"')
			ft_putchar_fd(line[i], 1);
		i++;
	}
}

int	echo_command(t_token *token, t_env *env)
{
	int		n_flag;
	t_token	*echo_token;

	if (!token->next || token->next->type != ARG)
	{
		printf("\n");
		return (0);
	}
	echo_token = token->next;
	n_flag = 0;
	if (!ft_memcmp(echo_token->value, "-n", 3))
	{
		if (!echo_token->next || echo_token->next->type != ARG)
			return (0);
		echo_token = echo_token->next;
		n_flag = 1;
	}
	print_echo(echo_token->value, env);
	if (!n_flag)
		printf("\n");
	return (0);
}
