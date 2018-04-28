#ifndef bubble_H
#define bubble_H

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>
#include <sensor.h>

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

//title
Evas_Object *title;
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

Evas_Object *rect[36];

sensorinfo sensor_info[3];

int sound; //0: off, 1: on
int single_mode; //0: single, 1: vs com
int stage_size;
int stage_num;

/* sensor test label*/
int sensor_status[2]; //acc sensor, heartrate sensor - 0:off, 1:on
Evas_Object *sensor_label[4];
float prev_accel[3];
float curr_velocity[3];
float curr_distance[3];


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


#endif /* bubble_H */
