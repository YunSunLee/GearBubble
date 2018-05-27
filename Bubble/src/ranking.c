/*
 * ranking.c
 *
 *  Created on: Apr 14, 2018
 *      Author: june
 */

#include "bubble.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ranking_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;

	elm_object_text_set(ad->title, "<font_size = 50><align=center>RANKING</align></font_size>");
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	//elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);
	elm_box_unpack(ad->box, ad->bottom);

	ad->ranking_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->ranking_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->ranking_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->ranking_list, "STAGE1", NULL, NULL, ranking_1_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE2", NULL, NULL, ranking_2_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE3", NULL, NULL, ranking_3_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE4", NULL, NULL, ranking_4_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE5", NULL, NULL, ranking_5_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE6", NULL, NULL, ranking_6_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE7", NULL, NULL, ranking_7_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE8", NULL, NULL, ranking_8_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE9", NULL, NULL, ranking_9_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE10", NULL, NULL, ranking_10_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE11", NULL, NULL, ranking_11_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE12", NULL, NULL, ranking_12_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE13", NULL, NULL, ranking_13_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE14", NULL, NULL, ranking_14_cb, ad);
	elm_list_item_append(ad->ranking_list, "STAGE15", NULL, NULL, ranking_15_cb, ad);
	evas_object_show(ad->ranking_list);
	elm_box_pack_after(ad->box, ad->ranking_list, ad->box_title);

	evas_object_show(ad->back_list);
	elm_box_pack_end(ad->box, ad->back_list);

}

static void ranking_1_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE1</align></font_size>");
	ranking_common(ad, 1);
}

static void ranking_2_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE2</align></font_size>");
	ranking_common(ad, 2);
}

static void ranking_3_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE3</align></font_size>");
	ranking_common(ad, 3);
}

static void ranking_4_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE4</align></font_size>");
	ranking_common(ad, 4);
}

static void ranking_5_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE5</align></font_size>");
	ranking_common(ad, 5);
}

static void ranking_6_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE6</align></font_size>");
	ranking_common(ad, 6);
}

static void ranking_7_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE7</align></font_size>");
	ranking_common(ad, 7);
}

static void ranking_8_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE8</align></font_size>");
	ranking_common(ad, 8);
}

static void ranking_9_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE9</align></font_size>");
	ranking_common(ad, 9);
}

static void ranking_10_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE10</align></font_size>");
	ranking_common(ad, 10);
}

static void ranking_11_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE11</align></font_size>");
	ranking_common(ad, 11);
}

static void ranking_12_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE12</align></font_size>");
	ranking_common(ad, 12);
}

static void ranking_13_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE13</align></font_size>");
	ranking_common(ad, 13);
}

static void ranking_14_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE14</align></font_size>");
	ranking_common(ad, 14);
}

static void ranking_15_cb(void *data, Evas_Object *obj, void *event_info){
	appdata_s *ad = data;
	elm_object_text_set(ad->title, "<font_size = 50><align=center>STAGE15</align></font_size>");
	ranking_common(ad, 15);
}

static void ranking_common(void *data, int stage){
	appdata_s *ad = data;
	evas_object_hide(ad->ranking_list);
	elm_box_unpack(ad->box, ad->ranking_list);

	ad->ranking_stage_list = elm_list_add(ad->box);
	evas_object_size_hint_weight_set(ad->ranking_stage_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(ad->ranking_stage_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_item_append(ad->ranking_stage_list,"RANK    TIME", NULL, NULL, NULL, ad);

	read_rank_file(ad);

	char rank_time[80];
	char buf[100];

	for(int i=0; i<5;i++){
		itoa(i+1, buf, 10);
		strcpy(rank_time, "  ");
		strcat(rank_time, buf);
		strcat(rank_time, "         ");
		itoa(ad->ranking[i+5*(stage-1)], buf, 10);
		strcat(rank_time, buf);
		strcat(rank_time, "s");
		elm_list_item_append(ad->ranking_stage_list, rank_time, NULL, NULL, NULL, ad);
	}
	evas_object_show(ad->ranking_stage_list);
	elm_box_pack_after(ad->box, ad->ranking_stage_list, ad->box_title);
}

static void read_rank_file(void *data){
	appdata_s *ad = data;

	FILE *fp;
	char *res;
	char str[80];

	int i=0;
	char filepath[PATH_MAX] = {0,};

	/* open file */
	/* get from data */
	app_get_data("ranking.txt", filepath, PATH_MAX);
	fp = fopen(filepath, "r");
	/* get from res */
	if(fp==NULL){
		app_get_resource("ranking.txt", filepath, PATH_MAX);
		fp = fopen(filepath, "r");
	}
	/* get time */
	while((res=fgets(str, sizeof(str), fp))!=NULL){
		ad->ranking[i] = atoi(res);
		i++;
	}
	fclose(fp);
}

static void write_rank_file(void *data){
	appdata_s *ad = data;
	char buf[100];

	char filepath[PATH_MAX] = {0,};
	app_get_data("ranking.txt", filepath, PATH_MAX);

	FILE *fp;
	fp = fopen(filepath, "w");

	for(int i=0; i<75; i++){
		itoa(ad->ranking[i], buf, 10);
		fputs(buf, fp);
		fputs("\n", fp);
	}

	fclose(fp);
}

static void *itoa(int number, char *str, int radix){
    int digits = 0;
    int divider = 1;
    int index = 0;
    while( (number / divider) > 0) {
        digits++;
        divider *= radix;
    }
    divider /= radix;
    for(; digits > 0; digits--){
        *(str + index) = (number / divider) + '0';
        number -= ((number / divider) * divider);
        index++;
        divider /= radix;
    }
    *(str + index) = '\0';
}

static void app_get_data(const char *res_file_in, char *res_path_out, int res_path_max){
	char *res_path = app_get_data_path();
	if(res_path){
		snprintf(res_path_out, res_path_max, "%s%s", res_path, res_file_in);
		free(res_path);
	}
}

static void app_get_resource(const char *res_file_in, char *res_path_out, int res_path_max){
	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(res_path_out, res_path_max, "%s%s", res_path, res_file_in);
		free(res_path);
	}
}

static void swap(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y = temp;
}
