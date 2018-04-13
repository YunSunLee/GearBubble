/*
 * sensor_test.c
 *
 *  Created on: Apr 11, 2018
 *      Author: yunsun
 */
#include <sensor.h>
#include "bubble.h"

static void show_is_supported(appdata_s *ad)
{
	char buf[PATH_MAX];
	bool is_supported = false;
	sensor_is_supported(SENSOR_ACCELEROMETER, &is_supported);
	sprintf(buf, "Acceleration Sensor is %s", is_supported ? "support" : "not support");
	elm_object_text_set(ad->acc_label[0], buf);
}

static void
_new_sensor_value(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	float x = sensor_data->values[0];
	float y = sensor_data->values[1];
	float z = sensor_data->values[2];

	 appdata_s *ad = user_data;

	 char buf[1024];
	 if (sensor_data->value_count < 3)
	 {
		 elm_object_text_set(ad->acc_label[1], "Gathering data...");
		 return;
	 }
	 snprintf(buf, sizeof(buf ), "<font_size = 10>X:%0.1f/Y:%0.1f/Z:%0.1f</font_size>", x, y, z);
	 elm_object_text_set(ad->acc_label[1], buf);
	 //evas_object_text_text_set(ad->title, buf);
}

static void
start_acceleration_sensor(appdata_s *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	//err = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &ad->sensor_info.sensor);
	err = sensor_get_default_sensor(SENSOR_LINEAR_ACCELERATION, &ad->sensor_info.sensor);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	err = sensor_create_listener(ad->sensor_info.sensor, &ad->sensor_info.sensor_listener);
	if (err != SENSOR_ERROR_NONE)
	goto error_check;
	sensor_listener_set_event_cb(ad->sensor_info.sensor_listener, 100, _new_sensor_value, ad); //INTERVAL
	sensor_listener_start(ad->sensor_info.sensor_listener);
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
		elm_object_text_set(ad->acc_label[0], "No data");
		elm_object_text_set(ad->acc_label[1], fullmsg);
	}
}


static void
sensor_test_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	show_is_supported(ad);
	start_acceleration_sensor(ad);

	elm_object_text_set(ad->title, "<font_size = 50><align=center>SENSOR TEST</align></font_size>");
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);
	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);
	for(int i = 0; i < 2; i++){
		ad->acc_label[i] = elm_label_add(ad->box_content);
		elm_object_text_set(ad->acc_label[i], "Msg - ");
		evas_object_show(ad->acc_label[i]);
		elm_box_pack_end(ad->box_content, ad->acc_label[i]);
	}
	show_is_supported(ad);
	start_acceleration_sensor(ad);

	//evas_object_event_callback_add(ad->conform, EVAS_CALLBACK_MOUSE_DOWN, main_menu_cb, ad);
	//evas_object_show(ad->back);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}
