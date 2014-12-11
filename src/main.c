#include <pebble.h>

static Window *g_window;

static BitmapLayer *g_bitmap_layer;
static GBitmap *g_bitmap_image;

static TextLayer *g_text_layer_time;
static GFont *g_font_upheaval;

static void show_shield()
{
	gbitmap_destroy(g_bitmap_image); // Destroy the image before loading a different one to save RAM
	g_bitmap_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACE);
	bitmap_layer_set_bitmap(g_bitmap_layer, g_bitmap_image);

	layer_mark_dirty(bitmap_layer_get_layer(g_bitmap_layer));
}

static void tap_handler(AccelAxisType axis, int32_t direction)
{
	gbitmap_destroy(g_bitmap_image); // Destroy the image before loading a different one to save RAM
	g_bitmap_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACE_CRACKED);
	bitmap_layer_set_bitmap(g_bitmap_layer, g_bitmap_image);

	layer_mark_dirty(bitmap_layer_get_layer(g_bitmap_layer));

	app_timer_register(10000, show_shield, NULL); // Change to the SHIELD logo after a while
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[8]; // Enough for XX:XXxXX
	strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
	clock_copy_time_string(buffer, sizeof(buffer));
	text_layer_set_text(g_text_layer_time, buffer);
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
	// Bitmap styling
	g_bitmap_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACE);
	g_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(g_bitmap_layer, g_bitmap_image);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(g_bitmap_layer));

	g_font_upheaval = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_UPHEAVAL_20));
	g_text_layer_time = text_layer_create(GRect(0, 140, 144, 24));
	text_layer_set_text_color(g_text_layer_time, GColorWhite);
	text_layer_set_background_color(g_text_layer_time, GColorClear);
	text_layer_set_font(g_text_layer_time, g_font_upheaval);
	text_layer_set_text_alignment(g_text_layer_time, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer_time));

	populate_clock();
}

static void window_unload(Window *window)
{
	gbitmap_destroy(g_bitmap_image);
	bitmap_layer_destroy(g_bitmap_layer);
	text_layer_destroy(g_text_layer_time);
	fonts_unload_custom_font(g_font_upheaval);
}

static void init()
{
	g_window = window_create();
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
	accel_tap_service_subscribe(tap_handler);

	window_stack_push(g_window, true);
}

static void deinit()
{
	window_destroy(g_window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}