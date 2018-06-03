#ifndef bubble_H
#define bubble_H

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>
#include <sensor.h>
#include <device/haptic.h>
#include <player.h>

#include <tizen.h>
#include <bluetooth_type.h>
#include <stdbool.h>

#include "log.h"

#if !defined(PACKAGE)
#define PACKAGE "BLUETOOTHCHAT"
#endif

#define EDJ_FILE "edje/bt_chat.edj"
#define CREATE_SERVER "edje/create_server.edj"
#define GRP_MAIN "main"

#define HAPI __attribute__((visibility("hidden")))


#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "bubble"

#if !defined(PACKAGE)
#define PACKAGE "org.example.bubble"
#endif

typedef struct
{
sensor_h sensor; /**< Sensor handle */
sensor_listener_h sensor_listener;
} sensorinfo;

struct appdata {
/* Window */
Evas_Object *win;
/* Conformant */
Evas_Object *conform;
/* Label */
Evas_Object *label;
/* Box */
Evas_Object *box; //main box
Evas_Object *box_title;
Evas_Object *box_content;
Evas_Object *box_bottom;

/* map_editor list */
Evas_Object *box2; //main box
Evas_Object *box_title2;
Evas_Object *box_content2;
Evas_Object *main_list2;

/* List */
Evas_Object *main_list;
Evas_Object *single_mode_list;
Evas_Object *stage_size_list;
Evas_Object *stage_list;
Evas_Object *stage;
Evas_Object *sensor_list;
Evas_Object *map_editor_size_list;
Evas_Object *ranking_list;
Evas_Object *ranking_stage_list;


//title
Evas_Object *title;
Evas_Object *title2;
Evas_Object *bottom;

//acc sensor test label
Evas_Object *acc_label[2];

//tutorial
Evas_Object *layout;
Evas_Object *scroller;
Evas_Object *padding_start;
Evas_Object *padding_end;
Evas_Object *index;

Evas_Object *back_list;

Evas_Object *sound_check;

Evas_Object *grid; //added
Evas_Object *grid2;
Evas_Object *rect[25]; //added
Evas *canvas; //added
int grid_width;

/* Initial User_State 0: grid_x, 1: grid_y, 2: bubble_count, 3: shield_count */
int user_state[4];

/* Grid_State 0: Up, 1: Down, 2: Left, 3: Right, 4: Bubble_Popped? 5:Challenger */
/* grid_state[][][4]==0: bubble not popped, 1: bubble popped */
/* grid_state[][][5]==0: bubble, 1: hurdle, 2: bug, 3: heart */
int grid_state[5][5][6];

sensorinfo sensor_info[3];

int sound; //0: off, 1: on
int single_mode; //0: single, 1: vs com
int stage_size;
int stage_num;


/* sensor test label*/
int sensor_status[3]; //acc sensor, gyroscope sensor, heartrate sensor // -1: unused, 0:off, 1:test_mode, 2:play_mode
Evas_Object *sensor_label[4];

//timer
Ecore_Timer *timer;
int time;

//vibration
haptic_device_h handle;
haptic_effect_h effect_handle;


//map editor
Evas_Object *rotary_selector;
Evas_Object *map_editor_rect[26];

/* ranking */
int ranking[75]; // 1st to 5th for each stage

/* sound effect player */
player_h player;


//for bluetooth
Evas_Object* navi;
Eext_Circle_Surface *circle_surface;
bool bt;
int socket_fd;
int server_socket_fd;
bt_socket_role_e role;

int win_w;
int win_h;
	
int hr_test; //remove later

int friend_pop_num;
int is_network;

};
typedef struct appdata appdata_s;

typedef enum {
	CS_MAIN = 0x00,
	CS_PREPARE,
	CS_CHAT,
} current_state_e;

static void app_resource_get(const char *edj_file_in, char *edj_path_out, int edj_path_max);
void cr_layout(appdata_s *ad);

void win_delete_request_cb(void *data, Evas_Object *obj, void *event_info);

void create_base_gui(appdata_s *ad);

void main_menu_cb(void *data, Evas_Object *obj, void *event_info);

void single_play_cb(void *data, Evas_Object *obj, void *event_info);

void network_play_cb(void *data, Evas_Object *obj, void *event_info);

void map_editor_cb(void *data, Evas_Object *obj, void *event_info);

void ranking_cb(void *data, Evas_Object *obj, void *event_info);

void tutorial_cb(void *data, Evas_Object *obj, void *event_info);

void map_creater_cb(void *data, Evas_Object *obj, void *event_info);

void sensor_test_cb(void *data, Evas_Object *obj, void *event_info);

static void sound_changed_cb(void *data, Evas_Object *obj, void *event_info);

void sound_cb(void *data, Evas_Object *obj, void *event_info);

/* stage */

void app_get_resource(const char *res_file_in, char *res_path_out, int res_path_max);

/* sensor */

void start_acceleration_sensor(appdata_s *ad);
void start_gyroscope_sensor(appdata_s *ad);
void start_heartrate_sensor(appdata_s *ad);


/* map creater */
void draw_map(appdata_s *ad);
Eina_Bool timer_cb(void *data EINA_UNUSED);

/* map_editor */


/* ranking */
void app_get_data(const char *res_file_in, char *res_path_out, int res_path_max);
void read_rank_file(void *data);
void write_rank_file(void *data);
void ranking_cb(void *data, Evas_Object *obj, void *event_info);
void swap(int *x, int *y);

/* player */
player_state_e get_player_state(player_h player);
player_h create_player();
static inline const char* get_resource_path(const char *file_path);
void prepare_player(appdata_s* ad, int index);
void start_player(void *data, Evas_Object *obj, void *event_info);


/*network*/
void _message_send(appdata_s *ad);



#endif /* bubble_H */
