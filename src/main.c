/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sskopek <sskopek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:18:24 by username          #+#    #+#             */
/*   Updated: 2024/10/30 21:40:46 by username         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	dis_init(t_fdf *fdf)
{
	fdf->mlx = mlx_init();
	fdf->win = mlx_new_window(fdf->mlx, WIDTH, HEIGHT, "FdF");
	fdf->img.ptr = NULL;
	return (1);
}

static void	cam_init(t_fdf *fdf)
{
	fdf->cam.elev = 0;
	fdf->cam.offs = (t_point){(double)1280 / 2, (double)720 / 2, 0, -1};
}

static void	map_init(t_fdf *fdf)
{
	fdf->map.h = 0;
	fdf->map.w = 0;
	fdf->map.max = (t_point){0, 0, 0, TOP_COLOR};
	fdf->map.min = (t_point){0, 0, 0, BOT_COLOR};
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;

	if (argc != 2)
		return (ft_printf("MONKE\n"), 1);
	fdf.map.h = 0;
	fdf.map.w = 0;
	cam_init(&fdf);
	map_init(&fdf);
	if (!parse(argv[1], &fdf) || !fdf.map.h || !fdf.map.w)
		return (1);
	proj_iso(&fdf);
	if (dis_init(&fdf))
		render(&fdf);
	mlx_key_hook(fdf.win, on_press, &fdf);
	mlx_hook(fdf.win, 17, 1L << 0, graceful_exit, &fdf);
	mlx_loop(fdf.mlx);
}