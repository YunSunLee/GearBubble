/*
 * tutorial.c
 *
 *  Created on: Apr 14, 2018
 *      Author: june
 */

#include "bubble.h"

#define PAGE_CHANGED_EVENT "active_page,changed"
static const int PAGES_COUNT = 6;

static const char *even_style[] = {
	"item/even_1",
	"item/even_2",
	"item/even_3",
	"item/even_4",
	"item/even_5",
	"item/even_6",
	"item/even_7",
	"item/even_8",
	"item/even_9",
	"item/even_10",
	"item/even_11",
	"item/even_12",
	"item/even_13",
	"item/even_14",
	"item/even_15",
	"item/even_16",
	"item/even_17",
	"item/even_18",
	"item/even_19",
	"item/even_20",
};

static const char *odd_style[] = {
	"item/odd_1",
	"item/odd_2",
	"item/odd_3",
	"item/odd_4",
	"item/odd_5",
	"item/odd_6",
	"item/odd_7",
	"item/odd_8",
	"item/odd_9",
	"item/odd_10",
	"item/odd_11",
	"item/odd_12",
	"item/odd_13",
	"item/odd_14",
	"item/odd_15",
	"item/odd_16",
	"item/odd_17",
	"item/odd_18",
	"item/odd_19",
};

static void
_win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	elm_win_lower(ad->win);
}

static void
_file_abs_resource_path_get(char *res_file_path, char *abs_path, int buf_size)
{
	char *res_dir_path = app_get_resource_path();

	if (res_dir_path)
	{
		snprintf(abs_path, buf_size, "%s%s", res_dir_path, res_file_path);
		free(res_dir_path);
	}
}

static void
_page_indicator_selected_set(Evas_Object *index, int page_index)
{
	Elm_Object_Item *item_to_select = elm_index_item_find(index, (void *)page_index);
	elm_index_item_selected_set(item_to_select, EINA_TRUE);
}

static Evas_Object *
_image_create(Evas_Object *parent, char *image_name)
{
	Evas_Object *image = elm_image_add(parent);

	char abs_path_to_image[PATH_MAX] = {0,};
	_file_abs_resource_path_get(image_name, abs_path_to_image, PATH_MAX);
	elm_image_file_set(image, abs_path_to_image, NULL);

	evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(image, EVAS_HINT_FILL, EVAS_HINT_FILL);

	evas_object_show(image);
	return image;
}

static Evas_Object *
_page_layout_create(Evas_Object *parent, int image_index)
{
	Evas_Object *page_layout = elm_layout_add(parent);
	elm_layout_theme_set(page_layout, "layout", "body_thumbnail", "default");
	evas_object_size_hint_weight_set(page_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(page_layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(page_layout);

	const int MAX_NAME_LENGTH = 10;
	char image_name[MAX_NAME_LENGTH] = {0,};
	snprintf(image_name, MAX_NAME_LENGTH, "%d.png", image_index);
	Evas_Object *image = _image_create(page_layout, image_name);
	elm_object_part_content_set(page_layout, "elm.icon", image);

	return page_layout;
}

static Evas_Object *
_padding_item_create(Evas_Object *parent)
{
	Evas_Object *padding = evas_object_rectangle_add(evas_object_evas_get(parent));

	evas_object_size_hint_weight_set(padding, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(padding, EVAS_HINT_FILL, EVAS_HINT_FILL);

	return padding;
}

static void
_layout_resize_cb(void *data , Evas *e, Evas_Object *page_layout, void *event_info)
{
	int page_width;
	int page_height;
	int container_width;
	int container_height;
	appdata_s *ad = data;

	evas_object_geometry_get(page_layout, NULL, NULL, &page_width, &page_height);
	evas_object_geometry_get(ad->scroller, NULL, NULL, &container_width, &container_height);

	int padding_size = (container_width - page_width)/2 ;
	evas_object_size_hint_min_set(ad->padding_start, padding_size, container_height);
	evas_object_size_hint_min_set(ad->padding_end, padding_size, container_height);

	elm_scroller_page_size_set(ad->scroller, page_width, page_height);
}

static void
_box_create(appdata_s *ad)
{
	ad->box = elm_box_add(ad->scroller);

	const int IMAGES_COUNT = 5;
	Evas_Object *page_layout;
	for (int i = 0; i < PAGES_COUNT; ++i)
	{
		page_layout = _page_layout_create(ad->box, i % IMAGES_COUNT);
		elm_box_pack_end(ad->box, page_layout);
	}

	evas_object_event_callback_add(page_layout, EVAS_CALLBACK_RESIZE, _layout_resize_cb, ad);

	ad->padding_start = _padding_item_create(ad->box);
	elm_box_pack_start(ad->box, ad->padding_start);

	ad->padding_end = _padding_item_create(ad->box);
	elm_box_pack_end(ad->box, ad->padding_end);

	elm_box_horizontal_set(ad->box, EINA_TRUE);
	evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(ad->box);
}

static void
_scroll_cb(void *data, Evas_Object *scroller, void *event)
{
	static int prev_h_page = 0;
	int cur_h_page = 0;

	elm_scroller_current_page_get(scroller, &cur_h_page, NULL);
	if (cur_h_page != prev_h_page)
	{
		appdata_s *ad= data;
		evas_object_smart_callback_call(ad->index, PAGE_CHANGED_EVENT, NULL);

		prev_h_page = cur_h_page;
	}
}

static void
_scroller_create(appdata_s *ad)
{
	ad->scroller = elm_scroller_add(ad->layout);

	elm_scroller_policy_set(ad->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

	_box_create(ad);
	elm_object_content_set(ad->scroller, ad->box);

	evas_object_smart_callback_add(ad->scroller, "scroll", _scroll_cb, ad);

	evas_object_show(ad->scroller);
}

static void
_active_page_changed_cb(void *data, Evas_Object *index, void *event)
{
	appdata_s *ad = data;
	int cur_h_page = 0;

	elm_scroller_current_page_get(ad->scroller, &cur_h_page, NULL);

	_page_indicator_selected_set(ad->index, cur_h_page);
}

static Evas_Object *
_index_create(appdata_s *ad)
{
	Evas_Object *index = elm_index_add(ad->layout);

	elm_object_style_set(index, "circle");
	elm_index_horizontal_set(index, EINA_TRUE);
	elm_index_autohide_disabled_set(index, EINA_TRUE);

	Eina_Bool is_even_count = PAGES_COUNT % 2 == 0;
	const int CENTER_INDEX = is_even_count ? 10 : 9;
	int start_index = CENTER_INDEX - PAGES_COUNT / 2;

	const char **style = is_even_count ? even_style : odd_style;

	for (int i = 0; i < PAGES_COUNT; ++i)
	{
		Elm_Object_Item *it = elm_index_item_append(index, NULL, NULL, (void *)i);
		elm_object_item_style_set(it, style[start_index + i]);
	}

	elm_index_level_go(index, 0);

	_page_indicator_selected_set(index, 0);

	evas_object_smart_callback_add(index, PAGE_CHANGED_EVENT, _active_page_changed_cb, ad);

	evas_object_show(index);

	return index;
}

static void
_layout_create(appdata_s *ad)
{
	ad->layout = elm_layout_add(ad->conform);

	elm_layout_theme_set(ad->layout, "layout", "application", "default");

	_scroller_create(ad);
	elm_object_part_content_set(ad->layout, "elm.swallow.bg", ad->scroller);

	ad->index = _index_create(ad);
	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->index);

	evas_object_show(ad->layout);
}

static void
tutorial_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;

	//elm_object_text_set(ad->title, "<font_size = 50><align=center>TUTORIAL</align></font_size>");

	//clear the screen
	//evas_object_hide(ad->title);
	//elm_box_unpack(ad->box_title, ad->title);
	evas_object_hide(ad->main_list);
	elm_box_unpack(ad->box, ad->main_list);
	elm_box_clear(ad->box_content);
	evas_object_hide(ad->bottom);
	elm_box_pack_before(ad->box, ad->box_content, ad->box_bottom);
	evas_object_hide(ad->back_list);
	elm_box_unpack(ad->box, ad->back_list);

	//index widget
	_layout_create(ad);
	elm_object_content_set(ad->conform, ad->layout);
	evas_object_show(ad->conform);
}


