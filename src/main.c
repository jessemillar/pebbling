#include <pebble.h>

static Window *window;

static BitmapLayer *h_bitmap_layer_face;
static GBitmap *h_bitmap_face;

/*
static uint32_t h_images_face[24] = {
	IMAGE_FACE_0,
	IMAGE_FACE_1,
	IMAGE_FACE_2,
	IMAGE_FACE_3,
	IMAGE_FACE_4,
	IMAGE_FACE_5,
	IMAGE_FACE_6,
	IMAGE_FACE_7,
	IMAGE_FACE_8,
	IMAGE_FACE_9,
	IMAGE_FACE_10,
	IMAGE_FACE_11,
	IMAGE_FACE_12,
	IMAGE_FACE_13,
	IMAGE_FACE_14,
	IMAGE_FACE_15,
	IMAGE_FACE_16,
	IMAGE_FACE_17,
	IMAGE_FACE_18,
	IMAGE_FACE_19,
	IMAGE_FACE_20,
	IMAGE_FACE_21,
	IMAGE_FACE_22,
	IMAGE_FACE_23
};

static uint32_t h_images_white_digits[12] = {
	IMAGE_DIGIT_WHITE_1,
	IMAGE_DIGIT_WHITE_2,
	IMAGE_DIGIT_WHITE_3,
	IMAGE_DIGIT_WHITE_4,
	IMAGE_DIGIT_WHITE_5,
	IMAGE_DIGIT_WHITE_6,
	IMAGE_DIGIT_WHITE_7,
	IMAGE_DIGIT_WHITE_8,
	IMAGE_DIGIT_WHITE_9,
	IMAGE_DIGIT_WHITE_10,
	IMAGE_DIGIT_WHITE_11,
	IMAGE_DIGIT_WHITE_12
};

static uint32_t h_images_black_digits[12] = {
	IMAGE_DIGIT_BLACK_1,
	IMAGE_DIGIT_BLACK_2,
	IMAGE_DIGIT_BLACK_3,
	IMAGE_DIGIT_BLACK_4,
	IMAGE_DIGIT_BLACK_5,
	IMAGE_DIGIT_BLACK_6,
	IMAGE_DIGIT_BLACK_7,
	IMAGE_DIGIT_BLACK_8,
	IMAGE_DIGIT_BLACK_9,
	IMAGE_DIGIT_BLACK_10,
	IMAGE_DIGIT_BLACK_11,
	IMAGE_DIGIT_BLACK_12
};
*/

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
	h_bitmap_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
	bitmap_layer_set_bitmap(h_bitmap_layer_face, h_bitmap_face);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(h_bitmap_layer_face));

	populate_clock();
}

static void window_unload(Window *window)
{
	bitmap_layer_destroy(h_bitmap_layer_face);
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