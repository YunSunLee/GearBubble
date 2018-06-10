#include "bubble.h"
#include "bt_noti.h"

static void _popup_hide_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) {
		return;
	}
	evas_object_del(obj);
}

static void _popup_hide_finished_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) {
		return;
	}
	evas_object_del(obj);
}

static void _timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (!obj) {
		return;
	}
	evas_object_del(obj);
}

HAPI Evas_Object *bt_noti_popup_create(Evas_Object *parent, const char *string)
{
	Evas_Object *popup;
	Evas_Object *layout;

	popup = elm_popup_add(parent);
	elm_object_style_set(popup, "circle");
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _popup_hide_cb, NULL);
	evas_object_smart_callback_add(popup, "dismissed", _popup_hide_finished_cb, NULL);

	evas_object_smart_callback_add(popup, "timeout", _timeout_cb, NULL);

	layout = elm_layout_add(popup);
	elm_layout_theme_set(layout, "layout", "popup", "content/circle");

	_D("popup content : %s", string);
	elm_object_part_text_set(layout, "elm.text", string);
	elm_object_content_set(popup, layout);

	evas_object_show(popup);

	return popup;
}
