/*
 * sensor_test.c
 *
 *  Created on: Apr 11, 2018
 *      Author: yunsun
 */
#include <sensor.h>
#include "bubble.h"

#define MIN_ACC_DATA 5


static int wait;
static float gyro[3];
static float recent_acc_x[10000];
static float recent_acc_y[10000];
static int recent_acc_x_count = 0;
static int recent_acc_y_count = 0;
static int maybe_stop_count = 0;

static int shake_flag=0;
static int initial_flag=1;// 첫 심장박동수 측정인지를 체크하기 위
static int initial_beat=0;
static int jump_flag=0;
static int bug_num=10;
static int shake_cnt=0;

//1: up, 2: down, 3:left, 4:right
static int can_move(appdata_s *ad, int direction){
	if(direction == 1 && ad->user_state[1] != 0)
		return 1;
	else if(direction == 2 && ad->user_state[1] != ad->stage_size - 1)
		return 1;
	else if(direction == 3 && ad->user_state[0] != 0)
		return 1;
	else if(direction == 4 && ad->user_state[0] != ad->stage_size - 1)
		return 1;
	else
		return 0;
}

static void move(appdata_s *ad, int from, int dest){
	evas_object_color_set(ad->rect[from], 0, 0, 0, 255);
	evas_object_color_set(ad->rect[dest], 255, 255, 255, 255);
}

static void bubble_pop(appdata_s *ad, int x, int y){
	if(ad->grid_state[x][y][4] == 0){
		char img_path[PATH_MAX] = "";
		/* Bubble Popped */
		app_get_resource("bubble_popped.png", img_path, PATH_MAX);
		Evas_Object *img2 = evas_object_image_filled_add(ad->canvas);

		img2 = evas_object_image_filled_add(ad->canvas);
		evas_object_image_file_set(img2, img_path, NULL);
		elm_grid_pack(ad->grid, img2, 26+(ad->grid_width+1)*x, 31+(ad->grid_width+1)*y, ad->grid_width, ad->grid_width);
		evas_object_show(img2);
		ad->grid_state[x][y][4] = 1;
		ad->user_state[2]++;
	}
}


static char* direction(appdata_s *ad, float x[], float y[]){

	char buf[1024];

	float max_x = 0, min_x = 0, max_y = 0, min_y = 0;
	float max_abs_x = 0, max_abs_y = 0;
	int max_x_index = 0, min_x_index = 0, max_y_index = 0, min_y_index = 0;

	if(recent_acc_x_count >= MIN_ACC_DATA-1){
		for(int i = 0; i < recent_acc_x_count; i++){
			if(x[i] > max_x){
				max_x = x[i];
				max_x_index = i;
			}
			else if(x[i] < min_x){
				min_x = x[i];
				min_x_index = i;
			}
		}
		if(fabsf(max_x) >= fabsf(min_x))
			max_abs_x = fabsf(max_x);
		else
			max_abs_x = fabsf(min_x);
	}

	if(recent_acc_y_count >= MIN_ACC_DATA-1){
		for(int i = 0; i < recent_acc_y_count; i++){
			if(y[i] > max_y){
				max_y = y[i];
				max_y_index = i;
			}

			else if(y[i] < min_y){
				min_y = y[i];
				min_y_index = i;
			}
		}
		if(fabsf(max_y) >= fabsf(min_y))
			max_abs_y = fabsf(max_y);
		else
			max_abs_y = fabsf(min_y);
	}

	int type; //0: x, 1: y

	if((max_x - min_x) * recent_acc_x_count > (max_y - min_y) * recent_acc_y_count)
		type = 0;
	else
		type = 1;

	if(type == 0){
		if(max_x_index > min_x_index)
			sprintf(buf, "LEFT");
		else
			sprintf(buf, "RIGHT");
	}
	else{
		if(max_y_index > min_y_index)
			sprintf(buf, "DOWN");
		else
			sprintf(buf, "UP");
	}

	char s1[100], s2[100];
	sprintf(s1, "X: %d / Y: %d", recent_acc_x_count, recent_acc_y_count);
	sprintf(s2, "MAX X: %0.1f / MAX Y: %0.1f", max_x - min_x, max_y - min_y);
	elm_object_text_set(ad->sensor_label[1], s1);
	elm_object_text_set(ad->sensor_label[2], s2);

	return buf;
}

static void show_is_supported(appdata_s *ad)
{
	char buf[PATH_MAX];
	bool is_supported = false;
	sensor_is_supported(SENSOR_ACCELEROMETER, &is_supported);
	//sprintf(buf, "Acceleration Sensor is %s", is_supported ? "support" : "not support");
	//elm_object_text_set(ad->sensor_label[0], buf);
}

static void
_new_sensor_value_acc_jump(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	appdata_s *ad = user_data;

	float z = sensor_data->values[2];

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->sensor_label[0], "Gathering data...");
		 return;
	 }
	 snprintf(buf, sizeof(buf ), "<font_size = 10>Z:%0.1f</font_size>", z);
	 elm_object_text_set(ad->sensor_label[0], buf);

	 if(z>20 || z<-20)
	 {
		 snprintf(buf, sizeof(buf ), "<font_size = 10>JUMP DETECTED!</font_size>");
			 elm_object_text_set(ad->sensor_label[1], buf);
	 }
}

static void
_new_sensor_value_acc_shake(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	appdata_s *ad = user_data;

	float x = sensor_data->values[0];
	float y = sensor_data->values[1];
	float z = sensor_data->values[2];

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->sensor_label[0], "Gathering data...");
		 return;
	 }
	 snprintf(buf, sizeof(buf ), "<font_size = 10>Bugs:%d</font_size>", bug_num);
	 elm_object_text_set(ad->sensor_label[0], buf);

	 snprintf(buf, sizeof(buf ), "<font_size = 10>Shake:%d</font_size>", shake_cnt);
	 elm_object_text_set(ad->sensor_label[1], buf);

	 if(fabsf(x)>20 || fabsf(y)>20 || fabsf(z)>20)
	 {
		 bug_num --;
		 shake_cnt++;
		 snprintf(buf, sizeof(buf ), "<font_size = 10>Bugs:%d</font_size>", bug_num);
		 elm_object_text_set(ad->sensor_label[0], buf);

		 snprintf(buf, sizeof(buf ), "<font_size = 10>Shake:%d</font_size>", shake_cnt);
		 elm_object_text_set(ad->sensor_label[1], buf);
	 }
	 if(shake_cnt>=10)
	 {
		 snprintf(buf, sizeof(buf ), "<font_size = 10>Shake complete!</font_size>");
		 elm_object_text_set(ad->sensor_label[2], buf);
	 }
}

static void
_new_sensor_value_acc(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	appdata_s *ad = user_data;

	float x = sensor_data->values[0];
	float y = sensor_data->values[1];
	float z = sensor_data->values[2];

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->sensor_label[0], "Gathering data...");
		 return;
	 }

	 if(ad->sensor_status[0] >= 1){

		 //play mode
		 if(ad->sensor_status[0] == 2){
			 char buf_title[100];
			 if(ad->user_state[2] == ad->stage_size * ad->stage_size)
				 sprintf(buf_title, "<align=center>CLEAR</align>");
			 else
				 sprintf(buf_title, "<align=center>BUBBLE: %d/%d</align>", ad->user_state[2], ad->stage_size * ad->stage_size);
			 elm_object_text_set(ad->title, buf_title);
		 }


		//test mode
		 snprintf(buf, sizeof(buf ), "<font_size = 10>X:%0.1f/Y:%0.1f/Z:%0.1f</font_size>", x, y, z);
		 elm_object_text_set(ad->sensor_label[0], buf);


		 if(fabsf(x) >= 1.5 && gyro[0] < 10 && gyro[1] < 10){
			 recent_acc_x[recent_acc_x_count] = x;
			 recent_acc_x_count++;
			 maybe_stop_count = 0;
		 }
		 if(fabsf(y) >= 1.2 && gyro[0] < 10 && gyro[1] < 10){
			 recent_acc_y[recent_acc_y_count] = y;
			 recent_acc_y_count++;
			 maybe_stop_count = 0;
		 }
		 if(fabsf(x) < 1.5 && fabsf(y) < 1.2){
			 maybe_stop_count++;
		 }
		 if(maybe_stop_count == 2 && (recent_acc_x_count >= MIN_ACC_DATA || recent_acc_y_count >= MIN_ACC_DATA - 1)){
			 sprintf(buf, direction(ad, recent_acc_x, recent_acc_y));
			 elm_object_text_set(ad->sensor_label[3], buf);

			 if(strcmp(buf, "UP") == 0 && can_move(ad, 1) == 1){
				 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * (ad->user_state[1] - 1) + ad->user_state[0]);
				 ad->user_state[1]--;
				 sprintf(buf, "?");
				 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
			 }
			 else if(strcmp(buf, "DOWN") == 0 && can_move(ad, 2) == 1){
				 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * (ad->user_state[1] + 1) + ad->user_state[0]);
				 ad->user_state[1]++;
				 sprintf(buf, "?");
				 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
			 }
			 else if(strcmp(buf, "LEFT") == 0 && can_move(ad, 3) == 1){
				 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * ad->user_state[1] + (ad->user_state[0] - 1));
				 ad->user_state[0]--;
				 sprintf(buf, "?");
				 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
			 }
			 else if(strcmp(buf, "RIGHT") == 0 && can_move(ad, 4) == 1){
				 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * ad->user_state[1] + (ad->user_state[0] + 1));
				 ad->user_state[0]++;
				 sprintf(buf, "?");
				 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
			 }

			 maybe_stop_count = 0;
			 recent_acc_x_count = 0;
			 recent_acc_y_count = 0;
		 }

	 }
}

static void
_new_sensor_value_gyro(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	gyro[0] = sensor_data->values[0];
	gyro[1] = sensor_data->values[1];
	gyro[2] = sensor_data->values[2];

	 appdata_s *ad = user_data;

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->sensor_label[1], "Gathering data...");
		 return;
	 }

	 //snprintf(buf, sizeof(buf), "<font_size = 10>X:%0.1f/Y:%0.1f/Z:%0.1f</font_size>", gyro[0], gyro[1], gyro[2]);
	 //elm_object_text_set(ad->sensor_label[1], buf);
}

static void
_new_sensor_value_heart(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	int hr = sensor_data->values[0];

	 appdata_s *ad = user_data;

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->sensor_label[1], "Gathering data...");
		 return;
	 }
	 if(ad->sensor_status[1] == 1){
		 snprintf(buf, sizeof(buf), "<font_size = 10>HEART RATE: %d</font_size>", hr);
		 elm_object_text_set(ad->sensor_label[0], buf);
	 }
	 if(hr>30 && initial_flag)
	 {
		 initial_beat=hr;
		 initial_flag=0;
		 snprintf(buf, sizeof(buf), "<font_size = 10>Initial: %d</font_size>", hr);
		 elm_object_text_set(ad->sensor_label[1], buf);
	 }
	 if(!initial_flag && hr> initial_beat+5)
	 {
		 snprintf(buf, sizeof(buf), "<font_size = 10>Initial: %d</font_size>", hr);
		 elm_object_text_set(ad->sensor_label[2], buf);
		 snprintf(buf, sizeof(buf), "<font_size = 10>WALL BREAK!</font_size>");
		 elm_object_text_set(ad->sensor_label[3], buf);
	 }
}

static void
start_acceleration_sensor(appdata_s *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	//err = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &ad->sensor_info.sensor);
	err = sensor_get_default_sensor(SENSOR_LINEAR_ACCELERATION, &ad->sensor_info[0].sensor);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	err = sensor_create_listener(ad->sensor_info[0].sensor, &ad->sensor_info[0].sensor_listener);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	if(jump_flag==1)
		sensor_listener_set_event_cb(ad->sensor_info[0].sensor_listener, 100, _new_sensor_value_acc_jump, ad); //INTERVAL
	else if(shake_flag==1)
		sensor_listener_set_event_cb(ad->sensor_info[0].sensor_listener, 100, _new_sensor_value_acc_shake, ad); //INTERVAL
	else
		sensor_listener_set_event_cb(ad->sensor_info[0].sensor_listener, 100, _new_sensor_value_acc, ad); //INTERVAL
	sensor_listener_start(ad->sensor_info[0].sensor_listener);
	error_check:
	if (err != SENSOR_ERROR_NONE)
	{
		const char *msg;
		char fullmsg[1024];
		switch (err)
		{
			case SENSOR_ERROR_IO_ERROR: msg = "I/O error"; break;
			case SENSOR_ERROR_INVALID_PARAMETER: msg = "Invalid parameter"; break;
			case SENSOR_ERROR_NOT_SUPPORTED: msg = "The sensor type is not supported in the current device"; break;
			case SENSOR_ERROR_PERMISSION_DENIED: msg = "Permission denied"; break;
			case SENSOR_ERROR_OUT_OF_MEMORY: msg = "Out of memory"; break;
			case SENSOR_ERROR_NOT_NEED_CALIBRATION: msg = "Sensor doesn't need calibration"; break;
			case SENSOR_ERROR_OPERATION_FAILED: msg = "Operation failed"; break;
			default: msg = "Unknown error"; break;
		}
		snprintf(fullmsg, sizeof(fullmsg), "<align=center>An error occurred:<br/>%s</>", msg);
		elm_object_text_set(ad->sensor_label[0], "No data");
		elm_object_text_set(ad->sensor_label[1], fullmsg);
	}
}

static void
start_gyroscope_sensor(appdata_s *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	err = sensor_get_default_sensor(SENSOR_GYROSCOPE, &ad->sensor_info[1].sensor);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	err = sensor_create_listener(ad->sensor_info[1].sensor, &ad->sensor_info[1].sensor_listener);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	sensor_listener_set_event_cb(ad->sensor_info[1].sensor_listener, 100, _new_sensor_value_gyro, ad); //INTERVAL
	sensor_listener_start(ad->sensor_info[1].sensor_listener);
	error_check:
	if (err != SENSOR_ERROR_NONE)
	{
		const char *msg;
		char fullmsg[1024];
		switch (err)
		{
			case SENSOR_ERROR_IO_ERROR: msg = "I/O error"; break;
			case SENSOR_ERROR_INVALID_PARAMETER: msg = "Invalid parameter"; break;
			case SENSOR_ERROR_NOT_SUPPORTED: msg = "The sensor type is not supported in the current device"; break;
			case SENSOR_ERROR_PERMISSION_DENIED: msg = "Permission denied"; break;
			case SENSOR_ERROR_OUT_OF_MEMORY: msg = "Out of memory"; break;
			case SENSOR_ERROR_NOT_NEED_CALIBRATION: msg = "Sensor doesn't need calibration"; break;
			case SENSOR_ERROR_OPERATION_FAILED: msg = "Operation failed"; break;
			default: msg = "Unknown error"; break;
		}
		snprintf(fullmsg, sizeof(fullmsg), "<align=center>An error occurred:<br/>%s</>", msg);
		elm_object_text_set(ad->sensor_label[0], "No data");
		elm_object_text_set(ad->sensor_label[1], fullmsg);
	}
}

static void
start_heartrate_sensor(appdata_s *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	err = sensor_get_default_sensor(SENSOR_HRM, &ad->sensor_info[2].sensor);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	err = sensor_create_listener(ad->sensor_info[2].sensor, &ad->sensor_info[2].sensor_listener);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	sensor_listener_set_event_cb(ad->sensor_info[2].sensor_listener, 100, _new_sensor_value_heart, ad); //INTERVAL
	sensor_listener_start(ad->sensor_info[2].sensor_listener);
	error_check:
	if (err != SENSOR_ERROR_NONE)
	{
		const char *msg;
		char fullmsg[1024];
		switch (err)
		{
			case SENSOR_ERROR_IO_ERROR: msg = "I/O error"; break;
			case SENSOR_ERROR_INVALID_PARAMETER: msg = "Invalid parameter"; break;
			case SENSOR_ERROR_NOT_SUPPORTED: msg = "The sensor type is not supported in the current device"; break;
			case SENSOR_ERROR_PERMISSION_DENIED: msg = "Permission denied"; break;
			case SENSOR_ERROR_OUT_OF_MEMORY: msg = "Out of memory"; break;
			case SENSOR_ERROR_NOT_NEED_CALIBRATION: msg = "Sensor doesn't need calibration"; break;
			case SENSOR_ERROR_OPERATION_FAILED: msg = "Operation failed"; break;
			default: msg = "Unknown error"; break;
		}
		snprintf(fullmsg, sizeof(fullmsg), "<align=center>An error occurred:<br/>%s</>", msg);
		elm_object_text_set(ad->sensor_label[0], "No data");
		elm_object_text_set(ad->sensor_label[1], fullmsg);
	}
}

static void
sensor_test_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>SENSOR TEST</align></font_size>");

	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	evas_object_hide(ad->bottom);
	elm_box_unpack(ad->box_bottom, ad->bottom);

	ad->sensor_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->sensor_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->sensor_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->sensor_list, "MOVE TEST", NULL, NULL, move_test_cb, ad);
	elm_list_item_append(ad->sensor_list, "JUMP TEST", NULL, NULL, jump_test_cb, ad);
	elm_list_item_append(ad->sensor_list, "SHAKE TEST", NULL, NULL, shake_test_cb, ad);
	elm_list_item_append(ad->sensor_list, "HEART TEST", NULL, NULL, heart_rate_test_cb, ad);
	evas_object_show(ad->sensor_list);
	elm_box_pack_after(ad->box, ad->sensor_list, ad->box_title);


	//evas_object_event_callback_add(ad->conform, EVAS_CALLBACK_MOUSE_DOWN, main_menu_cb, ad);
	//evas_object_show(ad->back);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}
static void jump_test_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>JUMP TEST</align></font_size>");

	evas_object_hide(ad->sensor_list);
	elm_box_unpack(ad->box, ad->sensor_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);

	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	wait = 0;
	//show_is_supported(ad);

	ad->sensor_status[0] = 1;
	jump_flag=1;
	start_acceleration_sensor(ad);

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void shake_test_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>SHAKE TEST</align></font_size>");

	evas_object_hide(ad->sensor_list);
	elm_box_unpack(ad->box, ad->sensor_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);

	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	wait = 0;
	//show_is_supported(ad);

	ad->sensor_status[0] = 1;
	shake_flag=1;
	start_acceleration_sensor(ad);

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);

}
static void move_test_cb(void *data, Evas_Object *obj, void *event_info){

	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>MOVE TEST</align></font_size>");

	evas_object_hide(ad->sensor_list);
	elm_box_unpack(ad->box, ad->sensor_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);

	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	wait = 0;
	//show_is_supported(ad);

	ad->sensor_status[0] = 1;
	start_acceleration_sensor(ad);
	start_gyroscope_sensor(ad);

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void heart_rate_test_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>HEART TEST</align></font_size>");

	evas_object_hide(ad->sensor_list);
	elm_box_unpack(ad->box, ad->sensor_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);

	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	ad->sensor_status[1] = 1;
	start_heartrate_sensor(ad);

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}
	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}
