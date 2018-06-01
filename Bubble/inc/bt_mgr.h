#ifndef __BT_MGR_H__
#define __BT_MGR_H__

#define BT_MGR_UUID "00001101-0000-1000-8000-00805F9B34FB"

#include <bluetooth.h>
#include <app_control.h>
#include <glib.h>
#include <stdlib.h>

typedef enum {
	BT_MGR_SEARCH = 0x00,
	BT_MGR_WAIT,
} bt_mgr_type;

void bt_mgr_initialize(void *data, bt_mgr_type type);
void bt_mgr_release(void);

#endif /* __BT_MGR_H__ */
