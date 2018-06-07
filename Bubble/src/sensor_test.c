#include <sensor.h>
#include "bubble.h"

static int wait = 0;
static float gyro[3];
static int maybe_stop_count = 0;
static int ready = 1;
static float px = 0;
static float py = 0;

static int shake_flag=0;
static int initial_flag=1;
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

Evas_Object *wall;

static void move_test_cb(void *data, Evas_Object *obj, void *event_info);
static void heart_rate_test_cb(void *data, Evas_Object *obj, void *event_info);
static void jump_test_cb(void *data, Evas_Object *obj, void *event_info);
static void shake_test_cb(void *data, Evas_Object *obj, void *event_info);
static void vibe_test_cb(void *data, Evas_Object *obj, void *event_info);
static void check_obstacle(appdata_s *ad);
static void make_wall_popup(void *data);
static void popup_timeout(void *data, Evas_Object *obj, void *event_info);

// direction - 1: up, 2: down, 3:left, 4:right
// grid_state[x][y][] - 0: up, 1: down, 2:left, 3:right
static int can_move(appdata_s *ad, int direction){
	/* user_state*/
	int x, y;
	x= ad->user_state[0];
	y= ad->user_state[1];

	/* wall */
	if(direction == 1 && ad->grid_state[x][y][0]==1){
		make_wall_popup(ad);
		return 0;
	}
	else if(direction == 2 && ad->grid_state[x][y][1]==1){
		make_wall_popup(ad);
		return 0;
	}
	else if(direction == 3 && ad->grid_state[x][y][2]==1){
		make_wall_popup(ad);
		return 0;
	}
	else if(direction == 4 && ad->grid_state[x][y][3]==1){
		make_wall_popup(ad);
		return 0;
	}
	else if  (direction == 1 && ad->user_state[1] != 0)
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

static void make_wall_popup(void *data)
{
	appdata_s *ad = data;
	ad->popup = elm_popup_add(ad->grid);
	elm_popup_align_set(ad->popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(ad->popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	elm_object_text_set(ad->popup, "<font_size = 50><align=center>Wall!</align></font_size>");
	evas_object_show(ad->popup);

	char img_path[PATH_MAX] = "";
	app_get_resource("wall.png", img_path, PATH_MAX);
	wall= evas_object_image_filled_add(ad->popup);
	evas_object_image_file_set(wall, img_path, NULL);
	elm_grid_pack(ad->grid, wall, 15, 25, 75, 75);
	evas_object_show(wall);

	elm_popup_timeout_set(ad->popup, 2.0);
	evas_object_smart_callback_add(ad->popup, "timeout", popup_timeout, ad);
}

static void
popup_timeout(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	evas_object_del(obj);
	elm_object_text_set(ad->label, "Time out");
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
		if(ad->sound==1){
			/* Create player */
			ad->player = create_player();

			/* Load audio file to Player */
			prepare_player(ad, 0);
			start_player(ad);
		}
	}
}




static char* direction(float x, float px, float y, float py){

	if(fabsf(x * px) > fabsf(y * py)){
		if(x > 0 && px < 0) return "LEFT";
		else if(x < 0 && px > 0) return "RIGHT";
		else return "?";
	}
	else if(fabsf(x * px) < fabsf(y * py)){
		if(y > 0 && py < 0)	return "DOWN";
		else if(y < 0 && py > 0) return "UP";
		else return "?";
	}
	else return "?";
}

static void check_obstacle(appdata_s *ad){
	/* Place of user */
	int x, y;
	x= ad->user_state[0];
	y= ad->user_state[1];

	if(ad->grid_state[x][y][4] == 0){
		/* hurdle */
		if(ad->grid_state[x][y][5]==1)
		{
			elm_object_text_set(ad->title, "<font_size = 50><align=center>JUMP start!</align></font_size>");

			//connect accelerometer
			ad->sensor_status[0] = 0;
			jump_flag=1;
			jmp_detect=0;
			start_acceleration_sensor(ad);

			ad->sensor_status[0] = 2; //play mode
			is_obstacle=1;
		}
		/* heart */
		else if(ad->grid_state[x][y][5]==3)
		{	//show Heart image


			//elm_object_text_set(ad->title, "<font_size = 50><align=center>Raise heart rate!</align></font_size>");


			//connect heart rate monitor
			heart_flag = 1;
			initial_flag = 1;
			ad->sensor_status[2] = 0;
			start_heartrate_sensor(ad);

			ad->sensor_status[2] = 2; //play mode
			is_obstacle=1;
		}
		/* bug */
		else if(ad->grid_state[x][y][5]==2)
		{	//show Bug image

			//connect shake sensor(accelerometer)
			shake_flag=1;
			bug_num = 10;
			shake_cnt = 0;
			shake_detect = 0;
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

		 ad->grid_state[ad->user_state[0]][ad->user_state[1]][5] = 0;

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

		 ad->grid_state[ad->user_state[0]][ad->user_state[1]][5] = 0;

		 start_acceleration_sensor(ad);

		 draw_map(ad);
	 }
}

static int gyro_check(){
	if(fabsf(gyro[0]) < 20 && fabsf(gyro[1]) < 20 && fabsf(gyro[2]) < 20)
		return 1;
	else
		return 0;
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
			 if(ad->user_state[2] == ad->stage_size * ad->stage_size && ad->is_network == 0){
				 sprintf(buf_title, "<font_size=20><align=center>TIME: %d <br>CLEAR!!!</align></font_size>", ad->time);
				 ecore_timer_del(ad->timer);
			 	 device_haptic_vibrate(ad->handle, 1000, 100, &ad->effect_handle); //vibration
			 }
			 else if(ad->user_state[2] < ad->stage_size * ad->stage_size && ad->is_network == 0)
				 sprintf(buf_title, "<font_size=20><align=center>TIME: %d <br>BUBBLE: %d/%d</align></font_size>", ad->time, ad->user_state[2], ad->stage_size * ad->stage_size);
			 else if(ad->is_network == 1)
				 sprintf(buf_title, "<font_size=20><align=center>YOU: %d <br>FRIEND: %d</align></font_size>", ad->user_state[2], ad->friend_pop_num);

			 elm_object_text_set(ad->title, buf_title);
		 }


		//test mode
		 else{
			 snprintf(buf, sizeof(buf ), "<font_size = 10>X:%0.1f/Y:%0.1f/Z:%0.1f</font_size>", x, y, z);
			 elm_object_text_set(ad->sensor_label[0], buf);
		 }

		 if(fabsf(x) < 0.5 && fabsf(y) < 0.5){
			 maybe_stop_count++;
		 }
		 else
			 maybe_stop_count = 0;

		 //ready to move?? ------- move when green light.....
		 if(ready == 0)
			 evas_object_color_set(ad->rect[ad->stage_size * ad->user_state[1] + ad->user_state[0]], 200, 0, 0, 100);
		 else if(ready == 1)
			 evas_object_color_set(ad->rect[ad->stage_size * ad->user_state[1] + ad->user_state[0]], 0, 200, 0, 100);




		 if(ready == 1 && (fabsf(x) >= 1 || fabsf(y) >= 1) && gyro_check() == 1){
			 sprintf(buf, direction(x, px, y, py));
			 //elm_object_text_set(ad->sensor_label[3], buf);

			 maybe_stop_count = 0;

			 //play mode
			 if(ad->sensor_status[0] == 2  && is_obstacle == 0){

				 ready = 0;

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
				 else
					 ready = 1;
				 //is_obstacle=0;
			 }
		 }

		 if(maybe_stop_count >= 3){
			 ready = 1;
			 px = 0;
			 py = 0;
		 }

		 if(fabsf(x) >= 1 && gyro_check() == 1)
			 px = x;

		 if(fabsf(y) >= 1 && gyro_check() == 1)
			 py = y;
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
		 //elm_object_text_set(ad->title2, "<font_size = 50><align=center>Raise heart rate!</align></font_size>");
		 char temp[100];
		 sprintf(temp, "<font_size = 50><align=center>%d / %d</align></font_size>", hr, initial_beat+5);
		 elm_object_text_set(ad->title2, temp);

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

			 ad->grid_state[ad->user_state[0]][ad->user_state[1]][5] = 0;

			 start_acceleration_sensor(ad);

			 draw_map(ad);
		 }
	 }
}

void start_acceleration_sensor(appdata_s *ad)
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

void start_gyroscope_sensor(appdata_s *ad)
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

void start_heartrate_sensor(appdata_s *ad)
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

void sensor_test_cb(void *data, Evas_Object *obj, void *event_info)
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
