#include <pebble.h>

static Window *window;

static BitmapLayer *s_image_layer_first;
static BitmapLayer *s_image_layer_second;
static BitmapLayer *s_image_layer_third;
static BitmapLayer *s_image_layer_fourth;

static GBitmap *s_image_digit_0;
static GBitmap *s_image_digit_1;
static GBitmap *s_image_digit_2;
static GBitmap *s_image_digit_3;
static GBitmap *s_image_digit_4;
static GBitmap *s_image_digit_5;
static GBitmap *s_image_digit_6;
static GBitmap *s_image_digit_7;
static GBitmap *s_image_digit_8;
static GBitmap *s_image_digit_9;

void set_digit(BitmapLayer *image_layer, char character)
{
	if (character == '0')
	{	
		bitmap_layer_set_bitmap(image_layer, s_image_digit_0);
	}
	else if (character == '1')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_1);
	}
	else if (character == '2')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_2);
	}
	else if (character == '3')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_3);
	}
	else if (character == '4')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_4);
	}
	else if (character == '5')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_5);
	}
	else if (character == '6')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_6);
	}
	else if (character == '7')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_7);
	}
	else if (character == '8')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_8);
	}
	else if (character == '9')
	{
		bitmap_layer_set_bitmap(image_layer, s_image_digit_9);
	}
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[5]; // Enough for XXXX with a trailing space

	if (clock_is_24h_style() == true)
	{
		// Use 24 hour format
		strftime(buffer, sizeof(buffer), "%H%M", tick_time);
	}
	else
	{
		// Use 12 hour format
		strftime(buffer, sizeof(buffer), "%I%M", tick_time);
	}

	set_digit(s_image_layer_first, buffer[0]);
	set_digit(s_image_layer_second, buffer[1]);
	set_digit(s_image_layer_third, buffer[2]);
	set_digit(s_image_layer_fourth, buffer[3]);
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

	s_image_digit_0 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
	s_image_digit_1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
	s_image_digit_2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
	s_image_digit_3 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
	s_image_digit_4 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_4);
	s_image_digit_5 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_5);
	s_image_digit_6 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_6);
	s_image_digit_7 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_7);
	s_image_digit_8 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_8);
	s_image_digit_9 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9);

	s_image_layer_first = bitmap_layer_create(GRect(36, 6, 72, 36));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer_first));
	s_image_layer_second = bitmap_layer_create(GRect(36, 46, 72, 36));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer_second));
	s_image_layer_third = bitmap_layer_create(GRect(36, 86, 72, 36));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer_third));
	s_image_layer_fourth = bitmap_layer_create(GRect(36, 126, 72, 36));
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_image_layer_fourth));

	populate_clock();
}

static void window_unload(Window *window)
{
	bitmap_layer_destroy(s_image_layer_first);
	bitmap_layer_destroy(s_image_layer_second);
	bitmap_layer_destroy(s_image_layer_third);
	bitmap_layer_destroy(s_image_layer_fourth);

	gbitmap_destroy(s_image_digit_0);
	gbitmap_destroy(s_image_digit_1);
	gbitmap_destroy(s_image_digit_2);
	gbitmap_destroy(s_image_digit_3);
	gbitmap_destroy(s_image_digit_4);
	gbitmap_destroy(s_image_digit_5);
	gbitmap_destroy(s_image_digit_6);
	gbitmap_destroy(s_image_digit_7);
	gbitmap_destroy(s_image_digit_8);
	gbitmap_destroy(s_image_digit_9);
}

static void init(void)
{
	window = window_create();

	window_set_window_handlers(window, (WindowHandlers)
	{
		.load = window_load,
		.unload = window_unload,
	});

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);

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