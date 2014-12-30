#include <pebble.h>

static Window *window;

static RotBitmapLayer *h_bitmap_layer_face;
static GBitmap *h_bitmap_face;

static bool h_white_digit;

static uint32_t h_images_white_digits[12] = {
	RESOURCE_ID_IMAGE_DIGIT_WHITE_1,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_2,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_3,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_4,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_5,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_6,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_7,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_8,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_9,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_10,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_11,
	RESOURCE_ID_IMAGE_DIGIT_WHITE_12
};

static uint32_t h_images_black_digits[12] = {
	RESOURCE_ID_IMAGE_DIGIT_BLACK_1,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_2,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_3,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_4,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_5,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_6,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_7,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_8,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_9,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_10,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_11,
	RESOURCE_ID_IMAGE_DIGIT_BLACK_12
};

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char hour_buffer[3]; // Enough for XX with a trailing space
	static char minute_buffer[3]; // Enough for XX with a trailing space

	if (clock_is_24h_style() == true)
	{
		// Use 24 hour format
		strftime(hour_buffer, sizeof(hour_buffer), "%k", tick_time);
	}
	else
	{
		// Use 12 hour format
		strftime(hour_buffer, sizeof(hour_buffer), "%l", tick_time);
	}

	strftime(minute_buffer, sizeof(minute_buffer), "%M", tick_time);

	rot_bitmap_layer_set_angle(h_bitmap_layer_face, tick_time->tm_min * TRIG_MAX_ANGLE / 60);
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
	h_white_digit = true;

	h_bitmap_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	h_bitmap_layer_face = rot_bitmap_layer_create(h_bitmap_face);
	layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)h_bitmap_layer_face), GRect(-85, -73, 314, 314));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer((BitmapLayer*)h_bitmap_layer_face));

	populate_clock();
}

static void window_unload(Window *window)
{
	rot_bitmap_layer_destroy(h_bitmap_layer_face);
	gbitmap_destroy(h_bitmap_face);
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