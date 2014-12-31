#include <pebble.h>

static Window *window;

static RotBitmapLayer *h_bitmap_layer_face;
static GBitmap *h_bitmap_face;

static BitmapLayer *h_bitmap_layer_digit;
static GBitmap *h_bitmap_digit;

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
	int current_hour;
	current_hour = tick_time->tm_hour;

	if (current_hour > 12)
	{
		current_hour = current_hour - 12;
	}

	gbitmap_destroy(h_bitmap_digit);

	if (h_white_digit == true)
	{
		bitmap_layer_set_compositing_mode(h_bitmap_layer_digit, GCompOpOr);
		h_bitmap_digit = gbitmap_create_with_resource(h_images_white_digits[current_hour - 1]);
	}
	else
	{
		bitmap_layer_set_compositing_mode(h_bitmap_layer_digit, GCompOpAnd);
		h_bitmap_digit = gbitmap_create_with_resource(h_images_black_digits[current_hour - 1]);
	}

	bitmap_layer_set_bitmap(h_bitmap_layer_digit, h_bitmap_digit);

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

static void tap_handler(AccelAxisType axis, int32_t direction)
{
	if (h_white_digit == true)
	{
		h_white_digit = false;
	}
	else
	{
		h_white_digit = true;
	}

	populate_clock();
}

static void window_load(Window *window)
{
	h_white_digit = true;

	h_bitmap_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	h_bitmap_layer_face = rot_bitmap_layer_create(h_bitmap_face);
	layer_set_frame(bitmap_layer_get_layer((BitmapLayer*)h_bitmap_layer_face), GRect(-85, -73, 314, 314));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer((BitmapLayer*)h_bitmap_layer_face));

	h_bitmap_layer_digit = bitmap_layer_create(GRect(0, 0, 144, 168));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_digit));

	populate_clock();
}

static void window_unload(Window *window)
{
	rot_bitmap_layer_destroy(h_bitmap_layer_face);
	gbitmap_destroy(h_bitmap_face);
	bitmap_layer_destroy(h_bitmap_layer_digit);
	gbitmap_destroy(h_bitmap_digit);
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
	accel_tap_service_subscribe(tap_handler);
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