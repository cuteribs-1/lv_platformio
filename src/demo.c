/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void write_create(lv_obj_t *parent);
static void text_area_event_handler(lv_obj_t *text_area, lv_event_t event);
static void keyboard_event_cb(lv_obj_t *keyboard, lv_event_t event);
#if LV_USE_ANIMATION
static void kb_hide_anim_end(lv_anim_t *a);
#endif
static void list_create(lv_obj_t *parent);
static void chart_create(lv_obj_t *parent);
static void slider_event_handler(lv_obj_t *slider, lv_event_t event);
static void list_btn_event_handler(lv_obj_t *slider, lv_event_t event);
#if LV_DEMO_SLIDE_SHOW
static void tab_switcher(lv_task_t *task);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *chart;
static lv_obj_t *ta;
static lv_obj_t *kb;

static lv_style_t style_kb;

#if LV_DEMO_WALLPAPER
LV_IMG_DECLARE(img_bubble_pattern)
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void write_create(lv_obj_t *parent)
{
    // lv_page_set_style(parent, LV_PAGE_STYLE_BG, &lv_style_transp_fit);
    // lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_fit);

    lv_page_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_ta;
    lv_style_init(&style_ta);
    lv_style_set_bg_opa(&style_ta, LV_STATE_DEFAULT, LV_OPA_30);
    lv_style_set_radius(&style_ta, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&style_ta, LV_STATE_DEFAULT, lv_color_hex3(0x222));

    ta = lv_textarea_create(parent, NULL);
    lv_obj_set_size(ta, lv_page_get_scrl_width(parent), lv_obj_get_height(parent) / 2);
    lv_obj_add_style(ta, LV_TEXTAREA_PART_BG, &style_ta);
    lv_textarea_set_text(ta, "");
    lv_obj_set_event_cb(ta, text_area_event_handler);
    lv_textarea_set_text_sel(ta, true);

    lv_style_init(&style_kb);
    lv_style_set_bg_opa(&style_kb, LV_STATE_DEFAULT, LV_OPA_70);
    lv_style_set_bg_color(&style_kb, LV_STATE_DEFAULT, lv_color_hex3(0x333));
    lv_style_set_bg_grad_color(&style_kb, LV_STATE_DEFAULT, lv_color_hex3(0x333));
    lv_style_set_pad_left(&style_kb, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_kb, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_kb, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_kb, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&style_kb, LV_STATE_DEFAULT, 0);

    lv_style_set_bg_opa(&style_kb, LV_BTN_STATE_RELEASED, LV_OPA_70);
    lv_style_set_radius(&style_kb, LV_BTN_STATE_RELEASED, 0);
    lv_style_set_border_width(&style_kb, LV_BTN_STATE_RELEASED, 1);
    lv_style_set_border_color(&style_kb, LV_BTN_STATE_RELEASED, LV_COLOR_SILVER);
    lv_style_set_border_opa(&style_kb, LV_BTN_STATE_RELEASED, LV_OPA_50);
    lv_style_set_bg_color(&style_kb, LV_BTN_STATE_RELEASED, lv_color_hex3(0x333));
    lv_style_set_bg_grad_color(&style_kb, LV_BTN_STATE_RELEASED, lv_color_hex3(0x333));
    lv_style_set_text_color(&style_kb, LV_BTN_STATE_RELEASED, LV_COLOR_WHITE);

    lv_style_set_radius(&style_kb, LV_BTN_STATE_PRESSED, 0);
    lv_style_set_bg_opa(&style_kb, LV_BTN_STATE_PRESSED, LV_OPA_50);
    lv_style_set_bg_color(&style_kb, LV_BTN_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_bg_grad_color(&style_kb, LV_BTN_STATE_PRESSED, LV_COLOR_WHITE);
    lv_style_set_border_width(&style_kb, LV_BTN_STATE_PRESSED, 1);
    lv_style_set_border_color(&style_kb, LV_BTN_STATE_PRESSED, LV_COLOR_SILVER);
}

static void text_area_event_handler(lv_obj_t *text_area, lv_event_t event)
{
    (void)text_area; /*Unused*/

    /*Text area is on the scrollable part of the page but we need the page itself*/
    lv_obj_t *parent = lv_obj_get_parent(lv_obj_get_parent(ta));

    if (event == LV_EVENT_CLICKED)
    {
        if (kb == NULL)
        {
            kb = lv_keyboard_create(parent, NULL);
            lv_obj_set_size(kb, lv_obj_get_width_fit(parent), lv_obj_get_height_fit(parent) / 2);
            lv_obj_align(kb, ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_add_style(kb, LV_KEYBOARD_PART_BG, &style_kb);
            lv_obj_add_style(kb, LV_KEYBOARD_PART_BTN, &style_kb);
            lv_obj_set_event_cb(kb, keyboard_event_cb);

#if LV_USE_ANIMATION
            lv_anim_t a;
            lv_anim_init(&a);
            lv_anim_set_var(&a, kb);
            lv_anim_set_time(&a, 300);
            lv_anim_set_values(&a, LV_VER_RES, lv_obj_get_y(kb));
            lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
            lv_anim_set_ready_cb(&a, NULL);
            lv_anim_start(&a);
#endif
        }
    }
}

/**
 * Called when the close or ok button is pressed on the keyboard
 * @param keyboard pointer to the keyboard
 * @return
 */
static void keyboard_event_cb(lv_obj_t *keyboard, lv_event_t event)
{
    (void)keyboard; /*Unused*/

    lv_keyboard_def_event_cb(kb, event);

    if (event == LV_EVENT_APPLY || event == LV_EVENT_CANCEL)
    {
#if LV_USE_ANIMATION
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, kb);
        lv_anim_set_time(&a, 300);
        lv_anim_set_values(&a, LV_VER_RES, lv_obj_get_y(kb));
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
        lv_anim_set_ready_cb(&a, NULL);
        lv_anim_start(&a);
#else
        lv_obj_del(kb);
        kb = NULL;
#endif
    }
}

static void list_create(lv_obj_t *parent)
{
    lv_coord_t hres = lv_disp_get_hor_res(NULL);

    static lv_style_t style_page;
    lv_style_init(&style_page);

    lv_style_set_bg_opa(&style_page, LV_STATE_DEFAULT, LV_OPA_100);
    lv_style_set_pad_top(&style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&style_page, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_page, LV_STATE_DEFAULT, 5);

    lv_obj_add_style(parent, LV_PAGE_PART_BG, &style_page);
    lv_obj_add_style(parent, LV_PAGE_PART_SCROLLABLE, &style_page);

    lv_page_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    /*Create styles for the buttons*/
    static lv_style_t style_btn;
    lv_style_init(&style_btn);

    lv_style_set_bg_color(&style_btn, LV_BTN_STATE_RELEASED, lv_color_hex3(0x333));
    lv_style_set_bg_grad_color(&style_btn, LV_BTN_STATE_RELEASED, LV_COLOR_BLACK);
    lv_style_set_border_color(&style_btn, LV_BTN_STATE_RELEASED, LV_COLOR_SILVER);
    lv_style_set_border_width(&style_btn, LV_BTN_STATE_RELEASED, 1);
    lv_style_set_border_opa(&style_btn, LV_BTN_STATE_RELEASED, LV_OPA_50);
    lv_style_set_radius(&style_btn, LV_BTN_STATE_RELEASED, 0);

    lv_style_set_bg_color(&style_btn, LV_BTN_STATE_PRESSED, lv_color_make(0x55, 0x96, 0xd8));
    lv_style_set_bg_grad_color(&style_btn, LV_BTN_STATE_PRESSED, lv_color_make(0x37, 0x62, 0x90));
    lv_style_set_text_color(&style_btn, LV_BTN_STATE_PRESSED, lv_color_make(0xbb, 0xd5, 0xf1));

    lv_obj_t *list = lv_list_create(parent, NULL);
    lv_obj_set_height(list, 2 * lv_obj_get_height(parent) / 3);

    static lv_style_t style_list;
    lv_style_init(&style_list);
    lv_style_set_bg_opa(&style_list, LV_STATE_DEFAULT, LV_OPA_100);
    lv_style_set_pad_left(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_list, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_list, LV_STATE_DEFAULT, 0);

    lv_obj_add_style(list, LV_LIST_PART_BG, &style_list);
    lv_obj_add_style(list, LV_LIST_PART_SCROLLABLE, &style_list);

    lv_obj_align(list, NULL, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 4);

    lv_obj_t *list_btn;
    list_btn = lv_list_add_btn(list, LV_SYMBOL_FILE, "New");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Open");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_TRASH, "Delete");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_EDIT, "Edit");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_SAVE, "Save");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_WIFI, "WiFi");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    list_btn = lv_list_add_btn(list, LV_SYMBOL_GPS, "GPS");
    lv_obj_set_event_cb(list_btn, list_btn_event_handler);

    lv_obj_t *mbox = lv_msgbox_create(parent, NULL);
    lv_msgbox_set_text(mbox, "Click a button to copy its text to the Text area ");
    lv_obj_set_width(mbox, hres - LV_DPI);
    static const char *mbox_btns[] = {"Got it", ""};
    lv_msgbox_add_btns(mbox, mbox_btns); /*The default action is close*/
    lv_obj_align(mbox, parent, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 2);
}

#if LV_USE_ANIMATION
static void kb_hide_anim_end(lv_anim_t *a)
{
    lv_obj_del(a->var);
    kb = NULL;
}
#endif

static void chart_create(lv_obj_t *parent)
{

    lv_coord_t vres = lv_disp_get_ver_res(NULL);

    static lv_style_t style_page;
    lv_style_init(&style_page);

    lv_style_set_bg_opa(&style_page, LV_STATE_DEFAULT, LV_OPA_100);
    lv_style_set_pad_top(&style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_page, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&style_page, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_right(&style_page, LV_STATE_DEFAULT, 5);

    lv_obj_add_style(parent, LV_PAGE_PART_BG, &style_page);
    lv_obj_add_style(parent, LV_PAGE_PART_SCROLLABLE, &style_page);

    lv_page_set_scrl_height(parent, lv_obj_get_height(parent));
    lv_page_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    static lv_style_t style_chart;
    lv_style_init(&style_chart);
    lv_style_set_bg_opa(&style_chart, LV_STATE_DEFAULT, LV_OPA_60);
    lv_style_set_radius(&style_chart, LV_STATE_DEFAULT, 0);
    lv_style_set_line_color(&style_chart, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_line_opa(&style_chart, LV_STATE_DEFAULT, LV_OPA_70);

    chart = lv_chart_create(parent, NULL);
    lv_obj_set_size(chart, 2 * lv_obj_get_width(parent) / 3, lv_obj_get_height(parent) / 2);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 4);
    lv_chart_set_type(chart, LV_CHART_TYPE_COLUMN);
    lv_obj_add_style(chart, LV_CHART_PART_BG, &style_chart);
    lv_obj_add_style(chart, LV_CHART_PART_SERIES, &style_chart);

    lv_chart_series_t *ser1;
    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_set_next(chart, ser1, 40);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 47);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 59);
    lv_chart_set_next(chart, ser1, 31);
    lv_chart_set_next(chart, ser1, 55);
    lv_chart_set_next(chart, ser1, 70);
    lv_chart_set_next(chart, ser1, 82);
    lv_chart_set_next(chart, ser1, 91);

    /*Create a bar, an indicator and a knob style*/
    static lv_style_t style_bar;
    static lv_style_t style_indic;
    static lv_style_t style_knob;

    lv_style_init(&style_bar);
    lv_style_set_bg_color(&style_bar, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_grad_color(&style_bar, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_radius(&style_bar, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
    lv_style_set_border_color(&style_bar, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&style_bar, LV_STATE_DEFAULT, LV_OPA_60);
    lv_style_set_pad_left(&style_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_bar, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&style_bar, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_pad_bottom(&style_bar, LV_STATE_DEFAULT, LV_DPI / 10);

    lv_style_init(&style_indic);
    lv_style_set_bg_color(&style_indic, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_bg_grad_color(&style_indic, LV_STATE_DEFAULT, LV_COLOR_MAROON);
    lv_style_set_radius(&style_indic, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
    lv_style_set_shadow_width(&style_indic, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_shadow_color(&style_indic, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_pad_left(&style_indic, LV_STATE_DEFAULT, LV_DPI / 30);
    lv_style_set_pad_right(&style_indic, LV_STATE_DEFAULT, LV_DPI / 30);
    lv_style_set_pad_top(&style_indic, LV_STATE_DEFAULT, LV_DPI / 30);
    lv_style_set_pad_bottom(&style_indic, LV_STATE_DEFAULT, LV_DPI / 30);

    lv_style_init(&style_knob);
    lv_style_set_radius(&style_knob, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
    lv_style_set_bg_opa(&style_knob, LV_STATE_DEFAULT, LV_OPA_70);

    /*Create a second slider*/
    lv_obj_t *slider = lv_slider_create(parent, NULL);
    lv_obj_add_style(slider, LV_SLIDER_PART_BG, &style_bar);
    lv_obj_add_style(slider, LV_SLIDER_PART_INDIC, &style_indic);
    lv_obj_add_style(slider, LV_SLIDER_PART_KNOB, &style_knob);
    lv_obj_set_size(slider, lv_obj_get_width(chart), LV_DPI / 3);
    lv_obj_align(slider, chart, LV_ALIGN_OUT_BOTTOM_MID, 0, (vres - chart->coords.y2 - lv_obj_get_height(slider)) / 2); /*Align to below the chart*/
    lv_obj_set_event_cb(slider, slider_event_handler);
    lv_slider_set_range(slider, 10, 1000);
    lv_slider_set_value(slider, 700, false);
    slider_event_handler(slider, LV_EVENT_VALUE_CHANGED); /*Simulate a user value set the refresh the chart*/
}

/**
 * Called when a new value on the slider on the Chart tab is set
 * @param slider pointer to the slider
 * @return LV_RES_OK because the slider is not deleted in the function
 */
static void slider_event_handler(lv_obj_t *slider, lv_event_t event)
{

    if (event == LV_EVENT_VALUE_CHANGED)
    {
        int16_t v = lv_slider_get_value(slider);
        v = 1000 * 100 / v; /*Convert to range modify values linearly*/
        lv_chart_set_range(chart, 0, v);
    }
}

/**
 * Called when a a list button is clicked on the List tab
 * @param btn pointer to a list button
 * @return LV_RES_OK because the button is not deleted in the function
 */
static void list_btn_event_handler(lv_obj_t *btn, lv_event_t event)
{

    if (event == LV_EVENT_SHORT_CLICKED)
    {
        lv_textarea_add_char(ta, '\n');
        lv_textarea_add_text(ta, lv_list_get_btn_text(btn));
    }
}

#if LV_DEMO_SLIDE_SHOW
/**
 * Called periodically (lv_task) to switch to the next tab
 */
static void tab_switcher(lv_task_t *task)
{
    static uint8_t tab = 0;
    lv_obj_t *tv = task->user_data;
    tab++;
    if (tab >= 3)
        tab = 0;
    lv_tabview_set_tab_act(tv, tab, true);
}
#endif

LV_FONT_DECLARE(qweather);
const unsigned default_fg_color = 0x000000;
const unsigned default_bg_color = 0xffffff;

lv_obj_t *my_label_create(lv_obj_t *parent, char *text, lv_align_t align, const lv_font_t *font, short width, short height, short left, short top, const unsigned fg_color, const unsigned bg_color)
{
    lv_obj_t *label = lv_label_create(parent, NULL);
    lv_label_set_text(label, text);
    lv_label_set_align(label, align);

    lv_obj_set_size(label, width, height);
    lv_obj_set_pos(label, left, top);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, LV_STATE_DEFAULT, font);
    lv_style_set_text_color(&style, LV_STATE_DEFAULT, lv_color_hex(fg_color));
    lv_style_set_bg_color(&style, LV_STATE_DEFAULT, lv_color_hex(bg_color));

    lv_obj_add_style(label, LV_OBJ_PART_MAIN, &style);
    return label;
}

lv_obj_t *my_line_create(lv_obj_t *parent, const lv_point_t *points, short left, short top, short width, short dots, const unsigned fg_color, const unsigned bg_color)
{
    lv_obj_t *line = lv_line_create(parent, NULL);
    lv_line_set_points(line, points, dots);

    lv_obj_set_pos(line, left, top);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_line_width(&style, LV_STATE_DEFAULT, width);
    lv_style_set_line_color(&style, LV_STATE_DEFAULT, lv_color_hex(fg_color));
    lv_style_set_bg_color(&style, LV_STATE_DEFAULT, lv_color_hex(bg_color));

    lv_obj_add_style(line, LV_OBJ_PART_MAIN, &style);
    return line;
}

/**
 * Create a demo application
 */
void demo_create(void)
{
    lv_coord_t hres = lv_disp_get_hor_res(NULL);
    lv_coord_t vres = lv_disp_get_ver_res(NULL);

    lv_obj_t *tv = lv_tabview_create(lv_scr_act(), NULL);
    lv_obj_set_size(tv, hres, vres);

    lv_obj_t *tab1 = lv_tabview_add_tab(tv, "Write");
    // lv_obj_t *tab2 = lv_tabview_add_tab(tv, "List");
    // lv_obj_t *tab3 = lv_tabview_add_tab(tv, "Chart");

    lv_obj_t *lbl1 = my_label_create(
        tab1,
        "\xEF\x84\x81",
        LV_ALIGN_CENTER,
        &qweather,
        100,
        60,
        50,
        50,
        0x096896,
        default_bg_color);

    static lv_point_t line1_points[] = {{0, 50}, {160, 100}, {320, 50}};

    lv_obj_t *line1 = my_line_create(
        tab1,
        line1_points,
        10,
        10,
        1,
        3,
        default_fg_color,
        default_bg_color);

    // write_create(tab1);
    // list_create(tab2);
    // chart_create(tab3);

#if LV_DEMO_SLIDE_SHOW
    lv_task_create(tab_switcher, 3000, LV_TASK_PRIO_MID, tv);
#endif
}