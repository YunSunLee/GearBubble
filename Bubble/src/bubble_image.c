/*
 * bubble_image.c
 *
 *  Created on: Apr 12, 2018
 *      Author: yunsun
 */
#include "bubble.h"

static void
app_get_resource(const char *res_file_in, char *res_path_out, int res_path_max)
{
	char *res_path = app_get_resource_path();
	if (res_path) {
	snprintf(res_path_out, res_path_max, "%s%s", res_path, res_file_in);
	free(res_path);
	}
}
