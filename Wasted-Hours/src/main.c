#include <pebble.h>

// These aren't values, they're "keys"
#define J_CURRENT_COUNT 9001

static int j_count;

static Window *window;
static TextLayer *text_layer;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	j_count++; // Log another second

	static char buffer[] = "00:00:00"; // Allocate "long-lived" storage (required by TextLayer)

	snprintf(buffer, sizeof(buffer), "%d", j_count / 60); // Display an int in a char buffer

	text_layer_set_text(text_layer, buffer); // Display the time in the text time layer
}

static void window_load(Window *window)
{
	text_layer = text_layer_create(GRect(0, 52, 144, 35));
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
}

static void window_unload(Window *window)
{
	text_layer_destroy(text_layer);
}

static void init(void)
{
	// Load the value from storage if it exists
	j_count = persist_exists(J_CURRENT_COUNT) ? persist_read_int(J_CURRENT_COUNT) : 0;

	window = window_create();

	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	window_stack_push(window, true);

	tick_timer_service_subscribe(SECOND_UNIT, (TickHandler)tick_handler); // Ask for an update every minute
}

static void deinit(void)
{
	persist_write_int(J_CURRENT_COUNT, j_count);

	window_destroy(window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}