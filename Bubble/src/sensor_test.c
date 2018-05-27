/*
 * sensor_test.c
 *
 *  Created on: Apr 11, 2018
 *      Author: yunsun
 */
#include <sensor.h>
#include "bubble.h"

#define MIN_ACC_DATA 4


static int wait = 0;
static float gyro[3];
static float recent_acc_x[10000];
static float recent_acc_y[10000];
static int recent_acc_x_count = 0;
static int recent_acc_y_count = 0;
static int maybe_stop_count = 0;

static int shake_flag=0;
static int initial_flag=1;// 泥??ъ옣諛뺣룞??痢≪젙?몄?瑜?泥댄겕?섍린 ??
static int initial_beat=0;
static int jump_flag=0;
static int heart_flag=0;
static int bug_num=10;
static int shake_cnt=0;
static int is_obstacle=0;

static int jmp_detect=0;
static int shake_detect=0;
//static int heart_detect=0;

Evas_Object *jmp;




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
		device_haptic_vibrate(ad->handle, 200, 50, &ad->effect_handle); //vibration

		/* Pop sound */
		if( get_player_state(ad->player) != PLAYER_STATE_PLAYING)
			player_start(ad->player);
	}
}




static char* direction(appdata_s *ad, float x[], float y[]){

	char buf[1024];

	float max_x = 0, min_x = 0, max_y = 0, min_y = 0;
	float max_abs_x = 0, max_abs_y = 0;
	int max_x_index = 0, min_x_index = 0, max_y_index = 0, min_y_index = 0;
	float positive_index_sum_x = 0, positive_index_count_x = 0, negative_index_sum_x = 0, negative_index_count_x = 0;
	float positive_index_sum_y = 0, positive_index_count_y = 0, negative_index_sum_y = 0, negative_index_count_y = 0;

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
			if(x[i] > 0){
				positive_index_sum_x += i;
				positive_index_count_x++;
			}
			else{
				negative_index_sum_x += i;
				negative_index_count_x++;
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
			if(y[i] > 0){
				positive_index_sum_y += i;
				positive_index_count_y++;
			}
			else{
				negative_index_sum_y += i;
				negative_index_count_y++;
			}
		}
		if(fabsf(max_y) >= fabsf(min_y))
			max_abs_y = fabsf(max_y);
		else
			max_abs_y = fabsf(min_y);
	}

	int type; //0: x, 1: y

	if(recent_acc_x_count >= recent_acc_y_count)
		type = 0;
	else
		type = 1;

	if(type == 0){
		if( /*max_x_index > min_x_index*/ positive_index_sum_x / positive_index_count_x > negative_index_sum_x / negative_index_count_x)
			sprintf(buf, "LEFT");
		else
			sprintf(buf, "RIGHT");
	}
	else{
		if( /*max_y_index > min_y_index*/ positive_index_sum_y / positive_index_count_y > negative_index_sum_y / negative_index_count_y)
			sprintf(buf, "DOWN");
		else
			sprintf(buf, "UP");
	}

	char s1[100] = "<font_size=20><align=center>";
	char s2[100] = "<font_size=20><align=center>";
	//sprintf(s1, "X: %d / Y: %d", recent_acc_x_count, recent_acc_y_count);
	//sprintf(s2, "MAX X: %0.1f / MAX Y: %0.1f", max_x - min_x, max_y - min_y);
	for (int i = 0; i < recent_acc_x_count; i++){
		char temp[10];
		sprintf(temp, "%0.1f ", x[i]);
		strcat(s1, temp);
	}
	for (int i = 0; i < recent_acc_y_count; i++){
		char temp[10];
		sprintf(temp, "%0.1f ", y[i]);
		strcat(s2, temp);
	}
//	sprintf(s1, "<font_size=20><align=center>%s</align></font_size>", s1);
//	sprintf(s2, "<font_size=20><align=center>%s</align></font_size>", s2);
	strcat(s1, "</align></font_size>");
	strcat(s2, "</align></font_size>");
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

static void check_obstacle(appdata_s *ad){
	if(ad->grid_state[ad->user_state[0]][ad->user_state[1]][4] == 0){
		if((ad->user_state[0]==0 && ad->user_state[1]==0) || (ad->user_state[0]==4 && ad->user_state[1]==4) )
		{
			elm_object_text_set(ad->title, "<font_size = 50><align=center>JUMP start!</align></font_size>");

			//connect accelerometer
			ad->sensor_status[0] = 0;
			jump_flag=1;
			start_acceleration_sensor(ad);

			ad->sensor_status[0] = 2; //play mode
			is_obstacle=1;
		}
		else if(ad->user_state[0]==1 && ad->user_state[1]==2 )
		{	//show Heart image
			elm_object_text_set(ad->title, "<font_size = 50><align=center>Raise heart rate!</align></font_size>");


			//connect heart rate monitor
			heart_flag = 1;
			ad->sensor_status[2] = 0;
			start_heartrate_sensor(ad);

			ad->sensor_status[2] = 2; //play mode
			is_obstacle=1;
		}
		else if((ad->user_state[0]==2 && ad->user_state[1]==1) || (ad->user_state[0]==3 && ad->user_state[1]==3) )
		{	//show Bug image

			//connect shake sensor(accelerometer)
			shake_flag=1;
			ad->sensor_status[0] = 0;
			start_acceleration_sensor(ad);

			ad->sensor_status[0] = 2;
			is_obstacle=1;
		}
	}
	//return 0;
}


static void
_new_sensor_value_acc_jump(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	appdata_s *ad = user_data;

	float z = sensor_data->values[2];
	//elm_object_text_set(ad->title, "<font_size = 50><align=center>JUMP start!</align></font_size>");

	if(jmp_detect==0 && jump_flag == 1)
	{
		ad->grid2 = elm_grid_add(ad->win);
		evas_object_size_hint_weight_set(ad->grid2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_object_content_set(ad->conform, ad->grid2);
		evas_object_show(ad->grid2);


		ad->title2 = elm_label_add(ad->grid2);
		elm_grid_pack(ad->grid2, ad->title2, 5, 10, 100, 20);
		evas_object_show(ad->title2);


		elm_object_text_set(ad->title2, "<font_size = 50><align=center>JUMP first!</align></font_size>");
		char img_path[PATH_MAX] = "";
		app_get_resource("jump.png", img_path, PATH_MAX);
		jmp= evas_object_image_filled_add(ad->canvas);
		evas_object_image_file_set(jmp, img_path, NULL);
		elm_grid_pack(ad->grid2, jmp, 15, 25, 75, 75);
		evas_object_show(jmp);
		//evas_object_hide(jmp);
		//elm_grid_unpack(ad->grid, jmp);


	}
	else{
		evas_object_show(ad->grid);
	}
	if((z>10 || z<-10) && jump_flag == 1)
	 {
		 elm_object_text_set(ad->title2, "<font_size = 50><align=center>JUMP detected!</align></font_size>");
		 jmp_detect=1;
		 jump_flag = 0;

		 is_obstacle = 0;

		 ad->user_state[2]++;

		 start_acceleration_sensor(ad);

		 draw_map(ad);
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
	 Evas_Object *bug;
	 if(shake_detect==0 && shake_flag ==1)//show image
	 {
		 snprintf(buf, sizeof(buf ), "<font_size = 10><align=center>Bugs:%d</align></font_size>", bug_num);
		 elm_object_text_set(ad->title, buf);
		 char img_path[PATH_MAX] = "";
		 app_get_resource("shake.png", img_path, PATH_MAX);
		 bug = evas_object_image_filled_add(ad->canvas);
		 evas_object_image_file_set(bug, img_path, NULL);
		 elm_grid_pack(ad->grid, bug, 15, 25, 75, 75);
		 evas_object_show(bug);

		 if(fabsf(x)>20 || fabsf(y)>20 || fabsf(z)>20)
		 {
			 bug_num --;
			 shake_cnt++;
			 snprintf(buf, sizeof(buf ), "<font_size = 10><align=center>Bugs:%d</align></font_size>", bug_num);
			 elm_object_text_set(ad->title, buf);

		 }
	 }
	 if(shake_cnt>=10 && shake_flag == 1)//hide image
	 {
		 snprintf(buf, sizeof(buf ), "<font_size = 10><align=center>Shake complete!</align></font_size>");
		 elm_object_text_set(ad->title, buf);
		 shake_detect=1;
		 shake_cnt = 0;
		 shake_flag = 0;

		 //elm_grid_pack(ad->grid, bug, 15, 25, 75, 75);
		 //evas_object_show(bug);
		 is_obstacle = 0;

		 ad->user_state[2]++;

		 start_acceleration_sensor(ad);

		 draw_map(ad);
	 }
}

static void
_new_sensor_value_acc(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	wait = 1;

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
			 if(ad->user_state[2] == ad->stage_size * ad->stage_size){
				 sprintf(buf_title, "<font_size=20><align=center>TIME: %d <br>CLEAR!!!</align></font_size>", ad->time);
				 ecore_timer_del(ad->timer);
				 device_haptic_vibrate(ad->handle, 1000, 100, &ad->effect_handle); //vibration
			 }
			 else
				 sprintf(buf_title, "<font_size=20><align=center>TIME: %d <br>BUBBLE: %d/%d</align></font_size>", ad->time, ad->user_state[2], ad->stage_size * ad->stage_size);

			 elm_object_text_set(ad->title, buf_title);
		 }


		//test mode
		 else{
			 snprintf(buf, sizeof(buf ), "<font_size = 10>X:%0.1f/Y:%0.1f/Z:%0.1f</font_size>", x, y, z);
			 elm_object_text_set(ad->sensor_label[0], buf);
		 }



		 if(fabsf(x) >= 1.5 && fabsf(x) > fabsf(y) && fabsf(gyro[0]) < 20 && fabsf(gyro[1]) < 10){
			 recent_acc_x[recent_acc_x_count] = x;
			 recent_acc_x_count++;
			 maybe_stop_count = 0;
		 }
		 if((y >= 1.5 || y <= -1) && fabsf(x) < fabsf(y) && fabsf(gyro[0]) < 20 && fabsf(gyro[1]) < 10){
			 recent_acc_y[recent_acc_y_count] = y;
			 recent_acc_y_count++;
			 maybe_stop_count = 0;
		 }
		 if(fabsf(x) < 0.5 && fabsf(y) < 0.5){
			 maybe_stop_count++;
		 }
		 if(maybe_stop_count == 3 && (recent_acc_x_count >= MIN_ACC_DATA || recent_acc_y_count >= MIN_ACC_DATA)){
			 sprintf(buf, direction(ad, recent_acc_x, recent_acc_y));
			 elm_object_text_set(ad->sensor_label[3], buf);

			 maybe_stop_count = 0;
			 recent_acc_x_count = 0;
			 recent_acc_y_count = 0;

			 //play mode
			 if(ad->sensor_status[0] == 2  && is_obstacle == 0){
				 if(strcmp(buf, "UP") == 0 && can_move(ad, 1) == 1){
					 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * (ad->user_state[1] - 1) + ad->user_state[0]);
					 ad->user_state[1]--;
					 sprintf(buf, "?");
					 check_obstacle(ad);
					 if(!is_obstacle)
						 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
				 }
				 else if(strcmp(buf, "DOWN") == 0 && can_move(ad, 2) == 1){
					 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * (ad->user_state[1] + 1) + ad->user_state[0]);
					 ad->user_state[1]++;
					 sprintf(buf, "?");
					 check_obstacle(ad);
					 if(!is_obstacle)
						 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
				 }
				 else if(strcmp(buf, "LEFT") == 0 && can_move(ad, 3) == 1){
					 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * ad->user_state[1] + (ad->user_state[0] - 1));
					 ad->user_state[0]--;
					 sprintf(buf, "?");
					 check_obstacle(ad);
					 if(!is_obstacle)
						 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
				 }
				 else if(strcmp(buf, "RIGHT") == 0 && can_move(ad, 4) == 1){
					 move(ad, ad->stage_size * ad->user_state[1] + ad->user_state[0], ad->stage_size * ad->user_state[1] + (ad->user_state[0] + 1));
					 ad->user_state[0]++;
					 sprintf(buf, "?");
					 check_obstacle(ad);
					 if(!is_obstacle)
						 bubble_pop(ad, ad->user_state[0], ad->user_state[1]);
				 }
				 //is_obstacle=0;
			 }


		 }
		 if(maybe_stop_count >= 3){
			 recent_acc_x_count = 0;
			 recent_acc_y_count = 0;
		 }


	 }
	 wait = 0;
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


	 if(ad->sensor_status[2] == 2 && heart_flag == 1){ //play mode

		 ad->grid2 = elm_grid_add(ad->win);
		 evas_object_size_hint_weight_set(ad->grid2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		 elm_object_content_set(ad->conform, ad->grid2);
		 evas_object_show(ad->grid2);

		 ad->title2 = elm_label_add(ad->grid2);
		 elm_grid_pack(ad->grid2, ad->title2, 5, 10, 100, 20);
		 evas_object_show(ad->title2);
		 elm_object_text_set(ad->title2, "<font_size = 50><align=center>Raise heart rate!</align></font_size>");

		 char buf[1024];
		 if (sensor_data->value_count < 1)
		 {
			 elm_object_text_set(ad->sensor_label[1], "Gathering data...");
			 return;
		 }

		 char img_path[PATH_MAX] = "";
		 app_get_resource("heart.png", img_path, PATH_MAX);
		 Evas_Object *heart = evas_object_image_filled_add(ad->canvas);
		 evas_object_image_file_set(heart, img_path, NULL);
		 elm_grid_pack(ad->grid2, heart, 15, 25, 75, 75);
		 evas_object_show(heart);

		 //if(ad->sensor_status[2] == 1){
			 //snprintf(buf, sizeof(buf), "<font_size = 10>HEART RATE: %d</font_size>", hr);
			 //elm_object_text_set(ad->title, buf);
		// }
		 if(hr>30 && initial_flag)
		 {
			 initial_beat=hr;
			 initial_flag=0;
			 //snprintf(buf, sizeof(buf), "<font_size = 10>Initial: %d</font_size>", hr);
			 //elm_object_text_set(ad->sensor_label[1], buf);
		 }
		 if(!initial_flag && hr> initial_beat+5 && heart_flag == 1)
		 {
			 //snprintf(buf, sizeof(buf), "<font_size = 10>Initial: %d</font_size>", hr);
			 //elm_object_text_set(ad->sensor_label[2], buf);
			 snprintf(buf, sizeof(buf), "<font_size = 10>WALL BREAK!</font_size>");
			 elm_object_text_set(ad->title2, buf);

			 heart_flag = 0;

			 is_obstacle = 0;

			 ad->user_state[2]++;

			 start_acceleration_sensor(ad);

			 draw_map(ad);
		 }
	 }
}

static void
start_acceleration_sensor(appdata_s *ad)
{
	if(ad->sensor_status[0] != -1 && ad->sensor_status[0] != 1 && ad->sensor_status[0] != 2){
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
		else if(wait >= 0)
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
}

static void
start_gyroscope_sensor(appdata_s *ad)
{
	if(ad->sensor_status[1] != -1 && ad->sensor_status[1] != 1 && ad->sensor_status[1] != 2){
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
}

static void
start_heartrate_sensor(appdata_s *ad)
{
	if(ad->sensor_status[2] != -1 && ad->sensor_status[2] != 1 && ad->sensor_status[2] != 2){
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
	elm_list_item_append(ad->sensor_list, "VIBE TEST", NULL, NULL, vibe_test_cb, ad);
	evas_object_show(ad->sensor_list);
	elm_box_pack_after(ad->box, ad->sensor_list, ad->box_title);


	//evas_object_event_callback_add(ad->conform, EVAS_CALLBACK_MOUSE_DOWN, main_menu_cb, ad);
	//evas_object_show(ad->back);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void vibe_test_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>VIBE TEST</align></font_size>");

	evas_object_hide(ad->sensor_list);
	elm_box_unpack(ad->box, ad->sensor_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);

	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}

	int error, num;
	error = device_haptic_get_count(&num);
	haptic_device_h handle;
	haptic_effect_h effect_handle;
	error = device_haptic_open(0, &handle);
	error = device_haptic_vibrate(handle, 1000, 100, &effect_handle);

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


	//show_is_supported(ad);


	jump_flag=1;
	start_acceleration_sensor(ad);
	ad->sensor_status[0] = 2;

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


	//show_is_supported(ad);


	shake_flag=1;
	start_acceleration_sensor(ad);
	ad->sensor_status[0] = 2;

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


	//show_is_supported(ad);


	start_acceleration_sensor(ad);
	start_gyroscope_sensor(ad);
	ad->sensor_status[0] = 1;
	ad->sensor_status[1] = 1;

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


	start_heartrate_sensor(ad);
	ad->sensor_status[2] = 2;

	for(int i = 0; i < 4; i++){
		ad->sensor_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->sensor_label[i], "?");
		evas_object_show(ad->sensor_label[i]);
		elm_box_pack_end(ad->box_content, ad->sensor_label[i]);
	}
	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}
