#include "bubble.h"
#include <sensor.h>
#include "single_play.c"
#include "network.c"
#include "map_editor.c"
#include "ranking.c"
#include "tutorial.c"

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void create_base_gui(appdata_s *ad);

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
main_menu_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	create_base_gui(ad);
}

static void single_mode_cb(void *data, Evas_Object *obj, void *event_info){}
static void vs_com_cb(void *data, Evas_Object *obj, void *event_info){}

static void stage_size_3_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage_size_4_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage_size_5_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage_size_6_cb(void *data, Evas_Object *obj, void *event_info){}

static void stage1_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage2_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage3_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage4_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage5_cb(void *data, Evas_Object *obj, void *event_info){}


static void
sound_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	Eina_Bool state = elm_check_state_get(obj);
	if(state)
		ad->sound = 1;
	else
		ad->sound = 0;
}

static void
sound_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>SOUND</align></font_size>");
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);
	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	ad->sound_check = elm_check_add(ad->box_content);
	elm_object_style_set(ad->sound_check,"popup");
	//elm_object_text_set(ad->sound_check, "SOUND");

	if(ad->sound == 0)
		elm_check_state_set(ad->sound_check, EINA_FALSE);
	else
		elm_check_state_set(ad->sound_check, EINA_TRUE);

	elm_box_pack_end(ad->box_content, ad->sound_check);
	evas_object_show(ad->sound_check);

	evas_object_smart_callback_add(ad->sound_check, "changed", sound_changed_cb, ad);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);

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

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Box */

	ad->box = elm_box_add(ad->conform);
	evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND,	EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, ad->box);
	evas_object_show(ad->box);

	ad->box_title = elm_box_add(ad->box);
	evas_object_size_hint_weight_set(ad->box_title, EVAS_HINT_EXPAND, 0.2);
	evas_object_show(ad->box_title);
	elm_box_pack_end(ad->box, ad->box_title);

	//title

	ad->title = elm_label_add(ad->box_title);
	elm_object_text_set(ad->title, "<font_size = 50><align=center>GEAR BUBBLE</align></font_size>");
	elm_box_pack_end(ad->box_title, ad->title);
	evas_object_show(ad->title);

	ad->box_content = elm_box_add(ad->conform);
	evas_object_size_hint_weight_set(ad->box_content, EVAS_HINT_EXPAND, 0.6);
	evas_object_show(ad->box_content);
//	elm_box_pack_end(ad->box, ad->box_content);

	/* List */

	/* Create the list */
	ad->main_list = elm_list_add(ad->box);
	/* Set the list size */
	evas_object_size_hint_weight_set(ad->main_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->main_list, EVAS_HINT_FILL, EVAS_HINT_FILL);

	/* Add an item to the list */
	elm_list_item_append(ad->main_list, "1PLAYER", NULL, NULL, single_play_cb, ad);
	elm_list_item_append(ad->main_list, "2PLAYERS", NULL, NULL, network_play_cb, ad);
	elm_list_item_append(ad->main_list, "MAP EDITOR", NULL, NULL, map_editor_cb, ad);
	elm_list_item_append(ad->main_list, "TUTORIAL", NULL, NULL, tutorial_cb, ad);
	elm_list_item_append(ad->main_list, "RANKING", NULL, NULL, ranking_cb, ad);
	elm_list_item_append(ad->main_list, "SOUND", NULL, NULL, sound_cb, ad);
	elm_list_item_append(ad->main_list, "SENSOR", NULL, NULL, sensor_test_cb, ad);

	/* Show and add to box */
	evas_object_show(ad->main_list);
	elm_box_pack_end(ad->box, ad->main_list);


	ad->box_bottom = elm_box_add(ad->conform);
	evas_object_size_hint_weight_set(ad->box_bottom, EVAS_HINT_EXPAND, 0.2);
	evas_object_show(ad->box_bottom);
	elm_box_pack_end(ad->box, ad->box_bottom);

	//main menu button(text)


	ad->bottom = elm_label_add(ad->box_bottom);

	if(ad->sound == 0)
		elm_object_text_set(ad->bottom, _("<font_size=20><align=center>SOUND OFF</align></font_size>"));
	else
		elm_object_text_set(ad->bottom, _("<font_size=20><align=center>SOUND ON</align></font_size>"));

	elm_box_pack_end(ad->box_bottom, ad->bottom);
	evas_object_show(ad->bottom);

	ad->back_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->back_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->back_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->back_list, "BACK", NULL, NULL, main_menu_cb, ad);

	ad->single_mode_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->single_mode_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->single_mode_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->single_mode_list, "SINGLE", NULL, NULL, single_mode_cb, ad);
	elm_list_item_append(ad->single_mode_list, "VS. COM", NULL, NULL, vs_com_cb, ad);

	ad->stage_size_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->stage_size_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->stage_size_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->stage_size_list, "3X3", NULL, NULL, stage_size_3_cb, ad);
	elm_list_item_append(ad->stage_size_list, "4X4", NULL, NULL, stage_size_4_cb, ad);
	elm_list_item_append(ad->stage_size_list, "5X5", NULL, NULL, stage_size_5_cb, ad);
	elm_list_item_append(ad->stage_size_list, "6X6", NULL, NULL, stage_size_6_cb, ad);

	ad->stage_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->stage_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->stage_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->stage_list, "STAGE 1", NULL, NULL, stage1_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 2", NULL, NULL, stage2_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 3", NULL, NULL, stage3_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 4", NULL, NULL, stage4_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 5", NULL, NULL, stage5_cb, ad);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;

}
