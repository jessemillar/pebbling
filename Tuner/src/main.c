#include <pebble.h>

static Window *window;

static RotBitmapLayer *j_bitmap_layer_hours;
static GBitmap *j_bitmap_hours;

static RotBitmapLayer *j_bitmap_layer_mask;
static GBitmap *j_bitmap_mask;

static RotBitmapLayer *j_bitmap_layer_minutes;
static GBitmap *j_bitmap_minutes;

void rotate_hours(struct tm *tick_time, TimeUnits units_changed)
{
	rot_bitmap_layer_set_angle(j_bitmap_layer_hours, tick_time->tm_hour * TRIG_MAX_ANGLE / 12);
	rot_bitmap_layer_set_angle(j_bitmap_layer_mask, tick_time->tm_hour * TRIG_MAX_ANGLE / 12);
}

void rotate_minutes(struct tm *tick_time, TimeUnits units_changed)
{
	rot_bitmap_layer_set_angle(j_bitmap_layer_minutes, tick_time->tm_min * TRIG_MAX_ANGLE / 60);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	rotate_hours(tick_time, MINUTE_UNIT);
	rotate_minutes(tick_time, MINUTE_UNIT);
}

void populate_clock() // Initially populate the clock so the face doesn't start blank
{
	struct tm *t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);

	tick_handler(t, MINUTE_UNIT);
}

static void window_load(Window *window)
{
	window_set_background_color(window, GColorBlack);

	j_bitmap_minutes = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MINUTES);
	j_bitmap_layer_minutes = rot_bitmap_layer_create(j_bitmap_minutes);
	layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_minutes), GRect(-13, -1, 170, 170));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_minutes));

	j_bitmap_mask = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MASK);
	j_bitmap_layer_mask = rot_bitmap_layer_create(j_bitmap_mask);
	rot_bitmap_set_compositing_mode(j_bitmap_layer_mask, GCompOpAnd);
	layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_mask), GRect(22.5, 34.5, 99, 99));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_mask));

	j_bitmap_hours = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HOURS);
	j_bitmap_layer_hours = rot_bitmap_layer_create(j_bitmap_hours);
	rot_bitmap_set_compositing_mode(j_bitmap_layer_hours, GCompOpOr);
	layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_hours), GRect(22.5, 34.5, 99, 99));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer((BitmapLayer*)j_bitmap_layer_hours));

	populate_clock();
}

static void window_unload(Window *window)
{
	rot_bitmap_layer_destroy(j_bitmap_layer_hours);
	gbitmap_destroy(j_bitmap_hours);
	rot_bitmap_layer_destroy(j_bitmap_layer_mask);
	gbitmap_destroy(j_bitmap_mask);
	rot_bitmap_layer_destroy(j_bitmap_layer_minutes);
	gbitmap_destroy(j_bitmap_minutes);
}

static void init(void)
{
	window = window_create();
	
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	window_stack_push(window, true);

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
}

static void deinit(void)
{
	window_destroy(window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}