#include "bubble.h"


Eina_Bool timer_cb(void *data EINA_UNUSED){
	appdata_s *ad = data;

	ad->time++;
	_message_send(ad);

	return ECORE_CALLBACK_RENEW;
}

void draw_map(appdata_s *ad){

	evas_object_hide(ad->box);
	evas_object_hide(ad->box_content);
	evas_object_hide(ad->bottom);

	//grid
	ad->grid = elm_grid_add(ad->win);
	evas_object_size_hint_weight_set(ad->grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, ad->grid);

	evas_object_show(ad->grid);

	/* Information_ Popped Bubble, Timer */
	ad->title = elm_label_add(ad->grid);
	elm_object_text_set(ad->title, "<align=center>No Sensor!</align>");
	elm_grid_pack(ad->grid, ad->title, 5, 10, 100, 20);
	evas_object_show(ad->title);

	/* Map Design */
	ad->canvas = evas_object_evas_get(ad->win);

	ad->grid_width = (49-ad->stage_size)/ ad->stage_size;

	/* Rect- White Wall */
	Evas_Object *rect = evas_object_rectangle_add(ad->canvas);
	evas_object_color_set(rect, 255, 255, 255, 255);
	evas_object_show(rect);
	elm_grid_pack(ad->grid, rect, 25, 30, (ad->grid_width+1)*ad->stage_size+1, (ad->grid_width+1)*ad->stage_size+1);

	/* challenger _ wall */
	/* Rect- Red Wall_ horizontal */
	/*
	rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 0, 0, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 26, 30, ad->grid_width, 1);*/

	/* Rect- Red Wall_ vertical
	rect = evas_object_rectangle_add(canvas);
	evas_object_color_set(rect, 255, 0, 0, 255);
	evas_object_show(rect);
	elm_grid_pack(grid, rect, 25, 31, 1, ad->grid_width);*/

	/* Bubble Image */
	char img_path[PATH_MAX] = "";
	app_get_resource("bubble_not_popped.png", img_path, PATH_MAX);
	Evas_Object *img = evas_object_image_filled_add(ad->canvas);

	/* Rect- Grid */
	int rect_count = 0;

	for (int j=0;j<ad->stage_size;j++){
		for (int i=0;i<ad->stage_size;i++){
			ad->rect[rect_count] = evas_object_rectangle_add(ad->canvas);
			evas_object_color_set(ad->rect[rect_count], 0, 0, 0, 255);
			evas_object_show(ad->rect[rect_count]);
			elm_grid_pack(ad->grid, ad->rect[rect_count], 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);

			rect_count++;

			/* Place obstacles on the specific location of map */
			/* hurdle : grid_state[][][5] =1 */
			if(ad->grid_state[i][j][5]==1)
			{
				app_get_resource("hurdle.png", img_path, PATH_MAX);
				Evas_Object *img = evas_object_image_filled_add(ad->canvas);

				img = evas_object_image_filled_add(ad->canvas);
				evas_object_image_file_set(img, img_path, NULL);
				elm_grid_pack(ad->grid, img, 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);
				evas_object_show(img);
			}
			/* heart : grid_state[][][5] =2 */
			else if(ad->grid_state[i][j][5]==3)
			{
				app_get_resource("heart.png", img_path, PATH_MAX);
				Evas_Object *img = evas_object_image_filled_add(ad->canvas);

				img = evas_object_image_filled_add(ad->canvas);
				evas_object_image_file_set(img, img_path, NULL);
				elm_grid_pack(ad->grid, img, 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);
				evas_object_show(img);
			}
			/* bug : grid_state[][][5] =3 */
			else if(ad->grid_state[i][j][5]==2)
			{
				app_get_resource("bug.png", img_path, PATH_MAX);
				Evas_Object *img = evas_object_image_filled_add(ad->canvas);

				img = evas_object_image_filled_add(ad->canvas);
				evas_object_image_file_set(img, img_path, NULL);
				elm_grid_pack(ad->grid, img, 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);
				evas_object_show(img);

			}
			else{
				app_get_resource("bubble_not_popped.png", img_path, PATH_MAX);

				/* Bubble_ not_ popped */
				img = evas_object_image_filled_add(ad->canvas);
				evas_object_image_file_set(img, img_path, NULL);
				if(ad->grid_state[i][j][4] == 0){
					elm_grid_pack(ad->grid, img, 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);
					evas_object_show(img);
				}
			}
		}
	}

	/* Place of User */
	evas_object_color_set(ad->rect[ad->stage_size * ad->user_state[1] + ad->user_state[0]], 255, 255, 255, 255);

	/* Bubble Popped */
	app_get_resource("bubble_popped.png", img_path, PATH_MAX);

	Evas_Object *img2 = evas_object_image_filled_add(ad->canvas);
	img2 = evas_object_image_filled_add(ad->canvas);
	evas_object_image_file_set(img2, img_path, NULL);
	elm_grid_pack(ad->grid, img2, 26+(ad->grid_width+1)*ad->user_state[0], 31+(ad->grid_width+1)*ad->user_state[1], ad->grid_width, ad->grid_width);
	evas_object_show(img2);
	ad->grid_state[ad->user_state[0]][ad->user_state[1]][4] = 1;


	/* popped already */
	for (int j=0;j<ad->stage_size;j++){
		for (int i=0;i<ad->stage_size;i++){
			if(ad->grid_state[i][j][4] == 1){
				app_get_resource("bubble_popped.png", img_path, PATH_MAX);
				Evas_Object *img2 = evas_object_image_filled_add(ad->canvas);
				img2 = evas_object_image_filled_add(ad->canvas);
				evas_object_image_file_set(img2, img_path, NULL);
				elm_grid_pack(ad->grid, img2, 26+(ad->grid_width+1)*i, 31+(ad->grid_width+1)*j, ad->grid_width, ad->grid_width);
				evas_object_show(img2);
			}
		}
	}


	/* Back_list*/
	ad->back_list = elm_list_add(ad->grid);
	evas_object_size_hint_weight_set(ad->back_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->back_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->back_list, "MENU", NULL, NULL, main_menu_cb, ad);
	elm_grid_pack(ad->grid, ad->back_list, 0, 70, 100, 30);
	evas_object_show(ad->back_list);
}

void map_creater_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	//start timer
	ad->timer = ecore_timer_add(1.0, timer_cb, ad);
	ad->time = 0;

	//create vibration
	device_haptic_open(0, &ad->handle);


	start_acceleration_sensor(ad);
	start_gyroscope_sensor(ad);
	start_heartrate_sensor(ad);
	ad->sensor_status[0] = 2;
	ad->sensor_status[1] = 2;
	ad->sensor_status[2] = 2;


	//initialize user_state and grid state
	ad->user_state[0] = 0;
	ad->user_state[1] = ad->stage_size-1;
	ad->user_state[2] = 0;
	ad->user_state[3] = 0;

	for (int j=0;j<ad->stage_size;j++){ //i,j order change
		for (int i=0;i<ad->stage_size;i++){
			ad->grid_state[i][j][4] = 0;
		}
	}

	ad->user_state[2] = 1;


	draw_map(ad);
}
