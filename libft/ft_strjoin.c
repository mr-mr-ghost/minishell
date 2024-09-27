/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:31:08 by jhoddy            #+#    #+#             */
/*   Updated: 2024/02/28 11:08:17 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*str_cat(char *final, const char *src1, const char *src2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src1[i])
	{
		final[i] = src1[i];
		i++;
	}
	while (src2[j])
	{
		final[i] = src2[j];
		i++;
		j++;
	}
	final[i] = '\0';
	return (final);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s_ult;
	size_t	tlen;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	tlen = ft_strlen(s1) + ft_strlen(s2);
	s_ult = malloc(tlen + 1);
	if (!s_ult)
		return (NULL);
	str_cat(s_ult, s1, s2);
	return (s_ult);
}
