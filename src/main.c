#include <pebble.h>

Window *g_window;

static GFont *g_font_hanged;
static TextLayer *g_text_layer_time;
static BitmapLayer *g_image_layer_main;
static GBitmap *g_image_face;

static void tap_handler()
{


	layer_mark_dirty(g_image_layer_main);
}

static void populate_clock(struct tm *tick_time, TimeUnits units_changed) // Initially populate the clock so the face doesn't start blank
{
	// We need space for at least "00:00xxx" and a trailing space
	static char time_buffer[9];
	strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
	clock_copy_time_string(time_buffer, sizeof(time_buffer)); // Reformat the time to the user's preference
	text_layer_set_text(g_text_layer_time, time_buffer);
}

static void window_load(Window *window)
{
	g_image_layer_main = bitmap_layer_create(GRect(0, 0, 144, 168));
	g_image_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FINN);
	bitmap_layer_set_bitmap(g_image_layer_main, g_image_face);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(g_image_layer_main));

	g_font_hanged = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANGED_32));

	g_text_layer_time = text_layer_create(GRect(0, -10, 144, 40));
	text_layer_set_font(g_text_layer_time, g_font_hanged);
	text_layer_set_text_alignment(g_text_layer_time, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer_time));

	time_t time_temp = time(NULL);
	populate_clock(localtime(&time_temp), MINUTE_UNIT); // Manually call the function with a "fake" time
}

static void window_unload(Window *window)
{
	bitmap_layer_destroy(g_image_layer_main);
	gbitmap_destroy(g_image_face);
	text_layer_destroy(g_text_layer_time);
	fonts_unload_custom_font(g_font_hanged);
}

static void init(void)
{
	g_window = window_create();

	window_set_window_handlers(g_window, (WindowHandlers)
	{
		.load = window_load,
		.unload = window_unload,
	});

	window_stack_push(g_window, true);

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)populate_clock); // Ask for an update every minute
	accel_tap_service_subscribe(tap_handler);
}

static void deinit(void)
{
	window_destroy(g_window);
}

int main(void)
{
	init();
	app_event_loop(); // This is apparently necessary even though I don't have a function called this
	deinit();
}