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

static uint32_t s_image_layers[4] = {
	s_image_layer_first,
	s_image_layer_second,
	s_image_layer_third,
	s_image_layer_fourth
};

static uint32_t s_images[4] = {
	s_image_first,
	s_image_second,
	s_image_third,
	s_image_fourth
};

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[6]; // Enough for XX:XX with a trailing space
	strftime(buffer, sizeof(buffer), "%H:%M", tick_time);
	clock_copy_time_string(buffer, sizeof(buffer));

	for (int i = 0; i < 4; i++) // Loop through and set the four digits
	{
		int current_image = i;
		int current_character = i;

		if (current_character > 1) // Ignore the colon
		{
			current_character++;
		}

		gbitmap_destroy(s_images[current_image]); // Destroy the image before loading a different one to save RAM

		if (buffer[current_character] == '0')
		{	
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
		}
		else if (buffer[current_character] == '1')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_1);
		}
		else if (buffer[current_character] == '2')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_2);
		}
		else if (buffer[current_character] == '3')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_3);
		}
		else if (buffer[current_character] == '4')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_4);
		}
		else if (buffer[current_character] == '5')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_5);
		}
		else if (buffer[current_character] == '6')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_6);
		}
		else if (buffer[current_character] == '7')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_7);
		}
		else if (buffer[current_character] == '8')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_8);
		}
		else if (buffer[current_character] == '9')
		{
			s_images[current_image] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_9);
		}

		bitmap_layer_set_bitmap(s_image_layers[current_image], s_image_first);
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
	s_image_layer_first = bitmap_layer_create(GRect(36, 6, 72, 36));
	s_image_layer_second = bitmap_layer_create(GRect(36, 42, 72, 36));
	s_image_layer_third = bitmap_layer_create(GRect(36, 78, 72, 36));
	s_image_layer_fourth = bitmap_layer_create(GRect(36, 114, 72, 36));

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