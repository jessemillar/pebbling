#include <pebble.h>

static Window *window;

static BitmapLayer *h_bitmap_layer_face;
static GBitmap *h_bitmap_face;
static BitmapLayer *h_bitmap_layer_digit;
static GBitmap *h_bitmap_digit;

static uint32_t h_images_face[24] = {
	RESOURCE_ID_IMAGE_FACE_0,
	RESOURCE_ID_IMAGE_FACE_1,
	RESOURCE_ID_IMAGE_FACE_2,
	RESOURCE_ID_IMAGE_FACE_3,
	RESOURCE_ID_IMAGE_FACE_4,
	RESOURCE_ID_IMAGE_FACE_5,
	RESOURCE_ID_IMAGE_FACE_6,
	RESOURCE_ID_IMAGE_FACE_7,
	RESOURCE_ID_IMAGE_FACE_8,
	RESOURCE_ID_IMAGE_FACE_9,
	RESOURCE_ID_IMAGE_FACE_10,
	RESOURCE_ID_IMAGE_FACE_11,
	RESOURCE_ID_IMAGE_FACE_12,
	RESOURCE_ID_IMAGE_FACE_13,
	RESOURCE_ID_IMAGE_FACE_14,
	RESOURCE_ID_IMAGE_FACE_15,
	RESOURCE_ID_IMAGE_FACE_16,
	RESOURCE_ID_IMAGE_FACE_17,
	RESOURCE_ID_IMAGE_FACE_18,
	RESOURCE_ID_IMAGE_FACE_19,
	RESOURCE_ID_IMAGE_FACE_20,
	RESOURCE_ID_IMAGE_FACE_21,
	RESOURCE_ID_IMAGE_FACE_22,
	RESOURCE_ID_IMAGE_FACE_23
};

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
	h_bitmap_layer_face = bitmap_layer_create(GRect(0, 0, 144, 168));
	h_bitmap_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACE_1);
	bitmap_layer_set_bitmap(h_bitmap_layer_face, h_bitmap_face);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_face));

	h_bitmap_layer_digit = bitmap_layer_create(GRect(0, 0, 144, 168));
	h_bitmap_digit = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DIGIT_WHITE_1);
	bitmap_layer_set_compositing_mode(h_bitmap_layer_digit, GCompOpOr);
	bitmap_layer_set_bitmap(h_bitmap_layer_digit, h_bitmap_digit);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_digit));

	populate_clock();
}

static void window_unload(Window *window)
{
	bitmap_layer_destroy(h_bitmap_layer_face);
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