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

	ad->stage_size_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->stage_size_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->stage_size_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->stage_size_list, "3X3", NULL, NULL, stage_size_3_cb, ad);
	elm_list_item_append(ad->stage_size_list, "4X4", NULL, NULL, stage_size_4_cb, ad);
	elm_list_item_append(ad->stage_size_list, "5X5", NULL, NULL, stage_size_5_cb, ad);
	evas_object_show(ad->stage_size_list);
	elm_box_pack_after(ad->box, ad->stage_size_list, ad->box_title);

	ad->stage_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->stage_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->stage_list, EVAS_HINT_FILL, EVAS_HINT_FILL);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);
}

/* stage size call back function */
static void stage_size_common(void *data, int stage_size){
	appdata_s *ad = data;
	ad->stage_size = stage_size;

	/* initialize grid state */
	for(int i =0; i< stage_size; i++){
		for (int j=0; j<stage_size; j++){
			for (int k=0; k<6; k++){
				ad->grid_state[i][j][k]=0;
			}
		}
	}

	evas_object_hide(ad->stage_size_list);
	elm_box_unpack(ad->box, ad->stage_size_list);

	evas_object_show(ad->stage_list);
	elm_box_pack_after(ad->box, ad->stage_list, ad->box_title);
}

static void stage_size_3_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>3 x 3</align></font_size>");
	elm_list_item_append(ad->stage_list, "STAGE 1", NULL, NULL, stage1_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 2", NULL, NULL, stage2_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 3", NULL, NULL, stage3_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 4", NULL, NULL, stage4_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 5", NULL, NULL, stage5_cb, ad);
	stage_size_common(ad,3);
}

static void stage_size_4_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>4 x 4</align></font_size>");
	stage_size_common(ad,4);
	elm_list_clear (ad->stage_list);
	elm_list_item_append(ad->stage_list, "STAGE 6", NULL, NULL, stage6_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 7", NULL, NULL, stage7_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 8", NULL, NULL, stage8_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 9", NULL, NULL, stage9_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 10", NULL, NULL, stage10_cb, ad);
}

static void stage_size_5_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>4 x 4</align></font_size>");
	stage_size_common(ad,5);
	elm_list_clear (ad->stage_list);
	elm_list_item_append(ad->stage_list, "STAGE 11", NULL, NULL, stage11_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 12", NULL, NULL, stage12_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 13", NULL, NULL, stage13_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 14", NULL, NULL, stage14_cb, ad);
	elm_list_item_append(ad->stage_list, "STAGE 15", NULL, NULL, stage15_cb, ad);
}


/* stage call back function */
static void stage_common(void *data, int stage){
	appdata_s* ad = data;
	ad-> stage_num = stage;

	evas_object_hide(ad->stage_list);
	elm_box_unpack(ad->box, ad->stage_list);

	evas_object_show(ad->stage);
	elm_box_pack_after(ad->box, ad->stage, ad->box_title);
}
static void stage1_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE1</align></font_size>");
	/* challenger */
	ad->grid_state[0][0][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[1][1][5] = 3; /* bug */
	stage_common(ad, 1);
}
static void stage2_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE2</align></font_size>");
	/* challenger */
	ad->grid_state[0][0][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[1][1][5] = 3; /* bug */
	stage_common(ad, 2);
}
static void stage3_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE3</align></font_size>");
	/* challenger */
	ad->grid_state[0][2][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[2][1][5] = 3; /* bug */
	stage_common(ad, 3);
}
static void stage4_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE4</align></font_size>");
	/* challenger */
	ad->grid_state[2][2][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[1][1][5] = 3; /* bug */
	stage_common(ad, 4);
}
static void stage5_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE5</align></font_size>");
	/* challenger */
	ad->grid_state[0][1][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[1][2][5] = 3; /* bug */
	stage_common(ad, 5);
}

static void stage6_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE6</align></font_size>");
	/* challenger */
	ad->grid_state[0][1][5]= 1; /* hurdle */
	//ad->grid_state[0][0][5]= 2; /* heart */
	ad->grid_state[1][2][5] = 3; /* bug */
	stage_common(ad, 1);
}
static void stage7_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE7</align></font_size>");
	stage_common(ad, 2);
}
static void stage8_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE8</align></font_size>");
	stage_common(ad, 3);
}
static void stage9_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE9</align></font_size>");
	stage_common(ad, 4);
}
static void stage10_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE10</align></font_size>");
	stage_common(ad, 5);
}

static void stage11_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE11</align></font_size>");
	stage_common(ad, 1);
}
static void stage12_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE12</align></font_size>");
	stage_common(ad, 2);
}
static void stage13_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE13</align></font_size>");
	stage_common(ad, 3);
}
static void stage14_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE14</align></font_size>");
	stage_common(ad, 4);
}
static void stage15_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE15</align></font_size>");
	stage_common(ad, 5);
}
