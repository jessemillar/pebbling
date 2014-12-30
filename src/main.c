#include <pebble.h>

static Window *window;

static RotBitmapLayer *h_bitmap_layer_background;
static GBitmap *h_bitmap_background;

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[3]; // Enough for XX with a trailing space

	if (clock_is_24h_style() == true)
	{
		// Use 24 hour format
		strftime(buffer, sizeof(buffer), "%k", tick_time);
	}
	else
	{
		// Use 12 hour format
		strftime(buffer, sizeof(buffer), "%l", tick_time);
	}
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
	// h_bitmap_layer_background = rot_bitmap_layer_create(GRect(-39, -27, 222, 222));
	h_bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	h_bitmap_layer_background = rot_bitmap_layer_create(h_bitmap_background);
	// bitmap_layer_set_bitmap(h_bitmap_layer_background, h_bitmap_background);
	// layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_background));

	populate_clock();
}

static void window_unload(Window *window)
{
	rot_bitmap_layer_destroy(h_bitmap_layer_background);
	gbitmap_destroy(h_bitmap_background);
}

static void init(void)
{
	window = window_create();

	window_set_window_handlers(window, (WindowHandlers)
	{
		.load = window_load,
		.unload = window_unload,
	});

	window_stack_push(window, true);
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