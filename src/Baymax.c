#include <pebble.h>

Window *g_window;

TextLayer *time_layer;
GBitmap *bitmap;
GBitmap *temp_bitmap;
BitmapLayer *face_layer;

static int fps = 25; // Time between frames of the animation

static int second_count = 0;
static int blink_interval = 3; // Blink after this many seconds

static bool animating = false;
static bool eyes_closing = true; // Since closing is the first half of the animation

static int current_frame = 0;
static int framecount = 3; // Apparently I can't set the size of an array with a variable
static int frames[3] = {
	RESOURCE_ID_IMAGE_BAYMAX_1,
	RESOURCE_ID_IMAGE_BAYMAX_2,
	RESOURCE_ID_IMAGE_BAYMAX_3
};

void animate()
{
	if (animating == true)
	{
		if (eyes_closing)
		{
			if (current_frame < framecount - 1)
			{
				current_frame++;
			}
			else
			{
				eyes_closing = false;
			}
		}
		else
		{
			if (current_frame > 0)
			{
				current_frame--;
			}
			else
			{
				// "Reset" the animation so it can run through again later
				eyes_closing = true;
				animating = false;
			}
		}

		// Show the new frame
		temp_bitmap = gbitmap_create_with_resource(frames[current_frame]);
		bitmap_layer_set_bitmap(face_layer, temp_bitmap);

		if (animating)
		{
			app_timer_register(1000 / fps, animate, NULL); // Run this function again after a pause so we can see the animation
		}
	}
}

void blink()
{
	if (second_count == blink_interval)
	{
		second_count = 0;
		animating = true;
		animate();
	}
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[] = "00:00"; // Allocate "long-lived" storage (required by TextLayer)
	// static char buffer[] = "00:00xx"; // The "xx" is required if we want am/pm to display
	strftime(buffer, sizeof(buffer), "%H:%M", tick_time); // Write the time to the buffer in a safe manner
	clock_copy_time_string(buffer, sizeof(buffer)); // Reformat the time to the user's preference
	text_layer_set_text(time_layer, buffer); // Display the time in the text time layer

	if (second_count < blink_interval)
	{
		second_count++;
		blink();
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
	// Time styling
	time_layer = text_layer_create(GRect(0, 112, 144, 30));
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, GColorBlack);
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

	// Bitmap styling
	bitmap = gbitmap_create_with_resource(frames[0]); // Start on the first frame
	face_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(face_layer, bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(face_layer));

	// Add text last
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer)); // Load the text layer

	populate_clock();
}

void window_unload(Window *window)
{
	gbitmap_destroy(bitmap);
	gbitmap_destroy(temp_bitmap);
	bitmap_layer_destroy(face_layer);

	text_layer_destroy(time_layer);
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