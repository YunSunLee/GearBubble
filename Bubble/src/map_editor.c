/*
 * map_editor.c
 *
 *  Created on: Apr 14, 2018
 *      Author: june
 */

#include "bubble.h"

#define WALL30 "000000000000000000000000000000000000000000000000000000"
#define WALL31 "010000000100000000000000000000000000000000000000000000"
#define WALL32 "000000010000000000000100010000000000000000000000000000"
#define WALL33 "000000010000000000010000000100000000000000000000000000"
#define WALL34 "010000000100000000000000000000000000000100000000000000"
#define WALL40 "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define WALL41 "010000000100000000000000000000000000000000000000000000010000000100000000000000000000000000000000"
#define WALL42 "000000010000000000000000010000000000000000000000000000000000010000000000000100000000000000000000"
#define WALL43 "010000000100000000000000000000000000000000010000000000010000000100000000000000000000000000000000"
#define WALL44 "010000000100000000000000000000000000000100000000000000010000000100000000000000000000000000000000"
#define WALL50 "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define WALL51 "010000000100000000000000000000000000000000000000000000010000000100000000000000000000000000000000000000000000010000000000000000000000000000000000000000"
#define WALL52 "000000010000000000000000010000000000000000000000000000000000010000000000000100010000000000000000000000000000000000010000000000000100000000000000000000"
#define WALL53 "000000010000000000010000000100000000000000000000000000010000000100000000000000000000000000000000000100000000010000010000000000000000000000000000000000"
#define WALL54 "010000000100000000000000000000000000000100000000000000010000000100000000000000000000000000000100000000000000010000000000000000000000000000000000000000"

Eext_Object_Item *items[5];
Eext_Object_Item *exit_item;
Eext_Object_Item *save_item;
static int size;
static char final_grid_info[200] ="000000000000000000000000000000000000000000000000000000";
static int position = 0;

static int write_map_num=0;//for writing to file(to know to which file to write final_grid_info)

static void _rotary_selector_create_wall(appdata_s *ad);
static void _rotary_selector_create_obstacle(appdata_s *ad);

static void map_editor_size_3_cb(void *data, Evas_Object *obj, void *event_info);
static void map_editor_size_4_cb(void *data, Evas_Object *obj, void *event_info);
static void map_editor_size_5_cb(void *data, Evas_Object *obj, void *event_info);

static void custom_map1_cb(void *data, Evas_Object *obj, void *event_info);
static void custom_map2_cb(void *data, Evas_Object *obj, void *event_info);
static void custom_map3_cb(void *data, Evas_Object *obj, void *event_info);
static void custom_map4_cb(void *data, Evas_Object *obj, void *event_info);
static void custom_map5_cb(void *data, Evas_Object *obj, void *event_info);


static void map_editor_decode_3(appdata_s *ad, char grid_info[]){
	//initial map create
	//ad->map_editor_rect[9] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
	evas_object_resize(ad->map_editor_rect[9], 154, 154);
	evas_object_color_set(ad->map_editor_rect[9], 255, 255, 255, 255);
	evas_object_move(ad->map_editor_rect[9], 103, 103);
	evas_object_show(ad->map_editor_rect[9]);

	for(int i = 0; i < size*size; i++){
		//ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
		evas_object_resize(ad->map_editor_rect[i], 50 - (grid_info[6 * i + 3] - '0'), 50 - (grid_info[6 * i + 1] - '0'));
		if(grid_info[6 * i + 5] == '1')
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 255, 100); //jump
		else if(grid_info[6 * i + 5] == '2')
			evas_object_color_set(ad->map_editor_rect[i], 255, 255, 0, 100); //bug
		else if(grid_info[6 * i + 5] == '3')
			evas_object_color_set(ad->map_editor_rect[i], 255, 0, 0, 100); //heart
		else
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 0, 255); //normal

		evas_object_move(ad->map_editor_rect[i], 105 + ((i % 3) * 50), 105 + ((i / 3) * 50));
		evas_object_show(ad->map_editor_rect[i]);
	}


}

static void map_editor_decode_4(appdata_s *ad, char grid_info[]){
	//initial map create
	//ad->map_editor_rect[9] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
	evas_object_resize(ad->map_editor_rect[16], 164, 164);
	evas_object_color_set(ad->map_editor_rect[16], 255, 255, 255, 255);
	evas_object_move(ad->map_editor_rect[16], 83, 83);
	evas_object_show(ad->map_editor_rect[16]);

	for(int i = 0; i < size*size; i++){
		//ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
		evas_object_resize(ad->map_editor_rect[i], 40 - (grid_info[6 * i + 3] - '0'), 40 - (grid_info[6 * i + 1] - '0'));
		if(grid_info[6 * i + 5] == '1')
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 255, 100); //jump
		else if(grid_info[6 * i + 5] == '2')
			evas_object_color_set(ad->map_editor_rect[i], 255, 255, 0, 100); //bug
		else if(grid_info[6 * i + 5] == '3')
			evas_object_color_set(ad->map_editor_rect[i], 255, 0, 0, 100); //heart
		else
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 0, 255); //normal

		evas_object_move(ad->map_editor_rect[i], 85 + ((i % 4) * 40), 85 + ((i / 4) * 40));
		evas_object_show(ad->map_editor_rect[i]);
	}
}

static void map_editor_decode_5(appdata_s *ad, char grid_info[]){
	//initial map create
	//ad->map_editor_rect[9] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
	evas_object_resize(ad->map_editor_rect[25], 154, 154);
	evas_object_color_set(ad->map_editor_rect[25], 255, 255, 255, 255);
	evas_object_move(ad->map_editor_rect[25], 103, 103);
	evas_object_show(ad->map_editor_rect[25]);

	for(int i = 0; i < size*size; i++){
		//ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
		evas_object_resize(ad->map_editor_rect[i], 30 - (grid_info[6 * i + 3] - '0'), 30 - (grid_info[6 * i + 1] - '0'));
		if(grid_info[6 * i + 5] == '1')
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 255, 100); //jump
		else if(grid_info[6 * i + 5] == '2')
			evas_object_color_set(ad->map_editor_rect[i], 255, 255, 0, 100); //bug
		else if(grid_info[6 * i + 5] == '3')
			evas_object_color_set(ad->map_editor_rect[i], 255, 0, 0, 100); //heart
		else
			evas_object_color_set(ad->map_editor_rect[i], 0, 0, 0, 255); //normal

		evas_object_move(ad->map_editor_rect[i], 105 + ((i % 5) * 30), 105 + ((i / 5) * 30));
		evas_object_show(ad->map_editor_rect[i]);
	}
}

static void
_item_clicked_cb_wall(void *data, Evas_Object *obj, void *event_info)
{
	if (!data) return;
	appdata_s *ad = data;


   Evas_Object *rotary_selector = ad->rotary_selector;
   Eext_Object_Item *selected_item = eext_rotary_selector_selected_item_get(rotary_selector);

   /*
   eext_rotary_selector_item_part_color_set(selected_item,
                                            "item,bg_image",
                                            EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL,
                                            255, 255, 255, 255); */

   if(size == 3){
	   if(selected_item == exit_item){
		   create_base_gui(ad);
	   }
	   else{
		   if(selected_item == items[0])
			   strcpy(final_grid_info, WALL30);
		   else if(selected_item == items[1])
			   strcpy(final_grid_info, WALL31);
		   else if(selected_item == items[2])
			   strcpy(final_grid_info, WALL32);
		   else if(selected_item == items[3])
			   strcpy(final_grid_info, WALL33);
		   else if(selected_item == items[4])
			   strcpy(final_grid_info, WALL34);

		   _rotary_selector_create_obstacle(ad);
		}
    }
   if(size == 4){
   	   if(selected_item == exit_item){
   		   create_base_gui(ad);
   	   }
   	   else{
   		   if(selected_item == items[0])
   			   strcpy(final_grid_info, WALL40);
   		   else if(selected_item == items[1])
   			   strcpy(final_grid_info, WALL41);
   		   else if(selected_item == items[2])
   			   strcpy(final_grid_info, WALL42);
   		   else if(selected_item == items[3])
   			   strcpy(final_grid_info, WALL43);
   		   else if(selected_item == items[4])
   			   strcpy(final_grid_info, WALL44);

   		   _rotary_selector_create_obstacle(ad);
   		}
    }
    else if(size == 5){
   	   if(selected_item == exit_item){
   		   create_base_gui(ad);
   	   }
   	   else{
   		   if(selected_item == items[0])
   			   strcpy(final_grid_info, WALL50);
   		   else if(selected_item == items[1])
   			   strcpy(final_grid_info, WALL51);
   		   else if(selected_item == items[2])
   			   strcpy(final_grid_info, WALL52);
   		   else if(selected_item == items[3])
   			   strcpy(final_grid_info, WALL53);
   		   else if(selected_item == items[4])
   			   strcpy(final_grid_info, WALL54);

   		   _rotary_selector_create_obstacle(ad);
   	    }
    }

}

static void
_item_selected_cb_wall(void *data, Evas_Object *obj, void *event_info)
{
   if (!data) return;
   appdata_s *ad = data;

   Evas_Object *rotary_selector = ad->rotary_selector;
   Eext_Object_Item *selected_item = eext_rotary_selector_selected_item_get(rotary_selector);


   /*
   eext_rotary_selector_item_part_color_set(selected_item,
                                            "item,bg_image",
                                            EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL,
                                            r, g, b, 255); */

   if(size == 3){
	   if(selected_item == items[0])
		   map_editor_decode_3(ad, WALL30);
	   else if(selected_item == items[1])
		   map_editor_decode_3(ad, WALL31);
	   else if(selected_item == items[2])
		   map_editor_decode_3(ad, WALL32);
	   else if(selected_item == items[3])
		   map_editor_decode_3(ad, WALL33);
	   else if(selected_item == items[4])
		   map_editor_decode_3(ad, WALL34);
	   else if(selected_item == exit_item){
		   for(int i = 0; i < size * size + 1; i++){
				evas_object_hide(ad->map_editor_rect[i]);
		   }
	   }
   }
   else if(size == 4){
   	   if(selected_item == items[0])
   		   map_editor_decode_4(ad, WALL40);
   	   else if(selected_item == items[1])
   		   map_editor_decode_4(ad, WALL41);
   	   else if(selected_item == items[2])
   		   map_editor_decode_4(ad, WALL42);
   	   else if(selected_item == items[3])
   		   map_editor_decode_4(ad, WALL43);
   	   else if(selected_item == items[4])
   		   map_editor_decode_4(ad, WALL44);
   	   else if(selected_item == exit_item){
   		   for(int i = 0; i < size * size + 1; i++){
   				evas_object_hide(ad->map_editor_rect[i]);
   		   }
   	   }
   }
   else if(size == 5){
   	   if(selected_item == items[0])
   		   map_editor_decode_5(ad, WALL50);
   	   else if(selected_item == items[1])
   		   map_editor_decode_5(ad, WALL51);
   	   else if(selected_item == items[2])
   		   map_editor_decode_5(ad, WALL52);
   	   else if(selected_item == items[3])
   		   map_editor_decode_5(ad, WALL53);
   	   else if(selected_item == items[4])
   		   map_editor_decode_5(ad, WALL54);
   	   else if(selected_item == exit_item){
   		   for(int i = 0; i < size * size + 1; i++){
   				evas_object_hide(ad->map_editor_rect[i]);
   		   }
   	   }
    }
}

static void
_item_clicked_cb_obstacle(void *data, Evas_Object *obj, void *event_info)
{
	if (!data) return;
	appdata_s *ad = data;


   Evas_Object *rotary_selector = ad->rotary_selector;
   Eext_Object_Item *selected_item = eext_rotary_selector_selected_item_get(rotary_selector);

   /*
   eext_rotary_selector_item_part_color_set(selected_item,
                                            "item,bg_image",
                                            EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL,
                                            255, 255, 255, 255); */


   //can not put obstacles on the start point
   if(selected_item == items[0] && position != size*(size-1)){ //jump
	   if(final_grid_info[6 * position + 5] == '1'){
		   evas_object_color_set(ad->map_editor_rect[position], 0, 0, 0, 255);
		   final_grid_info[6 * position + 5] = '0';
	   }
	   else{
		   evas_object_color_set(ad->map_editor_rect[position], 0, 0, 255, 100);
		   final_grid_info[6 * position + 5] = '1';
	   }
   }
   else if(selected_item == items[1] && position != size*(size-1)){ //bug
   	   if(final_grid_info[6 * position + 5] == '2'){
   		   evas_object_color_set(ad->map_editor_rect[position], 0, 0, 0, 255);
   		   final_grid_info[6 * position + 5] = '0';
   	   }
   	   else{
   		   evas_object_color_set(ad->map_editor_rect[position], 255, 255, 0, 100);
   		   final_grid_info[6 * position + 5] = '2';
   	   }
      }
      else if(selected_item == items[2] && position != size*(size-1)){ //heart
   	   if(final_grid_info[6 * position + 5] == '3'){
   		   evas_object_color_set(ad->map_editor_rect[position], 0, 0, 0, 255);
   		   final_grid_info[6 * position + 5] = '0';
   	   }
   	   else{
   		   evas_object_color_set(ad->map_editor_rect[position], 255, 0, 0, 100);
   		   final_grid_info[6 * position + 5] = '3';
   	   }
      }
   else if(selected_item == items[3]){ //next
	   position = (position + 1) % (size*size);

	   switch(size){
	   	   case 3:
	   		   map_editor_decode_3(ad, final_grid_info);
	   		   evas_object_resize(ad->map_editor_rect[position], 40, 40);
	   		   break;
	   	   case 4:
	   		   map_editor_decode_4(ad, final_grid_info);
			   evas_object_resize(ad->map_editor_rect[position], 32, 32);
			   break;
	   	   case 5:
	   		   map_editor_decode_5(ad, final_grid_info);
			   evas_object_resize(ad->map_editor_rect[position], 25, 25);
			   break;
	   }
   }
   else if(selected_item == items[4]){ //prev
	   if(position == 0)
		   position = size*size - 1;
	   else
		   position = (position - 1) % (size*size);

	   switch(size){
		   case 3:
			   map_editor_decode_3(ad, final_grid_info);
			   evas_object_resize(ad->map_editor_rect[position], 40, 40);
			   break;
		   case 4:
			   map_editor_decode_4(ad, final_grid_info);
			   evas_object_resize(ad->map_editor_rect[position], 32, 32);
			   break;
		   case 5:
			   map_editor_decode_5(ad, final_grid_info);
			   evas_object_resize(ad->map_editor_rect[position], 25, 25);
			   break;
	   }
   }
   else if(selected_item == save_item){
	   //maps are stored in map1.txt, map2.txt, map3.txt, map4.txt, map5.txt
	   //will keep track of current map number by write_map_num
	   //if 6th map needs to be made, it will overwrite map1.txt(maximum num of maps: 5)
	   write_map_num++;
	   if(write_map_num==6)
		   write_map_num=1;

	   char filepath[PATH_MAX] = {0,};
	   switch(write_map_num){
	   	   case 1:
	   		   app_get_data("map1.txt", filepath, PATH_MAX);
	   		   break;
	   	   case 2:
	   		   app_get_data("map2.txt", filepath, PATH_MAX);
	   		   break;
	   	   case 3:
	   		   app_get_data("map3.txt", filepath, PATH_MAX);
	   		   break;
	   	   case 4:
	   		   app_get_data("map4.txt", filepath, PATH_MAX);
	   		   break;
	   	   case 5:
	   		   app_get_data("map5.txt", filepath, PATH_MAX);
	   		   break;
	   }

	   //write to map.txt final_grid_info
	   FILE *fp;
	   fp = fopen(filepath, "w");
	   fprintf(fp,final_grid_info);
	   fputs("\n", fp);
	   fclose(fp);

	   /* hide current display */
	   evas_object_hide(ad->rotary_selector);
	   for(int i=0;i<=size*size;i++)
		   evas_object_hide(ad->map_editor_rect[i]);

	   //show list of custom maps
		/* Box */
		ad->box2 = elm_box_add(ad->conform);
		evas_object_size_hint_weight_set(ad->box2, EVAS_HINT_EXPAND,	EVAS_HINT_EXPAND);
		elm_object_content_set(ad->conform, ad->box2);
		evas_object_show(ad->box2);

		ad->box_title2 = elm_box_add(ad->box2);
		evas_object_size_hint_weight_set(ad->box_title2, EVAS_HINT_EXPAND, 0.3);
		evas_object_show(ad->box_title2);
		elm_box_pack_end(ad->box2, ad->box_title2);

		/* Title */
		ad->title2 = elm_label_add(ad->box_title2);
		elm_object_text_set(ad->title2, "<font_size = 50><align=center>Select map</align></font_size>");
		elm_box_pack_end(ad->box_title2, ad->title2);
		evas_object_show(ad->title2);

		ad->box_content2 = elm_box_add(ad->conform);
		evas_object_size_hint_weight_set(ad->box_content2, EVAS_HINT_EXPAND, 0.5);
		evas_object_show(ad->box_content2);

		/* List */
		/* Create the list */
		ad->main_list2 = elm_list_add(ad->box2);
		/* Set the list size */
		evas_object_size_hint_weight_set(ad->main_list2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(ad->main_list2, EVAS_HINT_FILL, EVAS_HINT_FILL);

		/* Add an item to the list */
		elm_list_item_append(ad->main_list2, "MAP 1", NULL, NULL, custom_map1_cb, ad);
		elm_list_item_append(ad->main_list2, "MAP 2", NULL, NULL, custom_map2_cb, ad);
		elm_list_item_append(ad->main_list2, "MAP 3", NULL, NULL, custom_map3_cb, ad);
		elm_list_item_append(ad->main_list2, "MAP 4", NULL, NULL, custom_map4_cb, ad);
		elm_list_item_append(ad->main_list2, "MAP 5", NULL, NULL, custom_map5_cb, ad);

		/* Show and add to box */
		evas_object_show(ad->main_list2);
		elm_box_pack_end(ad->box2, ad->main_list2);

   }
   else if(selected_item == exit_item){ //without save
	   create_base_gui(ad);
   }
}

static void
custom_map1_cb(void *data, Evas_Object *obj, void *event_info)
{
	//display
	appdata_s *ad = data;
	//elm_object_text_set(ad->title2, "<font_size = 50><align=center>MAP 1</align></font_size>");
	//evas_object_hide(ad->main_list2);
	//elm_box_unpack(ad->box, ad->main_list2);

	//read from file to char *map_str
	// open file
	// get from data
	FILE *fp;
	char str[200];
	char filepath[PATH_MAX] = {0,};
	app_get_data("map1.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	// get from res
	if(fp==NULL){
		app_get_resource("map1.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	char *map_str;
	map_str=fgets(str, sizeof(str), fp);//read final_grid_info[] from file

	fclose(fp);

	//change format from final_grid_info[] to grid_state[][][]
	int x=0;
	size = sqrt((strlen(map_str) - 1) / 6 ) ;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<6;k++)
			{
				int temp = map_str[x++]-'0';
				ad->grid_state[j][i][k]=temp;
			}
	elm_object_text_set(ad->title2, map_str);

	//set stage_size
	ad->stage_size = size;

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

	ad->user_state[2] = 1;

	//hand it over to make map
	draw_map(ad);

}

static void
custom_map2_cb(void *data, Evas_Object *obj, void *event_info)
{
	//display
	appdata_s *ad = data;
	//elm_object_text_set(ad->title2, "<font_size = 50><align=center>MAP 1</align></font_size>");
	//evas_object_hide(ad->main_list2);
	//elm_box_unpack(ad->box, ad->main_list2);

	//read from file to char *map_str
	// open file
	// get from data
	FILE *fp;
	char str[200];
	char filepath[PATH_MAX] = {0,};
	app_get_data("map2.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	// get from res
	if(fp==NULL){
		app_get_resource("map2.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	char *map_str;
	map_str=fgets(str, sizeof(str), fp);//read final_grid_info[] from file

	fclose(fp);

	//change format from final_grid_info[] to grid_state[][][]
	int x=0;
	size = sqrt((strlen(map_str) - 1) / 6 ) ;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<6;k++)
			{
				int temp = map_str[x++]-'0';
				ad->grid_state[j][i][k]=temp;
			}
	elm_object_text_set(ad->title2, map_str);

	//set stage_size
	ad->stage_size = size;

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

	ad->user_state[2] = 1;

	//hand it over to make map
	draw_map(ad);

}


static void
custom_map3_cb(void *data, Evas_Object *obj, void *event_info)
{
	//display
	appdata_s *ad = data;
	//elm_object_text_set(ad->title2, "<font_size = 50><align=center>MAP 1</align></font_size>");
	//evas_object_hide(ad->main_list2);
	//elm_box_unpack(ad->box, ad->main_list2);

	//read from file to char *map_str
	// open file
	// get from data
	FILE *fp;
	char str[200];
	char filepath[PATH_MAX] = {0,};
	app_get_data("map3.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	// get from res
	if(fp==NULL){
		app_get_resource("map3.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	char *map_str;
	map_str=fgets(str, sizeof(str), fp);//read final_grid_info[] from file

	fclose(fp);

	//change format from final_grid_info[] to grid_state[][][]
	int x=0;
	size = sqrt((strlen(map_str) - 1) / 6 ) ;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<6;k++)
			{
				int temp = map_str[x++]-'0';
				ad->grid_state[j][i][k]=temp;
			}
	elm_object_text_set(ad->title2, map_str);

	//set stage_size
	ad->stage_size = size;

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

	ad->user_state[2] = 1;

	//hand it over to make map
	draw_map(ad);

}


static void
custom_map4_cb(void *data, Evas_Object *obj, void *event_info)
{
	//display
	appdata_s *ad = data;
	//elm_object_text_set(ad->title2, "<font_size = 50><align=center>MAP 1</align></font_size>");
	//evas_object_hide(ad->main_list2);
	//elm_box_unpack(ad->box, ad->main_list2);

	//read from file to char *map_str
	// open file
	// get from data
	FILE *fp;
	char str[200];
	char filepath[PATH_MAX] = {0,};
	app_get_data("map4.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	// get from res
	if(fp==NULL){
		app_get_resource("map4.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	char *map_str;
	map_str=fgets(str, sizeof(str), fp);//read final_grid_info[] from file

	fclose(fp);

	//change format from final_grid_info[] to grid_state[][][]
	int x=0;
	size = sqrt((strlen(map_str) - 1) / 6 ) ;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<6;k++)
			{
				int temp = map_str[x++]-'0';
				ad->grid_state[j][i][k]=temp;
			}
	elm_object_text_set(ad->title2, map_str);

	//set stage_size
	ad->stage_size = size;

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

	ad->user_state[2] = 1;

	//hand it over to make map
	draw_map(ad);

}


static void
custom_map5_cb(void *data, Evas_Object *obj, void *event_info)
{
	//display
	appdata_s *ad = data;
	//elm_object_text_set(ad->title2, "<font_size = 50><align=center>MAP 1</align></font_size>");
	//evas_object_hide(ad->main_list2);
	//elm_box_unpack(ad->box, ad->main_list2);

	//read from file to char *map_str
	// open file
	// get from data
	FILE *fp;
	char str[200];
	char filepath[PATH_MAX] = {0,};
	app_get_data("map5.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	// get from res
	if(fp==NULL){
		app_get_resource("map5.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	char *map_str;
	map_str=fgets(str, sizeof(str), fp);//read final_grid_info[] from file

	fclose(fp);

	//change format from final_grid_info[] to grid_state[][][]
	int x=0;
	size = sqrt((strlen(map_str) - 1) / 6 ) ;
	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++)
			for(int k=0;k<6;k++)
			{
				int temp = map_str[x++]-'0';
				ad->grid_state[j][i][k]=temp;
			}
	elm_object_text_set(ad->title2, map_str);

	//set stage_size
	ad->stage_size = size;

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

	ad->user_state[2] = 1;

	//hand it over to make map
	draw_map(ad);

}


static void
_item_selected_cb_obstacle(void *data, Evas_Object *obj, void *event_info) //not really important cb
{
   if (!data) return;
   appdata_s *ad = data;

   Evas_Object *rotary_selector = ad->rotary_selector;
   Eext_Object_Item *selected_item = eext_rotary_selector_selected_item_get(rotary_selector);


   /*
   eext_rotary_selector_item_part_color_set(selected_item,
                                            "item,bg_image",
                                            EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL,
                                            r, g, b, 255); */


   //save or exit
   if(selected_item == save_item)
	   for(int i = 0; i < size * size + 1; i++){
			evas_object_hide(ad->map_editor_rect[i]);
	   }
   else if(selected_item == exit_item){
	   for(int i = 0; i < size * size + 1; i++){
			evas_object_hide(ad->map_editor_rect[i]);
	   }
   }
   else{
	   for(int i = 0; i < size * size + 1; i++){
			evas_object_show(ad->map_editor_rect[i]);
	   }
   }

}

static void
_rotary_selector_create_wall(appdata_s *ad)
{
   ad->rotary_selector = eext_rotary_selector_add(ad->conform);

   const int items_count = 5;
   for (int i = 0; i < items_count; ++i)
     {
        Eext_Object_Item *item = eext_rotary_selector_item_append(ad->rotary_selector);
        items[i] = item;

        const char MAX_TEXT_SIZE = 20;
        char item_text[MAX_TEXT_SIZE] = {'\0',};
        snprintf(item_text, MAX_TEXT_SIZE, "Element %d", i + 1);
        //eext_rotary_selector_item_part_text_set(item, "selector,main_text", item_text);
        //eext_rotary_selector_item_part_text_set(item, "selector,sub_text", "Additional information");
     }

   Eext_Object_Item *item = eext_rotary_selector_item_append(ad->rotary_selector);
   const char MAX_TEXT_SIZE = 20;
   char item_text[MAX_TEXT_SIZE] = {'\0',};
   snprintf(item_text, MAX_TEXT_SIZE, "EXIT");
   eext_rotary_selector_item_part_text_set(item, "selector,main_text", item_text);
   eext_rotary_selector_item_part_text_set(item, "selector,sub_text", "main menu");

   char img_path[PATH_MAX] = "";
   Evas_Object *img = elm_image_add(ad->rotary_selector);
   app_get_resource("exit.png", img_path, PATH_MAX);
   elm_image_file_set(img, "exit.png", NULL);
   evas_object_image_file_set(img, img_path, NULL);
   evas_object_show(img);
   eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);

   exit_item = item;

   evas_object_smart_callback_add(ad->rotary_selector,
                                  "item,clicked",
                                  _item_clicked_cb_wall,
                                  ad);

   evas_object_smart_callback_add(ad->rotary_selector,
                                  "item,selected",
                                  _item_selected_cb_wall,
                                  ad);

   eext_rotary_object_event_activated_set(ad->rotary_selector, EINA_TRUE);

   elm_object_content_set(ad->conform, ad->rotary_selector);
   evas_object_show(ad->rotary_selector);

   //wall editor start
   if(size == 3){
	   ad->map_editor_rect[9] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
	   for(int i = 0; i < 9; i++)
			ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
	   char grid_info[54] = WALL30;
	   map_editor_decode_3(ad, grid_info);
   }
   else if(size == 4){
   	   ad->map_editor_rect[16] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
   	   for(int i = 0; i < 16; i++)
   			ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
   	   char grid_info[96] = WALL40;
   	   map_editor_decode_4(ad, grid_info);
   }
   else if(size == 5){
   	   ad->map_editor_rect[25] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
   	   for(int i = 0; i < 25; i++)
   			ad->map_editor_rect[i] = evas_object_rectangle_add(evas_object_evas_get(ad->win));
   	   char grid_info[150] = WALL50;
   	   map_editor_decode_5(ad, grid_info);
   }

}

static void
_rotary_selector_create_obstacle(appdata_s *ad)
{
   ad->rotary_selector = eext_rotary_selector_add(ad->conform);

   char img_path[PATH_MAX] = "";

   const int items_count = 5;
   for (int i = 0; i < items_count; ++i)
     {
        Eext_Object_Item *item = eext_rotary_selector_item_append(ad->rotary_selector);
        items[i] = item;


        if(i == 0){ //jump
        	eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 0, 0, 100, 100);

			Evas_Object *img = elm_image_add(ad->rotary_selector);
			app_get_resource("hurdle.png", img_path, PATH_MAX);
			elm_image_file_set(img, "hurdle.png", NULL);
			evas_object_image_file_set(img, img_path, NULL);
			evas_object_show(img);

        	eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);
        }
        else if(i == 1){ //bug
        	eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 100, 100, 0, 100);

        	Evas_Object *img = elm_image_add(ad->rotary_selector);
        	app_get_resource("bug.png", img_path, PATH_MAX);
        	elm_image_file_set(img, "bug.png", NULL);
        	evas_object_image_file_set(img, img_path, NULL);
        	evas_object_show(img);

			eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);
        }
        else if(i == 2){ //heart
        	eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 100, 0, 0, 100);

        	Evas_Object *img = elm_image_add(ad->rotary_selector);
        	app_get_resource("heart2.png", img_path, PATH_MAX);
        	elm_image_file_set(img, "heart2.png", NULL);
        	evas_object_image_file_set(img, img_path, NULL);
        	evas_object_show(img);

			eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);
        }
        else if(i == 3){ //next
			eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 255,255,255,255);

			Evas_Object *img = elm_image_add(ad->rotary_selector);
			app_get_resource("arrow_right.png", img_path, PATH_MAX);
			elm_image_file_set(img, "arrow_right.png", NULL);
			evas_object_image_file_set(img, img_path, NULL);
			evas_object_show(img);

			eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);
		}
        else if(i == 4){ //prev
			eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 255,255,255,255);

			Evas_Object *img = elm_image_add(ad->rotary_selector);
			app_get_resource("arrow_left.png", img_path, PATH_MAX);
			elm_image_file_set(img, "arrow_left.png", NULL);
			evas_object_image_file_set(img, img_path, NULL);
			evas_object_show(img);

			eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);
		}

        const char MAX_TEXT_SIZE = 20;
        char item_text[MAX_TEXT_SIZE] = {'\0',};
        snprintf(item_text, MAX_TEXT_SIZE, "Element %d", i + 1);
        //eext_rotary_selector_item_part_text_set(item, "selector,main_text", item_text);
        //eext_rotary_selector_item_part_text_set(item, "selector,sub_text", "Additional information");
     }

   	//save
	Eext_Object_Item *item = eext_rotary_selector_item_append(ad->rotary_selector);
	const char MAX_TEXT_SIZE = 20;
	char item_text[MAX_TEXT_SIZE] = {'\0',};
	snprintf(item_text, MAX_TEXT_SIZE, "SAVE");
	eext_rotary_selector_item_part_text_set(item, "selector,main_text", item_text);
	eext_rotary_selector_item_part_text_set(item, "selector,sub_text", "save the map");
	eext_rotary_selector_item_part_color_set(item, "item,bg_image", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, 255,255,255,255);

	Evas_Object *img = elm_image_add(ad->rotary_selector);
	app_get_resource("save.png", img_path, PATH_MAX);
	elm_image_file_set(img, "save.png", NULL);
	evas_object_image_file_set(img, img_path, NULL);
	evas_object_show(img);
	eext_rotary_selector_item_part_content_set(item, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img);

	save_item = item;


	//without save
	Eext_Object_Item *item2 = eext_rotary_selector_item_append(ad->rotary_selector);
	snprintf(item_text, MAX_TEXT_SIZE, "EXIT");
	eext_rotary_selector_item_part_text_set(item2, "selector,main_text", item_text);
	eext_rotary_selector_item_part_text_set(item2, "selector,sub_text", "without save");

	Evas_Object *img2 = elm_image_add(ad->rotary_selector);
	app_get_resource("exit.png", img_path, PATH_MAX);
	elm_image_file_set(img2, "exit.png", NULL);
	evas_object_image_file_set(img2, img_path, NULL);
	evas_object_show(img2);
	eext_rotary_selector_item_part_content_set(item2, "item,icon", EEXT_ROTARY_SELECTOR_ITEM_STATE_NORMAL, img2);

	exit_item = item2;



   evas_object_smart_callback_add(ad->rotary_selector,
                                  "item,clicked",
                                  _item_clicked_cb_obstacle,
                                  ad);

   evas_object_smart_callback_add(ad->rotary_selector,
                                  "item,selected",
                                  _item_selected_cb_obstacle,
                                  ad);

   eext_rotary_object_event_activated_set(ad->rotary_selector, EINA_TRUE);

   elm_object_content_set(ad->conform, ad->rotary_selector);
   evas_object_show(ad->rotary_selector);



   //obstacle editor start
   position = 0;

   if(size == 3){
	   map_editor_decode_3(ad, final_grid_info);
	   evas_object_resize(ad->map_editor_rect[position], 40, 40);
   }
   else if(size == 4){
   	   map_editor_decode_4(ad, final_grid_info);
   	   evas_object_resize(ad->map_editor_rect[position], 32, 32);
   }
   else if(size == 5){
   	   map_editor_decode_5(ad, final_grid_info);
   	   evas_object_resize(ad->map_editor_rect[position], 25, 25);
   }
}


void map_editor_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>MAP EDITOR</align></font_size>");
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	evas_object_hide(ad->bottom);
	elm_box_unpack(ad->box_bottom, ad->bottom);

	//map editor

	ad->map_editor_size_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->map_editor_size_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->map_editor_size_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->map_editor_size_list, "3X3", NULL, NULL, map_editor_size_3_cb, ad);
	elm_list_item_append(ad->map_editor_size_list, "4X4", NULL, NULL, map_editor_size_4_cb, ad);
	elm_list_item_append(ad->map_editor_size_list, "5X5", NULL, NULL, map_editor_size_5_cb, ad);
	evas_object_show(ad->map_editor_size_list);
	elm_box_pack_after(ad->box, ad->map_editor_size_list, ad->box_title);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void map_editor_size_3_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>MAP EDITOR</align></font_size>");
	evas_object_hide(ad->stage_list);
	elm_box_unpack(ad->box, ad->stage_list);

	size = 3;

	_rotary_selector_create_wall(data);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void map_editor_size_4_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>MAP EDITOR</align></font_size>");
	evas_object_hide(ad->stage_list);
	elm_box_unpack(ad->box, ad->stage_list);

	size = 4;

	_rotary_selector_create_wall(data);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void map_editor_size_5_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>MAP EDITOR</align></font_size>");
	evas_object_hide(ad->stage_list);
	elm_box_unpack(ad->box, ad->stage_list);

	size = 5;

	_rotary_selector_create_wall(data);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}
