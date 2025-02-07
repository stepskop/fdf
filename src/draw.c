/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sskopek <sskopek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:42:41 by username          #+#    #+#             */
/*   Updated: 2024/10/30 17:04:21 by username         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static t_point	proj_persp(t_point point, double d)
{
	t_point	res;
	double	min_z;
	double	trim_z;

	min_z = d - 1;
	trim_z = point.z;
	if (point.z > min_z)
		trim_z = min_z;
	res.x = point.x / (-trim_z / d + 1);
	res.y = point.y / (-trim_z / d + 1);
	res.z = point.z;
	res.color = point.color;
	return (res);
}

static t_point	proc_point(t_point point, t_fdf fdf)
{
	t_point	res;

	if (fdf.cam.pers)
		point = proj_persp(point, 100);
	res = v_add(point, fdf.cam.offs);
	return (res);
}

static void	draw_map(t_fdf fdf)
{
	int		i;
	int		j;
	t_point	curr;

	i = -1;
	curr = v_add(*(fdf.map.pts[0][0]), fdf.cam.offs);
	while (fdf.map.pts[++i])
	{
		j = -1;
		while (fdf.map.pts[i][++j])
		{
			curr = proc_point(*(fdf.map.pts[i][j]), fdf);
			if (fdf.map.raw[i][j + 1])
				draw_line(curr,
					proc_point(*(fdf.map.pts[i][j + 1]), fdf), fdf);
			if (fdf.map.raw[i + 1])
				draw_line(curr,
					proc_point(*(fdf.map.pts[i + 1][j]), fdf), fdf);
		}
	}
}

static void	draw_instructions(t_fdf *fdf)
{
	int	line;

	line = 0;
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + ((line++) * 20), 0xFFFFFF,
		"Translate [Up / Down / Left / Right]");
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + ((line++) * 20), 0xFFFFFF,
		"Rotate [W / A / S / D / Q / E]");
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + ((line++) * 20), 0xFFFFFF,
		"Zoom [- / =]");
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + ((line++) * 20), 0xFFFFFF,
		"Elevation [I / K]");
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + ((line++) * 20), 0xFFFFFF,
		"Reset [R]");
	mlx_string_put(fdf->mlx, fdf->win, 30, 40 + (line * 20), 0xFFFFFF,
		"Perspective [P]: ");
	if (fdf->cam.pers)
		mlx_string_put(fdf->mlx, fdf->win, 135, 40 + (line * 20), 0x03FC6B,
			"ON");
	else
		mlx_string_put(fdf->mlx, fdf->win, 135, 40 + (line * 20), 0xFC0356,
			"OFF");
	mlx_string_put(fdf->mlx, fdf->win, WIDTH - 150, HEIGHT - 40, 0xFFFFFF,
		"github.com/stepskop");
	mlx_string_put(fdf->mlx, fdf->win, WIDTH - 85, HEIGHT - 60, 0xFFFFFF,
		"@sskopek");
}

void	render(t_fdf *fdf)
{
	if (fdf->img.ptr)
		mlx_destroy_image(fdf->mlx, fdf->img.ptr);
	fdf->img.ptr = mlx_new_image(fdf->mlx, WIDTH, HEIGHT);
	if (!fdf->img.ptr)
	{
		ft_printf("Failed to create new img, exiting...\n");
		graceful_exit(fdf, 1);
	}
	fdf->img.addr = mlx_get_data_addr(fdf->img.ptr, &fdf->img.bbp,
			&fdf->img.line_len, &fdf->img.endian);
	draw_map(*fdf);
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img.ptr, 0, 0);
	draw_instructions(fdf);
}
