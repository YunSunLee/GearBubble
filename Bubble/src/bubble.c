#include "bubble.h"
#include <sensor.h>
#include "single_play.c"
#include "network.c"
#include "map_editor.c"
#include "ranking.c"
#include "tutorial.c"
#include "sensor_test.c"
#include "make_map.c"
#include "sound.c"

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

static void
main_menu_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	if((ad->user_state[2] == ad->stage_size * ad->stage_size) &(ad->time>0)){
		read_rank_file(ad); // get previous record

		 /* store time */
		int stage = ad->stage_num; // get stage

		if (ad->ranking[5*stage-1] > ad->time){
		 	 ad->ranking[5*stage-1] =  ad->time;

		 	 for (int i = 5*stage-1 ; i>5*(stage-1); i--){
				 if(ad->ranking[i-1] > ad->ranking[i]){
		 			 swap(&ad->ranking[i-1], &ad->ranking[i]);
		 		 }
		 	 }
		}
		write_rank_file(ad);
	}

	ad->sensor_status[0] = -1;
	ad->sensor_status[1] = -1;
	ad->sensor_status[2] = -1;
	ad->user_state[2] = 0;
	ecore_timer_del(ad->timer);
	create_base_gui(ad);
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
		evas_object_size_hint_weight_set(ad->box_title, EVAS_HINT_EXPAND, 0.3);
		evas_object_show(ad->box_title);
		elm_box_pack_end(ad->box, ad->box_title);

		//title

		ad->title = elm_label_add(ad->box_title);
		elm_object_text_set(ad->title, "<font_size = 50><align=center>GEAR BUBBLE</align></font_size>");
		elm_box_pack_end(ad->box_title, ad->title);
		evas_object_show(ad->title);

		ad->box_content = elm_box_add(ad->conform);
		evas_object_size_hint_weight_set(ad->box_content, EVAS_HINT_EXPAND, 0.5);
		evas_object_show(ad->box_content);


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
		elm_list_item_append(ad->back_list, "MENU", NULL, NULL, main_menu_cb, ad);


		ad->stage_list = elm_list_add(ad->box);
		evas_object_size_hint_weight_set(ad->stage_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(ad->stage_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_list_item_append(ad->stage_list, "STAGE 1", NULL, NULL, stage1_cb, ad);
		elm_list_item_append(ad->stage_list, "STAGE 2", NULL, NULL, stage2_cb, ad);
		elm_list_item_append(ad->stage_list, "STAGE 3", NULL, NULL, stage3_cb, ad);
		elm_list_item_append(ad->stage_list, "STAGE 4", NULL, NULL, stage4_cb, ad);
		elm_list_item_append(ad->stage_list, "STAGE 5", NULL, NULL, stage5_cb, ad);

		ad->stage = elm_list_add(ad->box);
		evas_object_size_hint_weight_set(ad->stage, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(ad->stage, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_list_item_append(ad->stage, "START", NULL, NULL, map_creater_cb, ad);

		eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_delete_request_cb, ad); //back button == exit

		/* Show window after base gui is set up */
		evas_object_show(ad->win);

		/* Create player */
		ad->player = create_player();

		/* Load audio file to Player */
		prepare_player(ad, 0);
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

//initial sensors
ad.sensor_status[0] = 0;
ad.sensor_status[1] = 0;
ad.sensor_status[2] = 0;

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
