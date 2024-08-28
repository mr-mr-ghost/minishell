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

#include "../minishell.h"

/* echo_command: prints all the subsequent string							  */
/* if 1 str - prints new line, returns 1									  */
/* if 2 strs - if str2 = "-n", returns 1, else prints str2, returns 1		  */
/* if more strs - prints all strings, adds new line if str2 = "-n", returns 1 */
char	*get_echo_value(t_env *env, char *line, int *start)
{
	char	*env_name;
	char	*env_value;
	char	*tmp;

	env_name = find_env_name(env, line + *start);
	if (!env_name)
		return (NULL);
	*start += ft_strlen(env_name);
	tmp = find_env_value(env, env_name);
	free(env_name);
	env_value = remove_quotes(tmp);
	free(tmp);
	return (env_value);
}

int	process_dollar(t_data *data, char *line, int *i)
{
	char	*env_value;

	(*i)++;
	if (line[*i] == '?')
		env_value = ft_itoa(data->exit_code);
	else if (line[*i] == '$')
		env_value = find_env_value(data->env, "SYSTEMD_EXEC_PID");
	else if (select_valid_env(data->env, line, *i))
		env_value = get_echo_value(data->env, line, i);
	else if (!select_valid_env(data->env, line, *i)
		&& (line[*i] || line[*i] == ' '))
		env_value = ft_strdup("");
	else
		env_value = ft_strdup("$");
	if (!env_value)
		return (err_msg("echo", NULL, "Memory allocation failure", 1));
	ft_putstr_fd(env_value, 1);
	free(env_value);
	(*i)--;
	return (0);
}

void	process_quotes(char *line, int *i, bool *quote)
{
	static char	tmp = 0;

	if (!tmp && !quotes_check(line + *i))
	{
		tmp = line[*i];
		if (line[*i] == '\'')
			*quote = true;
	}
	else if (*quote && line[*i] == '\'')
		*quote = false;
	else if (line[*i] != tmp)
		ft_putchar_fd(line[*i], 1);
	else if (line[*i] == tmp)
		tmp = 0;
}

int	print_echo(t_data *data, char *line)
{
	int		i;
	bool	quote;

	i = 0;
	quote = false;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			process_quotes(line, &i, &quote);
		else if (!quote && line[i] == '$' && line[i + 1] && line[i + 1] != ' ')
		{
			if (process_dollar(data, line, &i))
				return (1);
		}
		else if (line[i])
			ft_putchar_fd(line[i], 1);
		if (line[i] != '\0')
			i++;
	}
	return (0);
}

int	echo_command(t_data *data, t_token *token)
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
	if (print_echo(data, echo_token->value))
		return (1);
	if (!n_flag)
		printf("\n");
	return (0);
}
