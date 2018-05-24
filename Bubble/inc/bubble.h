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

/* List */
Evas_Object *main_list;
Evas_Object *single_mode_list;
Evas_Object *stage_size_list;
Evas_Object *stage_list;
Evas_Object *stage;
Evas_Object *sensor_list;
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

/* Initial User_State: grid_x, grid_y, bubble_count, shield_count */
int user_state[4];
/* 0: Up, 1: Down, 2: Left, 3: Right, 4: Bubble_Popped? 5:Challenger */
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

/* ranking */
	int ranking[75]; // 1st to 5th for each stage

/* sound effect player */
player_h player;

};
typedef struct appdata appdata_s;

static void main_menu_cb(void *data, Evas_Object *obj, void *event_info);

static void single_play_cb(void *data, Evas_Object *obj, void *event_info);

static void network_play_cb(void *data, Evas_Object *obj, void *event_info);

static void map_editor_cb(void *data, Evas_Object *obj, void *event_info);

static void ranking_cb(void *data, Evas_Object *obj, void *event_info);

static void tutorial_cb(void *data, Evas_Object *obj, void *event_info);

static void show_is_supported(appdata_s *ad);

static void _new_sensor_value(sensor_h sensor, sensor_event_s *sensor_data, void *user_data);

static void start_acceleration_sensor(appdata_s *ad);

static void start_gyroscope_sensor(appdata_s *ad);

static void start_heartrate_sensor(appdata_s *ad);

static void map_creater_cb(void *data, Evas_Object *obj, void *event_info);

static void sensor_test_cb(void *data, Evas_Object *obj, void *event_info);

static void sound_changed_cb(void *data, Evas_Object *obj, void *event_info);

static void sound_cb(void *data, Evas_Object *obj, void *event_info);

/* stage */
static void stage_size_3_cb(void *data, Evas_Object *obj, void *event_info);
static void stage_size_4_cb(void *data, Evas_Object *obj, void *event_info);
static void stage_size_5_cb(void *data, Evas_Object *obj, void *event_info);
static void stage_size_6_cb(void *data, Evas_Object *obj, void *event_info);

static void stage1_cb(void *data, Evas_Object *obj, void *event_info);
static void stage2_cb(void *data, Evas_Object *obj, void *event_info);
static void stage3_cb(void *data, Evas_Object *obj, void *event_info);
static void stage4_cb(void *data, Evas_Object *obj, void *event_info);
static void stage5_cb(void *data, Evas_Object *obj, void *event_info);

static void
app_get_resource(const char *res_file_in, char *res_path_out, int res_path_max);

static void move_test_cb(void *data, Evas_Object *obj, void *event_info);
static void heart_rate_test_cb(void *data, Evas_Object *obj, void *event_info);
static void jump_test_cb(void *data, Evas_Object *obj, void *event_info);
static void shake_test_cb(void *data, Evas_Object *obj, void *event_info);
static void vibe_test_cb(void *data, Evas_Object *obj, void *event_info);
static void check_obstacle(appdata_s *ad);
static int start_jmp_sensor(appdata_s *ad);

static void draw_map(appdata_s *ad);

/* ranking */
static void app_get_data(const char *res_file_in, char *res_path_out, int res_path_max);
static void read_rank_file(void *data);
static void write_rank_file(void *data);
static void ranking_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_common(void *data, int stage);
static void ranking_1_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_2_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_3_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_4_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_5_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_6_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_7_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_8_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_9_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_10_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_11_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_12_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_13_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_14_cb(void *data, Evas_Object *obj, void *event_info);
static void ranking_15_cb(void *data, Evas_Object *obj, void *event_info);
static void *itoa(int number, char *str, int radix);
static void swap(int *x, int *y);

/* player */
static player_state_e get_player_state(player_h player);
static void on_player_completed(player_h* player);
static player_h create_player();
static void stop_player(void *data, Evas_Object *obj, void *event_info);
static inline const char* get_resource_path(const char *file_path);
static void prepare_player(appdata_s* ad, int index);
static void start_player(void *data, Evas_Object *obj, void *event_info);

#endif /* bubble_H */
