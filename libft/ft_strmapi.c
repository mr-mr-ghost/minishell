/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 10:26:30 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/22 12:45:43 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*fstr;
	size_t	len;
	size_t	i;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	fstr = malloc(len + 1);
	if (fstr == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		fstr[i] = f(i, s[i]);
		i++;
	}
	fstr[i] = '\0';
	return (fstr);
}
