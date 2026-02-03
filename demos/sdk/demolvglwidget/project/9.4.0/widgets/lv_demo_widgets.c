#include "lvgl.h"
#include "lv_demo_widgets.h"

typedef enum {
    DISP_SMALL = 1,
    DISP_MEDIUM = 2,
    DISP_LARGE = 3,
} disp_size_t;

/**********************
 *  GLOBAL VARIABLES
 **********************/
disp_size_t disp_size;
lv_style_t style_title;
lv_style_t style_text_muted;
lv_style_t style_icon;
lv_style_t style_bullet;

const lv_font_t * font_large;
const lv_font_t * font_normal;

lv_obj_t * t1;
lv_obj_t * tv;

lv_obj_t * chart1;

lv_obj_t * scale1;
lv_obj_t * scale2;
lv_obj_t * scale3;

static uint32_t session_desktop = 1000;
static uint32_t session_tablet = 1000;
static uint32_t session_mobile = 1000;

static lv_style_t scale3_section1_main_style;
static lv_style_t scale3_section1_indicator_style;
static lv_style_t scale3_section1_tick_style;
static lv_style_t scale3_section2_main_style;
static lv_style_t scale3_section2_indicator_style;
static lv_style_t scale3_section2_tick_style;
static lv_style_t scale3_section3_main_style;
static lv_style_t scale3_section3_indicator_style;
static lv_style_t scale3_section3_tick_style;

static lv_obj_t * scale3_needle;
static lv_obj_t * scale3_mbps_label;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void color_changer_create(lv_obj_t * parent);
static void color_changer_event_cb(lv_event_t * e);
static void color_event_cb(lv_event_t * e);
static void scroll_anim_y_cb(void * var, int32_t v);
static void scroll_anim_y_cb(void * var, int32_t v);
static void slideshow_anim_completed_cb(lv_anim_t * a_old);
static void tabview_delete_event_cb(lv_event_t * e);

void lv_demo_widgets_components_init(void)
{
#if LV_USE_DEMO_BENCHMARK && LV_DEMO_BENCHMARK_ALIGNED_FONTS
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_12_aligned)
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_14_aligned)
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_16_aligned)
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_18_aligned)
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_20_aligned)
    LV_FONT_DECLARE(lv_font_benchmark_montserrat_24_aligned)
#define USE_ALIGNED_FONTS
#endif

    if(_TFT_WIDTH_ <= 320) disp_size = DISP_SMALL;
    else if(_TFT_WIDTH_ <= 480) disp_size = DISP_MEDIUM;
    else disp_size = DISP_LARGE;

    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    if(disp_size == DISP_LARGE) {
#ifdef USE_ALIGNED_FONTS
        font_large     = &lv_font_benchmark_montserrat_24_aligned;
#elif LV_FONT_MONTSERRAT_24
        font_large     = &lv_font_montserrat_24;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_24 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#ifdef USE_ALIGNED_FONTS
        font_normal    = &lv_font_benchmark_montserrat_16_aligned;
#elif LV_FONT_MONTSERRAT_16
        font_normal    = &lv_font_montserrat_16;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_16 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else if(disp_size == DISP_MEDIUM) {
#ifdef USE_ALIGNED_FONTS
        font_large     = &lv_font_benchmark_montserrat_20_aligned;
#elif LV_FONT_MONTSERRAT_20
        font_large     = &lv_font_montserrat_20;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_20 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#ifdef USE_ALIGNED_FONTS
        font_normal    = &lv_font_benchmark_montserrat_14_aligned;
#elif LV_FONT_MONTSERRAT_14
        font_normal    = &lv_font_montserrat_14;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_14 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else {   /* disp_size == DISP_SMALL */
#ifdef USE_ALIGNED_FONTS
        font_large     = &lv_font_benchmark_montserrat_18_aligned;
#elif LV_FONT_MONTSERRAT_18
        font_large     = &lv_font_montserrat_18;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_18 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#ifdef USE_ALIGNED_FONTS
        font_normal    = &lv_font_benchmark_montserrat_12_aligned;
#elif LV_FONT_MONTSERRAT_12
        font_normal    = &lv_font_montserrat_12;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_12 or LV_DEMO_BENCHMARK_ALIGNED_FONTS is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);
#endif

    lv_obj_set_style_text_font(lv_screen_active(), font_normal, 0);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);

    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);

    lv_style_init(&style_icon);
    lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
    lv_style_set_text_font(&style_icon, font_large);

    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, 0);
    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);
}

lv_obj_t * lv_demo_widgets_title_create(lv_obj_t * parent, const char * text)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_obj_add_style(label, &style_title, 0);
    lv_label_set_text_static(label, text);

    return label;
}

void scale1_indic1_anim_cb(void * var, int32_t v)
{
    lv_arc_set_value(var, v);

    lv_obj_t * card = lv_obj_get_parent(scale1);
    lv_obj_t * label = lv_obj_get_child(card, -5);
    lv_label_set_text_fmt(label, "Windows: %"LV_PRId32" %%", v);
}

void scale1_indic2_anim_cb(void * var, int32_t v)
{
    lv_arc_set_value(var, v);

    lv_obj_t * card = lv_obj_get_parent(scale1);
    lv_obj_t * label = lv_obj_get_child(card, -3);
    lv_label_set_text_fmt(label, "Linux: %"LV_PRId32" %%", v);
}

void scale1_indic3_anim_cb(void * var, int32_t v)
{
    lv_arc_set_value(var, v);

    lv_obj_t * card = lv_obj_get_parent(scale1);
    lv_obj_t * label = lv_obj_get_child(card, -1);
    lv_label_set_text_fmt(label, "Mac: %"LV_PRId32" %%", v);
}

void scale2_timer_cb(lv_timer_t * timer)
{
    LV_UNUSED(timer);

    static bool down1 = false;
    static bool down2 = false;
    static bool down3 = false;

    if(down1) {
        session_desktop -= 137;
        if(session_desktop < 1400) down1 = false;
    }
    else {
        session_desktop += 116;
        if(session_desktop > 4500) down1 = true;
    }

    if(down2) {
        session_tablet -= 3;
        if(session_tablet < 1400) down2 = false;
    }
    else {
        session_tablet += 9;
        if(session_tablet > 4500) down2 = true;
    }

    if(down3) {
        session_mobile -= 57;
        if(session_mobile < 1400) down3 = false;
    }
    else {
        session_mobile += 76;
        if(session_mobile > 4500) down3 = true;
    }

    uint32_t all = session_desktop + session_tablet + session_mobile;
    uint32_t angle1 = (session_desktop * 354) / all;
    uint32_t angle2 = (session_tablet * 354) / all;

    lv_arc_set_angles(lv_obj_get_child(scale2, 0), 0, angle1);
    lv_arc_set_angles(lv_obj_get_child(scale2, 1), angle1 + 2, angle1 + 2 + angle2);
    lv_arc_set_angles(lv_obj_get_child(scale2, 2), angle1 + 2 + angle2 + 2, 358);

    lv_obj_t * card = lv_obj_get_parent(scale2);
    lv_obj_t * label;

    label = lv_obj_get_child(card, -5);
    lv_label_set_text_fmt(label, "Windows: %"LV_PRIu32, session_desktop);

    label = lv_obj_get_child(card, -3);
    lv_label_set_text_fmt(label, "Linux: %"LV_PRIu32, session_tablet);

    label = lv_obj_get_child(card, -1);
    lv_label_set_text_fmt(label, "Mac: %"LV_PRIu32, session_mobile);
}

static void scale3_anim_cb(void * var, int32_t v)
{
    lv_obj_t * needle = lv_obj_get_child(var, 0);
    lv_scale_set_image_needle_value(var, needle, v);

    lv_obj_t * label = lv_obj_get_child(var, 1);
    lv_label_set_text_fmt(label, "%"LV_PRId32, v);
}

static void scale3_size_changed_event_cb(lv_event_t * e)
{
    LV_UNUSED(e);

    /* the center of the scale is half of the smaller dimension */
    int32_t width = lv_obj_get_width(scale3);
    int32_t height = lv_obj_get_height(scale3);
    int32_t minor_dim = LV_MIN(width, height);
    int32_t minor_dim_half = minor_dim / 2;

    /* Update needle position */
    lv_obj_align(scale3_needle, LV_ALIGN_TOP_LEFT, minor_dim_half, minor_dim_half);

    /* Update labels position */
    lv_obj_align(scale3_mbps_label, LV_ALIGN_TOP_LEFT, minor_dim_half, minor_dim * 55 / 100);
}

static void delete_timer_event_cb(lv_event_t * e)
{
    lv_event_code_t _code = lv_event_get_code(e);
    if( _code == LV_EVENT_DELETE) {
        lv_timer_t * t = lv_event_get_user_data(e);
        if(t) lv_timer_delete(t);
    }
}

void scale3_delete_event_cb(lv_event_t * e)
{
    lv_event_code_t _code = lv_event_get_code(e);

    if( _code == LV_EVENT_DELETE) {
        lv_style_reset(&scale3_section1_main_style);
        lv_style_reset(&scale3_section1_indicator_style);
        lv_style_reset(&scale3_section1_tick_style);
        lv_style_reset(&scale3_section2_main_style);
        lv_style_reset(&scale3_section2_indicator_style);
        lv_style_reset(&scale3_section2_tick_style);
        lv_style_reset(&scale3_section3_main_style);
        lv_style_reset(&scale3_section3_indicator_style);
        lv_style_reset(&scale3_section3_tick_style);
    }
}

lv_obj_t * create_scale_box(lv_obj_t * parent, const char * title, const char * text1, const char * text2,
                                   const char * text3)
{
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_height(cont, LV_SIZE_CONTENT);
    if(disp_size<DISP_LARGE) {
        lv_obj_set_width(cont, _TFT_WIDTH_);
    } else {
        lv_obj_set_flex_grow(cont, 1);
    }

    lv_obj_t * title_label = lv_demo_widgets_title_create(cont, title);

    lv_obj_t * scale = lv_scale_create(cont);
    lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_INNER);
    lv_scale_set_post_draw(scale, true);
    lv_obj_remove_style(scale, NULL, LV_PART_MAIN);
    lv_obj_remove_style(scale, NULL, LV_PART_INDICATOR);
    lv_obj_set_width(scale, LV_PCT(100));

    lv_obj_t * bullet1 = lv_obj_create(cont);
    lv_obj_set_size(bullet1, 13, 13);
    lv_obj_remove_style(bullet1, NULL, LV_PART_SCROLLBAR);
    lv_obj_add_style(bullet1, &style_bullet, 0);
    lv_obj_set_style_bg_color(bullet1, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_t * label1 = lv_label_create(cont);
    lv_label_set_text_static(label1, text1);

    lv_obj_t * bullet2 = lv_obj_create(cont);
    lv_obj_set_size(bullet2, 13, 13);
    lv_obj_remove_style(bullet2, NULL, LV_PART_SCROLLBAR);
    lv_obj_add_style(bullet2, &style_bullet, 0);
    lv_obj_set_style_bg_color(bullet2, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_t * label2 = lv_label_create(cont);
    lv_label_set_text_static(label2, text2);

    lv_obj_t * bullet3 = lv_obj_create(cont);
    lv_obj_set_size(bullet3, 13, 13);
    lv_obj_remove_style(bullet3,  NULL, LV_PART_SCROLLBAR);
    lv_obj_add_style(bullet3, &style_bullet, 0);
    lv_obj_set_style_bg_color(bullet3, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_t * label3 = lv_label_create(cont);
    lv_label_set_text_static(label3, text3);

    if(disp_size == DISP_MEDIUM) {
        static int32_t grid_col_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_FR(8), LV_GRID_TEMPLATE_LAST};
        static int32_t grid_row_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

        lv_obj_set_grid_dsc_array(cont, grid_col_dsc, grid_row_dsc);
        lv_obj_set_grid_cell(title_label, LV_GRID_ALIGN_START, 0, 4, LV_GRID_ALIGN_START, 0, 1);
        lv_obj_set_grid_cell(scale, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 1, 3);
        lv_obj_set_grid_cell(bullet1, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 2, 1);
        lv_obj_set_grid_cell(bullet2, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 3, 1);
        lv_obj_set_grid_cell(bullet3, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, 4, 1);
        lv_obj_set_grid_cell(label1, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 2, 1);
        lv_obj_set_grid_cell(label2, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 3, 1);
        lv_obj_set_grid_cell(label3, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_CENTER, 4, 1);
    }
    else {
        static int32_t grid_col_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
        static int32_t grid_row_dsc[] = {LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};
        lv_obj_set_grid_dsc_array(cont, grid_col_dsc, grid_row_dsc);
        lv_obj_set_grid_cell(title_label, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_START, 0, 1);
        lv_obj_set_grid_cell(scale, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_START, 1, 1);
        lv_obj_set_grid_cell(bullet1, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 2, 1);
        lv_obj_set_grid_cell(bullet2, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 3, 1);
        lv_obj_set_grid_cell(bullet3, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 4, 1);
        lv_obj_set_grid_cell(label1, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 2, 1);
        lv_obj_set_grid_cell(label2, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 3, 1);
        lv_obj_set_grid_cell(label3, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_START, 4, 1);
    }
    return scale;
}

void lv_demo_widgets_analytics_create(lv_obj_t * parent)
{
    uint32_t i;

    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_ROW);

    /*Create all 3 scales first to have their size resolved*/
    scale1 = create_scale_box(parent, "NECTO Active Sessions", "Windows: -%", "Linux: -%", "Mac: -%");
    lv_obj_add_flag(lv_obj_get_parent(scale1), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    scale2 = create_scale_box(parent, "PD Sessions", "Windows: -%", "Linux: -%", "Mac: -%");
    if(disp_size < DISP_LARGE) lv_obj_add_flag(lv_obj_get_parent(scale2), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    scale3 = create_scale_box(parent, "Network Speed", "Low speed", "Normal Speed", "High Speed");
    if(disp_size < DISP_LARGE) lv_obj_add_flag(lv_obj_get_parent(scale3), LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    lv_obj_update_layout(parent);
    int32_t scale_w;
    if(disp_size == DISP_MEDIUM) {
        scale_w = 200;
        lv_obj_set_size(scale1, scale_w, scale_w);
        lv_obj_set_size(scale2, scale_w, scale_w);
        lv_obj_set_size(scale3, scale_w, scale_w);
    }
    else {
        scale_w = lv_obj_get_width(scale1);
        lv_obj_set_height(scale1, scale_w);
        lv_obj_set_height(scale2, scale_w);
        lv_obj_set_height(scale3, scale_w);
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_values(&a, 20, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    /*Scale 1*/
    lv_scale_set_mode(scale1, LV_SCALE_MODE_ROUND_OUTER);
    lv_obj_set_style_pad_all(scale1, 30, 0);
    lv_obj_t * arc;
    arc = lv_arc_create(scale1);

    lv_scale_set_total_tick_count(scale1, 0);
    lv_obj_set_style_text_opa(scale1, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_line_opa(scale1, LV_OPA_0, LV_PART_ITEMS);

    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_remove_style(arc, NULL, LV_PART_MAIN);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_arc_opa(arc, 0, 0);
    lv_obj_set_style_arc_width(arc, 15, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    lv_anim_set_exec_cb(&a, scale1_indic1_anim_cb);
    lv_anim_set_var(&a, arc);
    lv_anim_set_duration(&a, 4100);
    lv_anim_set_reverse_duration(&a, 2700);
    lv_anim_start(&a);

    arc = lv_arc_create(scale1);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(arc, -20, 0);
    lv_obj_set_style_arc_opa(arc, 0, 0);
    lv_obj_set_style_arc_width(arc, 15, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);

    lv_anim_set_exec_cb(&a, scale1_indic2_anim_cb);
    lv_anim_set_var(&a, arc);
    lv_anim_set_duration(&a, 2600);
    lv_anim_set_reverse_duration(&a, 3200);
    lv_anim_start(&a);

    arc = lv_arc_create(scale1);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(arc, 20, 0);
    lv_obj_set_style_arc_opa(arc, 0, 0);
    lv_obj_set_style_arc_width(arc, 15, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);

    lv_anim_set_exec_cb(&a, scale1_indic3_anim_cb);
    lv_anim_set_var(&a, arc);
    lv_anim_set_duration(&a, 2800);
    lv_anim_set_reverse_duration(&a, 1800);
    lv_anim_start(&a);

    /*Scale 2*/
    static const char * scale2_text[] = {"0", "10", "20", "30", "40", "50", "60", "70", "80", "90", NULL};
    lv_scale_set_angle_range(scale2, 360);
    lv_scale_set_text_src(scale2, scale2_text);
    lv_scale_set_total_tick_count(scale2, 11);
    lv_obj_set_style_length(scale2, 30, LV_PART_INDICATOR);
    lv_scale_set_major_tick_every(scale2, 1);
    arc = lv_arc_create(scale2);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(arc, 10, 0);
    lv_obj_set_style_bg_opa(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 10, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, false, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);

    arc = lv_arc_create(scale2);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(arc, 5, 0);
    lv_obj_set_style_arc_opa(arc, 0, 0);
    lv_obj_set_style_bg_opa(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, false, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);

    arc = lv_arc_create(scale2);
    lv_obj_set_size(arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_arc_opa(arc, 0, 0);
    lv_obj_set_style_bg_opa(arc, 0, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 30, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(arc, false, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(arc);

    lv_timer_t * scale2_timer = lv_timer_create(scale2_timer_cb, 100, scale2);
    lv_obj_add_event_cb(scale2, delete_timer_event_cb, LV_EVENT_DELETE, scale2_timer);

    /*Scale 3*/
    lv_scale_set_range(scale3, 10, 60);
    lv_scale_set_total_tick_count(scale3, 21);
    lv_scale_set_major_tick_every(scale3, 4);
    lv_obj_set_style_length(scale3, 10, LV_PART_ITEMS);
    lv_obj_set_style_length(scale3, 20, LV_PART_INDICATOR);
    lv_scale_set_angle_range(scale3, 225);
    lv_scale_set_rotation(scale3, 135);

    lv_style_init(&scale3_section1_main_style);
    lv_style_set_arc_width(&scale3_section1_main_style, 8);
    lv_style_set_arc_color(&scale3_section1_main_style, lv_palette_main(LV_PALETTE_RED));

    lv_style_init(&scale3_section1_indicator_style);
    lv_style_set_line_width(&scale3_section1_indicator_style, 4);
    lv_style_set_line_color(&scale3_section1_indicator_style, lv_palette_darken(LV_PALETTE_RED, 2));

    lv_style_init(&scale3_section1_tick_style);
    lv_style_set_line_width(&scale3_section1_tick_style, 4);
    lv_style_set_line_color(&scale3_section1_tick_style, lv_palette_darken(LV_PALETTE_RED, 2));

    lv_style_init(&scale3_section2_main_style);
    lv_style_set_arc_width(&scale3_section2_main_style, 8);
    lv_style_set_arc_color(&scale3_section2_main_style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_init(&scale3_section2_indicator_style);
    lv_style_set_line_width(&scale3_section2_indicator_style, 4);
    lv_style_set_line_color(&scale3_section2_indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 2));

    lv_style_init(&scale3_section2_tick_style);
    lv_style_set_line_width(&scale3_section2_tick_style, 4);
    lv_style_set_line_color(&scale3_section2_tick_style, lv_palette_darken(LV_PALETTE_BLUE, 2));

    lv_style_init(&scale3_section3_main_style);
    lv_style_set_arc_width(&scale3_section3_main_style, 8);
    lv_style_set_arc_color(&scale3_section3_main_style, lv_palette_main(LV_PALETTE_GREEN));

    lv_style_init(&scale3_section3_indicator_style);
    lv_style_set_line_width(&scale3_section3_indicator_style, 4);
    lv_style_set_line_color(&scale3_section3_indicator_style, lv_palette_darken(LV_PALETTE_GREEN, 2));

    lv_style_init(&scale3_section3_tick_style);
    lv_style_set_line_width(&scale3_section3_tick_style, 4);
    lv_style_set_line_color(&scale3_section3_tick_style, lv_palette_darken(LV_PALETTE_GREEN, 2));

    lv_obj_add_event_cb(scale3, scale3_delete_event_cb, LV_EVENT_DELETE, NULL);

    lv_scale_section_t * section;
    section = lv_scale_add_section(scale3);
    lv_scale_set_section_range(scale3, section, 0, 20);
    lv_scale_set_section_style_main(scale3, section, &scale3_section1_main_style);
    lv_scale_set_section_style_indicator(scale3, section, &scale3_section1_indicator_style);
    lv_scale_set_section_style_items(scale3, section, &scale3_section1_tick_style);

    section = lv_scale_add_section(scale3);
    lv_scale_set_section_range(scale3, section, 20, 40);
    lv_scale_set_section_style_main(scale3, section, &scale3_section2_main_style);
    lv_scale_set_section_style_indicator(scale3, section, &scale3_section2_indicator_style);
    lv_scale_set_section_style_items(scale3, section, &scale3_section2_tick_style);

    section = lv_scale_add_section(scale3);
    lv_scale_set_section_range(scale3, section, 40, 60);
    lv_scale_set_section_style_main(scale3, section, &scale3_section3_main_style);
    lv_scale_set_section_style_indicator(scale3, section, &scale3_section3_indicator_style);
    lv_scale_set_section_style_items(scale3, section, &scale3_section3_tick_style);

    LV_IMAGE_DECLARE(img_demo_widgets_needle);
    scale3_needle = lv_image_create(scale3);
    lv_image_set_src(scale3_needle, &img_demo_widgets_needle);
    lv_image_set_pivot(scale3_needle, 3, 4);
    lv_obj_align(scale3_needle, LV_ALIGN_CENTER, 47, -2);

    scale3_mbps_label = lv_demo_widgets_title_create(scale3, "-");

    lv_obj_t * mbps_unit_label = lv_label_create(scale3);
    lv_label_set_text(mbps_unit_label, "Mbps");

    lv_anim_init(&a);
    lv_anim_set_values(&a, 10, 60);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&a, scale3_anim_cb);
    lv_anim_set_var(&a, scale3);
    lv_anim_set_duration(&a, 4100);
    lv_anim_set_reverse_duration(&a, 800);
    lv_anim_start(&a);

    lv_obj_align(scale3_mbps_label, LV_ALIGN_TOP_MID, 10, lv_pct(55));
    lv_obj_align_to(mbps_unit_label, scale3_mbps_label, LV_ALIGN_OUT_RIGHT_BOTTOM, 10, 0);

    lv_obj_add_event_cb(scale3, scale3_size_changed_event_cb, LV_EVENT_SIZE_CHANGED, NULL);
}

void lv_demo_widgets(void)
{
    lv_demo_widgets_components_init();

    tv = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tv, 0);
    lv_obj_add_event_cb(tv, tabview_delete_event_cb, LV_EVENT_DELETE, NULL);

    t1 = lv_tabview_add_tab(tv, "Analytics");

    lv_demo_widgets_analytics_create(t1);
}

static void tabview_delete_event_cb(lv_event_t * e)
{
    lv_event_code_t _code = lv_event_get_code(e);

    if(_code == LV_EVENT_DELETE) {
        lv_style_reset(&style_text_muted);
        lv_style_reset(&style_title);
        lv_style_reset(&style_icon);
        lv_style_reset(&style_bullet);
    }
}
