/*
 * network.c
 *
 *  Created on: Apr 14, 2018
 *      Author: june
 */

#include "bubble.h"
#include "bt_chat.c"

void network_play_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	ad->is_network = 1;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>2PLAYERS</align></font_size>");
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);
	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	//network play

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);

	_create_base_gui(ad);
}
