#ifndef __bubble_H__
#define __bubble_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>
#include <sensor.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
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

    //title
    Evas_Object *title;
    Evas_Object *bottom;

    //acc sensor test label
    Evas_Object *acc_label[2];


    Evas_Object *back_list;

    Evas_Object *sound_check;

    Evas_Object *rect[36];

    sensorinfo sensor_info;

    int sound; //0: off, 1: on
    int single_mode; //0: single, 1: vs com
    int stage_size;
    int stage_num;
};
typedef struct appdata appdata_s;

static void
single_play_cb(void *data, Evas_Object *obj, void *event_info);

static void
network_play_cb(void *data, Evas_Object *obj, void *event_info);

static void
map_editor_cb(void *data, Evas_Object *obj, void *event_info);

static void
ranking_cb(void *data, Evas_Object *obj, void *event_info);

static void
tutorial_cb(void *data, Evas_Object *obj, void *event_info);
#endif /* __bubble_H__ */
