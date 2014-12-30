#include <pebble.h>

static Window *window;

static BitmapLayer *h_bitmap_layer_background;
static GBitmap *h_bitmap_background;

static GFont *h_font_helvetica;
static TextLayer *h_text_layer_time;

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
	h_bitmap_layer_background = bitmap_layer_create(GRect(-39, -27, 222, 222));
	h_bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	bitmap_layer_set_bitmap(h_bitmap_layer_background, h_bitmap_background);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_background));

	h_font_helvetica = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELVETICA_50));
	h_text_layer_time = text_layer_create(GRect(0, 20, 144, 60));
	text_layer_set_text(h_text_layer_time, "6");
	text_layer_set_text_alignment(h_text_layer_time, GTextAlignmentCenter);
	layer_add_child(window, text_layer_get_layer(h_text_layer_time));

	populate_clock();
}

static void window_unload(Window *window)
{
	bitmap_layer_destroy(h_bitmap_layer_background);
	gbitmap_destroy(h_bitmap_background);
	text_layer_destroy(h_text_layer_time);
	fonts_unload_custom_font(h_font_helvetica);
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