#include <pebble.h>

static Window *window;

static BitmapLayer *s_image_layer_first;
static GBitmap *s_image_first;
static BitmapLayer *s_image_layer_second;
static GBitmap *s_image_second;
static BitmapLayer *s_image_layer_third;
static GBitmap *s_image_third;
static BitmapLayer *s_image_layer_fourth;
static GBitmap *s_image_fourth;

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

	gbitmap_destroy(s_image_first); // Destroy the image before loading a different one to save RAM

	if (buffer[0] == '0')
	{	
		s_image_first = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
	}
	else if (buffer[0] == '1')
	{
		s_image_first = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
	}
	else if (buffer[0] == '2')
	{
		s_image_first = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
	}

	bitmap_layer_set_bitmap(s_image_layer_first, s_image_first);

	gbitmap_destroy(s_image_second); // Destroy the image before loading a different one to save RAM

	if (buffer[1] == '0')
	{	
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
	}
	else if (buffer[1] == '1')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
	}
	else if (buffer[1] == '2')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
	}
	else if (buffer[1] == '3')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
	}
	else if (buffer[1] == '4')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_4);
	}
	else if (buffer[1] == '5')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_5);
	}
	else if (buffer[1] == '6')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_6);
	}
	else if (buffer[1] == '7')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_7);
	}
	else if (buffer[1] == '8')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_8);
	}
	else if (buffer[1] == '9')
	{
		s_image_second = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9);
	}

	bitmap_layer_set_bitmap(s_image_layer_second, s_image_second);

	gbitmap_destroy(s_image_third); // Destroy the image before loading a different one to save RAM

	if (buffer[2] == '0')
	{	
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
	}
	else if (buffer[2] == '1')
	{
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
	}
	else if (buffer[2] == '2')
	{
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
	}
	else if (buffer[2] == '3')
	{
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
	}
	else if (buffer[2] == '4')
	{
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_4);
	}
	else if (buffer[2] == '5')
	{
		s_image_third = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_5);
	}

	bitmap_layer_set_bitmap(s_image_layer_third, s_image_third);

	gbitmap_destroy(s_image_fourth); // Destroy the image before loading a different one to save RAM

	if (buffer[3] == '0')
	{	
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
	}
	else if (buffer[3] == '1')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
	}
	else if (buffer[3] == '2')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
	}
	else if (buffer[3] == '3')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
	}
	else if (buffer[3] == '4')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_4);
	}
	else if (buffer[3] == '5')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_5);
	}
	else if (buffer[3] == '6')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_6);
	}
	else if (buffer[3] == '7')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_7);
	}
	else if (buffer[3] == '8')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_8);
	}
	else if (buffer[3] == '9')
	{
		s_image_fourth = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9);
	}

	bitmap_layer_set_bitmap(s_image_layer_fourth, s_image_fourth);
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
	gbitmap_destroy(s_image_first);
	bitmap_layer_destroy(s_image_layer_second);
	gbitmap_destroy(s_image_second);
	bitmap_layer_destroy(s_image_layer_third);
	gbitmap_destroy(s_image_third);
	bitmap_layer_destroy(s_image_layer_fourth);
	gbitmap_destroy(s_image_fourth);
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