#include "bubble.h"
#include "bt_chat.h"
#include "bt_mgr.h"
#include "bt_noti.h"
#include "bt_chat_room.h"


#define MAX_NUM_PENDING 1

static struct _s_info {
	bt_adapter_device_discovery_info_s *info;
	Evas_Object *list;
	Evas_Object *noti;
} s_info = {
	.info = NULL,
	.noti = NULL,
	.list = NULL,
};

HAPI static void app_resource_get(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
		free(res_path);
	}
}

static void _socket_conn_state_changed_cb(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *user_data)
{//연결 기다렷다가 채팅 방 레이아웃 부르기

	appdata_s *ad = (appdata_s *) user_data;
	ret_if(!ad);

	ret_if(result != BT_ERROR_NONE);

	if (connection_state == BT_SOCKET_CONNECTED) {
		if (connection != NULL) {
			_D("Connected %d %d", ad->socket_fd, connection->socket_fd);
			ad->role = connection->local_role;
			ad->socket_fd = connection->socket_fd;

			/*
			// draw 3*3 map
			//appdata_s *ad = data;

			//start timer
			ad->timer = ecore_timer_add(1.0, timer_cb, ad);
			ad->time = 0;

			//create vibration
			device_haptic_open(0, &ad->handle);

			start_acceleration_sensor(ad);
			start_gyroscope_sensor(ad);
			start_heartrate_sensor(ad);
			ad->sensor_status[0] = 2;
			ad->sensor_status[1] = 2;
			ad->sensor_status[2] = 2;


			//initialize user_state and grid state
			ad->user_state[0] = 0;
			ad->user_state[1] = ad->stage_size-1;
			ad->user_state[2] = 0;
			ad->user_state[3] = 0;

			for (int j=0;j<ad->stage_size;j++){ //i,j order change
				for (int i=0;i<ad->stage_size;i++){
					ad->grid_state[i][j][4] = 0;
				}
			}

			ad->user_state[2] = 1;
			*/


			create_base_gui(ad);
			single_play_cb(ad, NULL, NULL);
			ad->stage_size = 3;
			map_creater_cb(ad, NULL, NULL);


			//bt_chat_room_layout_create(ad);
			if (s_info.noti) {
				evas_object_del(s_info.noti);
				s_info.noti = NULL;
			}
		} else {
			_D("No connection data");
		}
	} else {
		ad->socket_fd = -1;
		_D("Disconnected");
	}
}

static void _device_bond_created_cb(int result, bt_device_info_s *device_info, void *user_data)
{
	int ret = 0;

	if (result != BT_ERROR_NONE) {
		_E("Failed result: %d", result);
		goto DEL_NOTI;
	}

	if (device_info != NULL && !strncmp(device_info->remote_address, s_info.info->remote_address, strlen(device_info->remote_address))) {
		ret = bt_socket_connect_rfcomm(s_info.info->remote_address, BT_MGR_UUID);
		if (ret != BT_ERROR_NONE) {
			_E("[bt_socket_listen_and_accept_rfcomm] Failed");
			goto DEL_NOTI;
		}
	} else {
		_D("[bond create cb] Bonded with another device");
		goto DEL_NOTI;
	}

	return;

DEL_NOTI:

	if (s_info.noti) {
		evas_object_del(s_info.noti);
		s_info.noti = NULL;
	}

	return;
}

static void _click_friend_item_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *noti = NULL;
	Elm_Object_Item *item = NULL;
	bt_adapter_device_discovery_info_s *info = NULL;
	bt_error_e ret = BT_ERROR_NONE;

	ret_if(!obj);

	info = (bt_adapter_device_discovery_info_s *) data;
	ret_if(!info);
	s_info.info = info;

	item = elm_list_selected_item_get(obj);
	ret_if(!item);

	noti = bt_noti_popup_create(obj, "Waiting for connecting with the friend");
	if (!noti) {
		_E("Failed to create popup noti");
	} else {
		s_info.noti = noti;
	}

	elm_list_item_selected_set(item, EINA_FALSE);

	ret = bt_device_set_bond_created_cb(_device_bond_created_cb, NULL);
	ret_if(ret != BT_ERROR_NONE);

	ret = bt_device_create_bond(info->remote_address);
	ret_if(ret != BT_ERROR_NONE);
}

static void _adapter_discovery_state_changed_cb(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void *user_data)
{
	appdata_s *ad = NULL;
	bt_adapter_device_discovery_info_s *new_device_info = NULL;
	int ret = 0;

	ad = (appdata_s *) user_data;
	ret_if(!ad);

	ret_if(!s_info.list);
	ret_if(result != BT_ERROR_NONE);

	switch (discovery_state) {
	case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
		_D("BT_ADAPTER_DEVICE_DISCOVERY_STARTED");
		break;
	case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
		_D("BT_ADAPTER_DEVICE_DISCOVERY_FINISHED");
		ret = bt_adapter_unset_device_discovery_state_changed_cb();
		if (ret != BT_ERROR_NONE) {
			_E("[BT_ADAPTER_DEVICE_DISCOVERY_FINISHED] Failed to unset the state discovery cb");
			return;
		}
		ad->bt = FALSE;
		break;
	case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
		_D("BT_ADAPTER_DEVICE_DISCOVERY_FOUND");
		if (discovery_info != NULL && s_info.list != NULL) {
			new_device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));
			if (new_device_info != NULL) {
				_D("Device Name is: %s", discovery_info->remote_name);

				memcpy(new_device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
				new_device_info->remote_address = strdup(discovery_info->remote_address);
				new_device_info->remote_name = strdup(discovery_info->remote_name);
				elm_list_item_append(s_info.list, new_device_info->remote_name, NULL, NULL, _click_friend_item_cb, new_device_info);
				elm_list_go(s_info.list);
			}
		}
		break;
	}
}

static void _discovery_start(appdata_s *ad)
{
	bt_error_e ret = BT_ERROR_NONE;

	ret_if(!ad);
	ret_if(ad->bt);

	ad->bt = TRUE;

	ret = bt_adapter_set_device_discovery_state_changed_cb(_adapter_discovery_state_changed_cb, ad);
	ret_if(ret != BT_ERROR_NONE);

	ret = bt_adapter_start_device_discovery();
	ret_if(ret != BT_ERROR_NONE);
}

static void _discovery_stop(appdata_s *ad)
{
	bt_error_e ret = BT_ERROR_NONE;

	ret_if(!ad);

	if (ad->bt) {
		ret = bt_adapter_stop_device_discovery();
		ret_if(ret != BT_ERROR_NONE);
	}

	ad->bt = FALSE;

	ret = bt_adapter_unset_device_discovery_state_changed_cb();
	ret_if(ret != BT_ERROR_NONE);
}

static void _on_search_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *) data;
	ret_if(!ad);

	if (s_info.list != NULL) {
		_discovery_stop(ad);
		elm_list_clear(s_info.list);
		s_info.list = NULL;
	}
}

static void _search_layout_create(appdata_s *ad)
{
	int ret = 0;

	ret_if(!ad);
	ret_if(!ad->navi);

	ad->role = BT_SOCKET_CLIENT;

	s_info.list = elm_list_add(ad->navi);
	ret_if(!s_info.list);

	evas_object_event_callback_add(s_info.list, EVAS_CALLBACK_DEL, _on_search_del_cb, ad);
	evas_object_size_hint_weight_set(s_info.list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(s_info.list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_list_go(s_info.list);
	elm_naviframe_item_push(ad->navi, "Search Friends", NULL, NULL, s_info.list, NULL);

	ret = bt_socket_set_connection_state_changed_cb(_socket_conn_state_changed_cb, ad);
	ret_if(ret != BT_ERROR_NONE);

	_discovery_start(ad);
}

static void _server_create(appdata_s *ad)
{
	int server_socket_fd = -1;
	int ret = 0;

	ret = bt_socket_create_rfcomm(BT_MGR_UUID, &server_socket_fd);
	ret_if(ret != BT_ERROR_NONE);

	ad->server_socket_fd = server_socket_fd;
	_D("[bt_socket_create_rfcomm] socket %d", server_socket_fd);

	ret = bt_socket_set_connection_state_changed_cb(_socket_conn_state_changed_cb, ad);
	ret_if(ret != BT_ERROR_NONE);

	ret = bt_socket_listen_and_accept_rfcomm(server_socket_fd, MAX_NUM_PENDING);
	if (ret != BT_ERROR_NONE) {
		_E("[bt_socket_listen_and_accept_rfcomm] Failed");
	}
}

static void _server_layout_create(appdata_s *ad)
{
	Evas_Object *layout = NULL;
	Evas_Object *progress = NULL;
	Elm_Object_Item *navi_it = NULL;
	char edj_path[PATH_MAX] = { 0, };

	ad->role = BT_SOCKET_SERVER;

	app_resource_get(CREATE_SERVER, edj_path, (int)PATH_MAX);
	layout = elm_layout_add(ad->navi);
	goto_if(!layout, ERROR);
	elm_layout_file_set(layout, edj_path, "create_server");
	evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	progress = elm_progressbar_add(layout);
	elm_object_style_set(progress, "process/popup/small");
	evas_object_size_hint_align_set(progress, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(progress, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse(progress, EINA_TRUE);
	evas_object_show(progress);

	elm_object_part_content_set(layout, "progress", progress);

	navi_it = elm_naviframe_item_push(ad->navi, "Wait a Friend", NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(navi_it, EINA_FALSE, EINA_FALSE);

	_server_create(ad);

	return;

ERROR:
	_E("Failed to create server layout");

	if (layout) {
		evas_object_del(layout);
		layout = NULL;
	}

	if (progress) {
		evas_object_del(progress);
		progress = NULL;
	}

	return;
}

HAPI void bt_mgr_initialize(void *data, bt_mgr_type type)
{
	appdata_s *ad = NULL;
	bt_adapter_state_e bt_ad_state = BT_ADAPTER_DISABLED;
	int ret = 0;

	ad = (appdata_s *) data;
	ret_if(!ad);

	ret = bt_initialize();
	ret_if(ret != BT_ERROR_NONE);

	ret = bt_adapter_get_state(&bt_ad_state);
	ret_if(ret != BT_ERROR_NONE);

	if (bt_ad_state == BT_ADAPTER_DISABLED) {
		Evas_Object *noti = bt_noti_popup_create(ad->navi, "Bluetooth is OFF");
		if (!noti) {
			_E("Failed to create popup noti");
		} else {
			s_info.noti = noti;
		}
	} else {
		switch (type) {
		case BT_MGR_SEARCH:
			_search_layout_create(ad);
			break;
		case BT_MGR_WAIT:
			_server_layout_create(ad);
			break;
		}
	}
}

HAPI void bt_mgr_release(void)
{
	bt_adapter_unset_state_changed_cb();
	bt_adapter_unset_device_discovery_state_changed_cb();
	bt_device_unset_service_searched_cb();
	bt_socket_unset_data_received_cb();
	bt_socket_unset_connection_state_changed_cb();
	bt_deinitialize();
}
