/*
 * sound.c
 *
 *  Created on: Apr 13, 2018
 *      Author: yunsun
 */

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
