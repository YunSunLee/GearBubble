#include "bubble.h"
#include "bt_chat.h"
#include "log.h"
#include "bt_genlist.h"
#include "bt_mgr.h"

static void _search_cb(void *data, Evas_Object *obj, void *event_info)
{


	_D("%s", __func__);

	appdata_s *ad = (appdata_s *) data;
	ret_if(!ad);

	bt_mgr_initialize(ad, BT_MGR_SEARCH);
	ad->is_network=2;
}

static void _wait_cb(void *data, Evas_Object *obj, void *event_info)
{
	_D("%s", __func__);

	appdata_s *ad = (appdata_s *) data;
	ret_if(!ad);

	bt_mgr_initialize(ad, BT_MGR_WAIT);
	ad->is_network=1;
}

static void _exit_cb(void *data, Evas_Object *obj, void *event_info)
{
	_D("%s", __func__);

	appdata_s *ad = (appdata_s *) data;
	ret_if(!ad);

	create_base_gui(ad);
}

static void list_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *it = event_info;
	elm_list_item_selected_set(it, EINA_FALSE);
}

static Eina_Bool _naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	ui_app_exit();
	return EINA_FALSE;
}

void create_list_view(appdata_s *ad)
{
	Evas_Object *list;
	Evas_Object *btn;
	Evas_Object *nf = ad->navi;
	Elm_Object_Item *nf_it;

	/* List */
	list = elm_list_add(nf);
	elm_list_mode_set(list, ELM_LIST_COMPRESS);
	evas_object_smart_callback_add(list, "selected", list_selected_cb, NULL);

	/* Main Menu Items Here */
	elm_list_item_append(list, "Search friends", NULL, NULL, _search_cb, ad);
	elm_list_item_append(list, "Wait a friend", NULL, NULL, _wait_cb, ad);
	elm_list_item_append(list, "Exit", NULL, NULL, _exit_cb, ad);

	elm_list_go(list);

	/* This button is set for devices which doesn't have H/W back key. */
	btn = elm_button_add(nf);
	elm_object_style_set(btn, "naviframe/end_btn/default");
	nf_it = elm_naviframe_item_push(nf, "2 Players", btn, NULL, list, NULL);
	elm_naviframe_item_pop_cb_set(nf_it, _naviframe_pop_cb, ad);
}
