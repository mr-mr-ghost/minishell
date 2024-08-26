/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 15:10:20 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	print_tokens(t_token *token)
{
	while (token)
	{
		ft_printf("%s\n", token->value);
		ft_printf("type: %d\n\n", token->type);
		token = token->next;
	}
}

char	*shorten_path(char *path)
{
	int		slash_index;
	int		slash_count;
	char	*line;
	int		i;

	if (!path)
		return (NULL);
	slash_count = 0;
	i = ft_strlen(path) - 1;
	while (i >= 0)
	{
		if (path[i] == '/')
		{
			slash_count++;
			if (slash_count <= 2)
				slash_index = i;
		}
		i--;
	}
	line = ft_substr(path, slash_index, ft_strlen(path) - slash_index);
	free(path);
	return (line);
}

char	*generate_prompt(t_env *env)
{
	char	*line;
	char	*tmp;
	char	*prompt;
	char	*env_path;

	line = ft_strdup("\033[1;34mMinishell:\033[0m \033[1;32m~");
	env_path = find_env_value(env, "PWD");
	env_path = shorten_path(env_path);
	if (env_path)
	{
		tmp = ft_strjoin(line, env_path);
		free(line);
		free(env_path);
		prompt = ft_strjoin(tmp, "\033[0m> ");
		free(tmp);
	}
	else
		prompt = ft_strjoin(line, "\033[0m> ");
	return (prompt);
}

char	*read_line(t_env *env)
{
	char	*line;
	char	*prompt;
	char	*tmp;

	prompt = generate_prompt(env);
	if (!prompt)
		return (NULL);
	line = readline(prompt);
	free(prompt);
	if (!line)
	{
		g_sig.exit_status = 130;
		return (NULL);
	}
	tmp = line;
	line = ft_strdup(line);
	free(tmp);
	if (!line)
		return (NULL);
	return (line);
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->token = NULL;
	data->env = NULL;
	data->exit_code = 0;
	data->loop = false;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	disable_sigquit();
	init_data(&data);
	init_env(&data, envp);
	while (!data.loop)
	{
		sig_init();
		data.line = read_line(data.env);
		if (!data.line)
			break ;
		add_history(data.line);
		if (token_split(&data))
			break ;
//		print_tokens(data.token);
		if (g_sig.exit_status)
			data.exit_code = g_sig.exit_status;
		process_n_exec(&data);
		free_tokens(&data);
	}
	rl_clear_history();
	free_env(data.env);
	free_env(data.secret_env);
	ft_printf("exit\n");
	return (data.exit_code);
}
