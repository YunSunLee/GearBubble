#include <bluetooth.h>
#include "bubble.h"
#include "bt_chat.h"
#include "bt_mgr.h"
#include "bt_genlist.h"
#include "log.h"



HAPI void app_resource_get(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
		free(res_path);
	}
}

static void _destroy_base_gui(appdata_s *ad)
{
	ret_if(!ad);

	evas_object_del(ad->navi);
	evas_object_del(ad->layout);
	evas_object_del(ad->conform);
	evas_object_del(ad->win);
}

static void _create_base_gui(appdata_s *ad)
{
	Evas_Object *genlist = NULL;
	int x = 0, y = 0, w = 0, h = 0;

	ad->socket_fd = -1;
	ad->server_socket_fd = -1;
	ad->role = BT_SOCKET_UNKNOWN;

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *) (&rots), 4);
	}

	elm_win_screen_size_get(ad->win, &x, &y, &w, &h);
	_D("window size : %dx%d(%d, %d)", w, h, x, y);
	ad->win_w = w;
	ad->win_h = h;

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	goto_if(!ad->conform, ERROR);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Eext Circle Surface Creation */
	ad->circle_surface = eext_circle_surface_conformant_add(ad->conform);

	/* Base Layout */
	ad->layout = elm_layout_add(ad->conform);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	evas_object_show(ad->layout);

	elm_object_content_set(ad->conform, ad->layout);

	/* Naviframe */
	ad->navi = elm_naviframe_add(ad->layout);
	goto_if(!ad->navi, ERROR);
	elm_object_part_content_set(ad->layout, "elm.swallow.content", ad->navi);
	eext_object_event_callback_add(ad->navi, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	eext_object_event_callback_add(ad->navi, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, NULL);
	evas_object_show(ad->navi);

	create_list_view(ad);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);

	return;

ERROR:
	_E("Failed to create base GUI");

	if (genlist) {
		evas_object_del(genlist);
		genlist = NULL;
	}

	return;
}

static bool _app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
	   Initialize UI resources and application's data
	   If this function returns true, the main loop of application starts
	   If this function returns false, the application is terminated */
	appdata_s *ad = (appdata_s *) data;
	retv_if(!ad, false);

	_create_base_gui(ad);

	return true;
}

static void _app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void _app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void _app_resume(void *data)
{
	_D("RESUMED AND BT_MGR_RELEASE");
	bt_mgr_release();
}

static void _app_terminate(void *data)
{
	/* Release all resources. */
	appdata_s *ad = (appdata_s *) data;
	int ret;

	ret_if(!ad);

	_D("APP TERMINATED");
	if (ad->socket_fd != -1) {
		ret = bt_socket_disconnect_rfcomm(ad->socket_fd);
		if (ret != BT_ERROR_NONE) {
			_E("[bt_socket_disconnect_rfcomm] Failed");
		} else {
			ad->socket_fd = -1;
		}
	}
	if (ad->server_socket_fd != -1) {
		ret = bt_socket_destroy_rfcomm(ad->server_socket_fd);
		if (ret != BT_ERROR_NONE) {
			_E("[bt_socket_destroy_rfcomm] Failed");
		} else {
			ad->server_socket_fd = -1;
		}
	}
	bt_mgr_release();

	_destroy_base_gui(ad);
}
