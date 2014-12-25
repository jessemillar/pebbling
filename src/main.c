#include <pebble.h>

static Window *g_window;

static TextLayer *g_text_layer_time;
static GFont *g_font_hacked;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[9]; // Enough for XX:XXxXX with a trailing space
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
	window_set_background_color(window, GColorBlack);

	g_font_hacked = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HACKED_35));
	g_text_layer_time = text_layer_create(GRect(0, 66, 144, 40));
	text_layer_set_text_color(g_text_layer_time, GColorWhite);
	text_layer_set_background_color(g_text_layer_time, GColorClear);
	text_layer_set_font(g_text_layer_time, g_font_hacked);
	text_layer_set_text_alignment(g_text_layer_time, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_layer_time));

	populate_clock();
}

static void window_unload(Window *window)
{
	text_layer_destroy(g_text_layer_time);
	fonts_unload_custom_font(g_font_hacked);
}

static void init()
{
	g_window = window_create();
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);

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