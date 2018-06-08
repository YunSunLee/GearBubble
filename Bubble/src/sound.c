#include "bubble.h"

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

void sound_cb(void *data, Evas_Object *obj, void *event_info)
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


/* Get player state */
player_state_e get_player_state(player_h player)
{
	player_state_e state;
	player_get_state(player, &state);
	return state;
}

/* Play completed event function */
static void
on_player_completed(player_h* player)
{
	if(player)
		player_stop(player);
}

/* Create Player */
player_h create_player()
{
	player_h player;
	player_create(&player);
	player_set_completed_cb(player, on_player_completed, player);
	return player;
}


/* Get full path of source file */
static inline const char*
get_resource_path(const char *file_path)
{
	static char absolute_path[PATH_MAX] = {'\0'};
	static char *res_path_buff = NULL;
	if(res_path_buff == NULL)
		res_path_buff = app_get_resource_path();
	snprintf(absolute_path, PATH_MAX, "%s%s", res_path_buff, file_path);
	return absolute_path;
}

/* Load file to Player */

void
prepare_player(appdata_s* ad, int index)
{
	// Stop play
	stop_player(ad);
	// Close file
	player_unprepare(ad->player);
	const char* file_name[] = { "bubble_pop.wav" };

	const char* file = file_name[index];
	// Get full path of source file
	const char *res_path = get_resource_path(file);
	// Load file
	player_set_uri(ad->player, res_path);
	// Prepare play
	int result = player_prepare(ad->player);
	dlog_print(DLOG_INFO, "tag", "File load : %d", result);
}

/* Stop play */
void
stop_player(void *data)
{
	appdata_s *ad = data;
	if( get_player_state(ad->player) == PLAYER_STATE_PLAYING || get_player_state(ad->player) == PLAYER_STATE_PAUSED)
	{
		player_stop(ad->player);
	}
}

/* Start play */
void
start_player(void *data)
{
	appdata_s *ad = data;
	if( get_player_state(ad->player) != PLAYER_STATE_PLAYING)
		player_start(ad->player);
}
