#include "sensoracceleration.h"
#include <sensor.h>

typedef struct {
sensor_h sensor; /**< Sensor handle */
sensor_listener_h sensor_listener;
} sensorinfo;

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *label0;
	Evas_Object *label1;
	Evas_Object *label2;
	sensorinfo sensor_info;
} appdata_s;

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

static void show_is_supported(appdata_s *ad)
{
	char buf[PATH_MAX];
	bool is_supported = false;
	sensor_is_supported(SENSOR_ACCELEROMETER, &is_supported);
	sprintf(buf, "Acceleration Sensor is %s", is_supported ? "support" : "not support");
	elm_object_text_set(ad->label0, buf);
}

static void
my_box_pack(Evas_Object *box, Evas_Object *child,
double h_weight, double v_weight, double h_align, double v_align)
{
	/* create a frame we shall use as padding around the child widget */
	Evas_Object *frame;
	//= elm_frame_add(box);
	/* use the medium padding style. there is "pad_small", "pad_medium",
	 * "pad_large" and "pad_huge" available as styles in addition to the
	 * "default" frame style */
	elm_object_style_set(frame, "pad_small");
	/* set the input weight/aling on the frame insted of the child */
	evas_object_size_hint_weight_set(frame, h_weight, v_weight);
	evas_object_size_hint_align_set(frame, h_align, v_align);
	{
		/* tell the child that is packed into the frame to be able to expand */
		evas_object_size_hint_weight_set(child, EVAS_HINT_EXPAND,EVAS_HINT_EXPAND);
		/* fill the expanded area (above) as opposaed to center in it */
		evas_object_size_hint_align_set(child, EVAS_HINT_FILL, EVAS_HINT_FILL);
		/* actually put the child in the frame and show it */
		evas_object_show(child);
		elm_object_content_set(frame, child);
	}
	/* put the frame into the box instead of the child directly */
	elm_box_pack_end(box, frame);

	/* show the frame */
	evas_object_show(frame);
}

static void
_new_sensor_value(sensor_h sensor, sensor_event_s *sensor_data, void *user_data)
{
	appdata_s *ad = user_data;
	char buf[1024];

	if (sensor_data->value_count < 3)
	{
		elm_object_text_set(ad->label2, "Gathering data...");
		return;
	}

	snprintf(buf, sizeof(buf), "Value - X : %0.1f / Y : %0.1f / Z : %0.1f", sensor_data->values[0], sensor_data->values[1], sensor_data->values[2]);
	elm_object_text_set(ad->label1, buf);
}

static void start_acceleration_sensor(appdata_s *ad)
{
	sensor_error_e err = SENSOR_ERROR_NONE;
	err = sensor_get_default_sensor(SENSOR_ACCELEROMETER, &ad->sensor_info.sensor);
	if (err != SENSOR_ERROR_NONE)
		goto error_check;

	err = sensor_create_listener(ad->sensor_info.sensor, &ad->sensor_info.sensor_listener);

	if (err != SENSOR_ERROR_NONE)
		goto error_check;
	sensor_listener_set_event_cb(ad->sensor_info.sensor_listener, 100, _new_sensor_value, ad);
	sensor_listener_start(ad->sensor_info.sensor_listener);

	error_check:
	if (err != SENSOR_ERROR_NONE)
	{
		const char *msg;
		char fullmsg[1024];
		switch (err)
		{
			case SENSOR_ERROR_IO_ERROR: msg = "I/O error";
				break;
			case SENSOR_ERROR_INVALID_PARAMETER: msg = "Invalid parameter";
				break;
			case SENSOR_ERROR_NOT_SUPPORTED: msg = "The sensor type is not supported in the current device";
				break;
			case SENSOR_ERROR_PERMISSION_DENIED: msg = "Permission denied";
				break;
			case SENSOR_ERROR_OUT_OF_MEMORY: msg = "Out of memory";
				break;
			case SENSOR_ERROR_NOT_NEED_CALIBRATION: msg = "Sensor doesn't need calibration";
				break;
			case SENSOR_ERROR_OPERATION_FAILED: msg = "Operation failed";
				break;
			default: msg = "Unknown error";
				break;
		}
		snprintf(fullmsg, sizeof(fullmsg), "<align=center>An error occurred:<br/>%s</>", msg);
		elm_object_text_set(ad->label1, "No data");
		elm_object_text_set(ad->label2, fullmsg);
	}
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

	{ /* child object - indent to how relationship */
		Evas_Object * box, *btn;
		/* A box to put things in verticallly - default mode for box */
		box = elm_box_add(ad->win);
		evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_object_content_set(ad->conform, box);
		evas_object_show(box);

		{ /* child object - indent to how relationship */
			/* Label-0 */
			ad->label0 = elm_label_add(ad->conform);
			elm_object_text_set(ad->label0, "Msg - ");
			my_box_pack(box, ad->label0, 1.0, 0.0, -1.0, 0.0);

			/* Label-1 */
			ad->label1 = elm_label_add(ad->conform);
			elm_object_text_set(ad->label1, "Value - ");
			my_box_pack(box, ad->label1, 1.0, 0.0, -1.0, 0.0);
		}
	}


	/* Show window after base gui is set up */
	evas_object_show(ad->win);
	show_is_supported(ad);
	start_acceleration_sensor(ad);
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
