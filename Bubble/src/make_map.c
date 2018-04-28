/*
 * make_map.c
 *
 *  Created on: Apr 13, 2018
 *      Author: yunsun
 */

#include "bubble.h"
//#include "sensor_test.c"

static void
map_creater_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	ad->sensor_status[0] = 1;
	start_acceleration_sensor(ad);
	start_gyroscope_sensor(ad);

	evas_object_hide(ad->box);
	evas_object_hide(ad->box_content);
	evas_object_hide(ad->bottom);

	int stage_size = ad->stage_size;

	/* Initial User_State: grid_x, grid_y, bubble_count, shield_count */
	int user_state[4] = {0, stage_size-1, 0, 0};
	/* 0: Up, 1: Down, 2: Left, 3: Right, 4: Bubble_Popped? 5:Challenger */
	int grid_state[stage_size][stage_size][6];

	/* Grid */
	Evas_Object *grid = elm_grid_add(ad->win);
	evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, grid);

	evas_object_show(grid);

	/* Information_ Popped Bubble, Timer */
	ad->title = elm_label_add(grid);
	elm_object_text_set(ad->title, "<font_size =20><align=center>BUBBLE: </align></font_size>");
	elm_grid_pack(grid, ad->title, 5, 10, 70, 10);
	evas_object_show(ad->title);

	/* Map Design */
	Evas* canvas = evas_object_evas_get(ad->win);

	double grid_width = (49-stage_size)/ stage_size;

	/* Rect- White Wall */
	Evas_Object *rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 255, 255, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 25, 30, (grid_width+1)*stage_size+1, (grid_width+1)*stage_size+1);

	// Need Revision
	/* Rect- Red Wall_ horizontal
	rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 0, 0, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 26, 30, grid_width, 1);*/

	/* Rect- Red Wall_ vertical
	rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 0, 0, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 25, 31, 1, grid_width);*/

	/* Bubble Image */
	char img_path[PATH_MAX] = "";
	app_get_resource("bubble_not_popped.png", img_path, PATH_MAX);
	Evas_Object *img = evas_object_image_filled_add(canvas);

	/* Rect- Grid */
	for (int i=0;i<stage_size;i++){
	 	for (int j=0;j<stage_size;j++){
	   		rect = evas_object_rectangle_add(canvas);
	   		evas_object_color_set(rect, 0, 0, 0, 255);
	   		evas_object_show(rect);
	   		elm_grid_pack(grid, rect, 26+(grid_width+1)*i, 31+(grid_width+1)*j, grid_width, grid_width);

	   		/* Bubble_ not_ popped */
	   		img = evas_object_image_filled_add(canvas);
	   		evas_object_image_file_set(img, img_path, NULL);
	   		elm_grid_pack(grid, img, 26+(grid_width+1)*i, 31+(grid_width+1)*j, grid_width, grid_width);
	   		evas_object_show(img);
	   	}
	}

	/* Place of User */
	rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 255, 255, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 26+(grid_width+1)*user_state[0], 31+(grid_width+1)*user_state[1], grid_width, grid_width);

	/* Bubble Popped */
	app_get_resource("bubble_popped.png", img_path, PATH_MAX);
	Evas_Object *img2 = evas_object_image_filled_add(canvas);

	img2 = evas_object_image_filled_add(canvas);
	evas_object_image_file_set(img2, img_path, NULL);
	elm_grid_pack(grid, img2, 26+(grid_width+1)*user_state[0], 31+(grid_width+1)*user_state[1], grid_width, grid_width);
	evas_object_show(img2);

	/* Back_list*/
	ad->back_list = elm_list_add(grid);
	evas_object_size_hint_weight_set(ad->back_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->back_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->back_list, "MENU", NULL, NULL, main_menu_cb, ad);
	elm_grid_pack(grid, ad->back_list, 0, 70, 100, 30);
	evas_object_show(ad->back_list);
}



