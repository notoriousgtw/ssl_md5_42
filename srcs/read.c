/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gwood <gwood@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 13:29:09 by gwood             #+#    #+#             */
/*   Updated: 2018/08/13 13:18:42 by gwood            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_ssl.h"

static void				add_input(t_ft_ssl_data *d, t_ft_ssl_input *input)
{
	input->next = d->inputs;
	d->inputs = input;
}

static t_ft_ssl_input	*new_input(void)
{
	t_ft_ssl_input *ret;

	if (!(ret = (t_ft_ssl_input *)ft_memalloc(sizeof(t_ft_ssl_input))))
		return (NULL);
	ret->input_type = -1;
	ret->filename = NULL;
	ret->msg = NULL;
	ret->msg_len = 0;
	ret->next = NULL;
	return (ret);
}

void					ft_ssl_read_stdin(t_ft_ssl_data *d)
{
	int				check;
	char			buf[SSL_BUFF_SIZE + 1];
	t_ft_ssl_input	*input;

	input = new_input();
	input->input_type = INPUT_STDIN;
	input->msg = ft_strnew(0);
	while ((check = read(0, &buf, SSL_BUFF_SIZE)) > 0)
	{
		buf[check] = 0;
		input->msg =
			ft_strjoinfree(input->msg, buf, 1);
		input->msg_len += check;
		ft_bzero(buf, SSL_BUFF_SIZE + 1);
	}
	if (check < 0)
		ft_error_unknown_free(FT_ERROR_UNKNOWN,
			(t_free_fnc)ft_ssl_free_data, d);
	if (d->p)
		ft_putstr(input->msg);
	add_input(d, input);
}

void					ft_ssl_read_string(t_ft_ssl_data *d)
{
	t_ft_ssl_input	*input;

	input = new_input();
	input->input_type = INPUT_STRING;
	input->msg = ft_strdup(d->arg_str);
	input->msg_len = ft_strlen(d->arg_str);
	add_input(d, input);
}

void					ft_ssl_read_file(t_ft_ssl_data *d, char *path)
{
	int				fd;
	int				check;
	char			buf[SSL_BUFF_SIZE + 1];
	t_ft_ssl_input	*input;

	input = new_input();
	input->input_type = INPUT_FILE;
	input->filename = ft_strdup(path);
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		add_input(d, input);
		return ;
	}
	input->msg = ft_strnew(0);
	while ((check = read(fd, &buf, SSL_BUFF_SIZE)) > 0)
	{
		buf[check] = 0;
		input->msg = ft_strjoinfree(input->msg, buf, 1);
		input->msg_len += check;
		ft_bzero(buf, SSL_BUFF_SIZE + 1);
	}
	if (check < 0)
		ft_error_unknown_free(FT_ERROR_UNKNOWN,
			(t_free_fnc)ft_ssl_free_data, d);
	add_input(d, input);
}
