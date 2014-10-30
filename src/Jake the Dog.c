#include <pebble.h>

Window *g_window;

TextLayer *g_text_slogan_layer;
TextLayer *g_text_time_layer;
GBitmap *g_frame_bitmap;
BitmapLayer *g_frame_layer;

static int g_slogan_flash = 0;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[] = "00:00"; // Allocate "long-lived" storage (required by TextLayer)
	// static char buffer[] = "00:00xx"; // The "xx" is required if we want am/pm to display
	strftime(buffer, sizeof(buffer), "%H:%M", tick_time); // Write the time to the buffer in a safe manner
	clock_copy_time_string(buffer, sizeof(buffer)); // Reformat the time to the user's preference
	text_layer_set_text(g_text_time_layer, buffer); // Display the time in the text time layer

	if (g_slogan_flash == 0)
	{
		text_layer_set_text(g_text_slogan_layer, "IT'S ADVENTURE TIME!");
		g_slogan_flash = 1;
	}
	else
	{
		text_layer_set_text(g_text_slogan_layer, "");
		g_slogan_flash = 0;
	}
}

void populate_clock() // Initially populate the clock so the face doesn't start blank
{
	// Get a time structure so that the face doesn't start blank
	struct tm *t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);

	// Manually call the tick handler when the window is loading
	tick_handler(t, MINUTE_UNIT);
}

void window_load(Window *window)
{
	// Slogan styling
	g_text_slogan_layer = text_layer_create(GRect(0, 2, 144, 14));
	text_layer_set_background_color(g_text_slogan_layer, GColorClear);
	text_layer_set_text_color(g_text_slogan_layer, GColorBlack);
	text_layer_set_font(g_text_slogan_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(g_text_slogan_layer, GTextAlignmentCenter);

	// Time styling
	g_text_time_layer = text_layer_create(GRect(0, 112, 144, 30));
	text_layer_set_background_color(g_text_time_layer, GColorClear);
	text_layer_set_text_color(g_text_time_layer, GColorBlack);
	text_layer_set_font(g_text_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(g_text_time_layer, GTextAlignmentCenter);

	// Bitmap styling
	g_frame_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_JAKE);
	g_frame_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(g_frame_layer, g_frame_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(g_frame_layer));

	// Add text last
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_slogan_layer)); // Load the text layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(g_text_time_layer)); // Load the text layer

	populate_clock();
}

void window_unload(Window *window)
{
	gbitmap_destroy(g_frame_bitmap);
	bitmap_layer_destroy(g_frame_layer);

	text_layer_destroy(g_text_slogan_layer);
	text_layer_destroy(g_text_time_layer);
}

void init()
{
	g_window = window_create();
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	tick_timer_service_subscribe(SECOND_UNIT, (TickHandler)tick_handler); // Ask for an update every second

	window_stack_push(g_window, true);
}

void deinit()
{
	window_destroy(g_window);
}

int main(void)
{
	init();
	app_event_loop();
	deinit();
}