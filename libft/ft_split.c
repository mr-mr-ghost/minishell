/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 12:41:52 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/29 10:50:21 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	assign_words(char const *s, char c, char **arr, size_t word_count)
{
	size_t	i;
	size_t	j;
	size_t	start;
	size_t	wlen;

	i = 0;
	j = 0;
	while (i < word_count)
	{
		while (s[j] == c && s[j] != '\0')
			j++;
		start = j;
		while (s[j] != c && s[j] != '\0')
			j++;
		wlen = j - start;
		arr[i] = ft_substr(s, start, wlen);
		i++;
	}
	arr[i] = NULL;
}

static size_t	word_char(char const *s, char c)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			n++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else if (s[i] == c)
			i++;
	}
	return (n);
}

static void	free_and_return(char **arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	words;
	size_t	i;

	if (!s)
		return (NULL);
	words = word_char(s, c);
	arr = (char **)malloc(sizeof(char *) * (words + 1));
	if (arr == NULL)
		return (NULL);
	assign_words(s, c, arr, words);
	i = 0;
	while (i < words)
	{
		if (arr[i] == NULL)
		{
			free_and_return(arr, i);
			return (NULL);
		}
		i++;
	}
	return (arr);
}
