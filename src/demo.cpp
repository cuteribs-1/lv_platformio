/**
 * @file demo.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "demo.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

#if LV_DEMO_WALLPAPER
LV_IMG_DECLARE(img_bubble_pattern)
#endif

LV_FONT_DECLARE(qweather);
const unsigned default_fg_color = 0x000000;
const unsigned default_bg_color = 0xffffff;

/**********************
 *   STATIC FUNCTIONS
 **********************/

lv_obj_t *my_label_create(lv_obj_t *parent, const char *text, lv_align_t align, const lv_font_t *font, short width, short height, short left, short top, const unsigned fg_color = default_fg_color, const unsigned bg_color = default_bg_color)
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

lv_obj_t *my_line_create(lv_obj_t *parent, const lv_point_t *points, short left, short top, short width, const unsigned fg_color = default_fg_color)
{
	short dots = sizeof(points) / sizeof(points[0]);
	lv_obj_t *line = lv_line_create(parent, NULL);
	lv_line_set_points(line, points, dots);

	lv_obj_set_pos(line, left, top);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_line_width(&style, LV_STATE_DEFAULT, width);
	lv_style_set_line_color(&style, LV_STATE_DEFAULT, lv_color_hex(fg_color));

	lv_obj_add_style(line, LV_OBJ_PART_MAIN, &style);
	return line;
}

/**********************
 * DEMO CREATE
 **********************/
void demo_create(void)
{
	lv_obj_t *scr1 = lv_scr_act();

	/* draw lines */
	static lv_point_t line1_points[] = {{0, 0}, {320, 0}};
	static lv_point_t line2_points[] = {{0, 0}, {0, 30}};

	my_line_create(scr1, line1_points, 0, 30, 1);
	my_line_create(scr1, line1_points, 0, 180, 1);
	my_line_create(scr1, line1_points, 0, 270, 1);
	my_line_create(scr1, line1_points, 0, 420, 1);
	my_line_create(scr1, line1_points, 0, 450, 1);

	my_line_create(scr1, line2_points, 90, 0, 1);
	my_line_create(scr1, line2_points, 230, 0, 1);
	my_line_create(scr1, line2_points, 100, 420, 1);
	my_line_create(scr1, line2_points, 160, 420, 1);
	my_line_create(scr1, line2_points, 80, 450, 1);

	/* draw labels */
	my_label_create(
			scr1,
			"风向 西北风2级",
			LV_ALIGN_CENTER,
			lv_font_,
			90,
			0,
			140,
			30);

	my_label_create(
			scr1,
			"\xEF\x84\x81",
			LV_ALIGN_CENTER,
			&qweather,
			100,
			60,
			50,
			50,
			0x096896);
}