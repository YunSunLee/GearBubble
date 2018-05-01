/*
 * single_play.c
 *
 *  Created on: Apr 9, 2018
 *      Author: june
 */
#include "bubble.h"


static void
single_play_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;



	elm_object_text_set(ad->title, "<font_size = 50><align=center>1PLAYER</align></font_size>");

	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	evas_object_hide(ad->bottom);
	elm_box_unpack(ad->box_bottom, ad->bottom);
	//evas_object_hide(ad->box_content);
	//elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);

	ad->stage_size_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->stage_size_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->stage_size_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->stage_size_list, "3X3", NULL, NULL, stage_size_3_cb, ad);
	elm_list_item_append(ad->stage_size_list, "4X4", NULL, NULL, stage_size_4_cb, ad);
	elm_list_item_append(ad->stage_size_list, "5X5", NULL, NULL, stage_size_5_cb, ad);
	evas_object_show(ad->stage_size_list);
	elm_box_pack_after(ad->box, ad->stage_size_list, ad->box_title);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

static void stage_size_3_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	ad->stage_size = 3;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>3x3</align></font_size>");
	evas_object_hide(ad->stage_size_list);
	elm_box_unpack(ad->box, ad->stage_size_list);

	evas_object_show(ad->stage_list);
	elm_box_pack_after(ad->box, ad->stage_list, ad->box_title);
}
static void stage_size_4_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	ad->stage_size = 4;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>4x4</align></font_size>");
	evas_object_hide(ad->stage_size_list);
	elm_box_unpack(ad->box, ad->stage_size_list);

	evas_object_show(ad->stage_list);
	elm_box_pack_after(ad->box, ad->stage_list, ad->box_title);
}
static void stage_size_5_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	ad->stage_size = 5;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>5x5</align></font_size>");
	evas_object_hide(ad->stage_size_list);
	elm_box_unpack(ad->box, ad->stage_size_list);

	evas_object_show(ad->stage_list);
	elm_box_pack_after(ad->box, ad->stage_list, ad->box_title);
}

static void stage1_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>stage1</align></font_size>");
	evas_object_hide(ad->stage_list);
	elm_box_unpack(ad->box, ad->stage_list);

	evas_object_show(ad->stage);
	elm_box_pack_after(ad->box, ad->stage, ad->box_title);
}
static void stage2_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage3_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage4_cb(void *data, Evas_Object *obj, void *event_info){}
static void stage5_cb(void *data, Evas_Object *obj, void *event_info){}
