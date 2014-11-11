#include <pebble.h>

Window *g_window;

TextLayer *time_layer;
BitmapLayer *face_layer;

static int fps = 15; // Time between frames of the animation

static bool animating = false;
static bool eyes_closing = true; // Since closing is the first half of the animation

static int current_frame = 1;
static int frame_count = 2;

static int charge_level;
static int low_battery_level = 20; // The percent at which we consider the battery low

GBitmap *frame_1;
GBitmap *frame_2;
GBitmap *frame_low_1;
GBitmap *frame_low_2;

void display_frame() // Do things manually to keep bitmaps in memory
{
	if (charge_level > low_battery_level)
	{
		if (current_frame == 1)
		{
			bitmap_layer_set_bitmap(face_layer, frame_1);
		}
		else if (current_frame == 2)
		{
			bitmap_layer_set_bitmap(face_layer, frame_2);
		}
	}
	else
	{
		if (current_frame == 1)
		{
			bitmap_layer_set_bitmap(face_layer, frame_low_1);
		}
		else if (current_frame == 2)
		{
			bitmap_layer_set_bitmap(face_layer, frame_low_2);
		}
	}
}

void animate()
{
	if (animating == true)
	{
		if (eyes_closing)
		{
			if (current_frame < frame_count)
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

		display_frame(); // Show the new frame

		if (animating)
		{
			app_timer_register(1000 / fps, animate, NULL); // Run this function again after a pause so we can see the animation
		}
	}
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	static char buffer[] = "00:00"; // Allocate "long-lived" storage (required by TextLayer)
	// static char buffer[] = "00:00xx"; // The "xx" is required if we want am/pm to display
	strftime(buffer, sizeof(buffer), "%H:%M", tick_time); // Write the time to the buffer in a safe manner
	clock_copy_time_string(buffer, sizeof(buffer)); // Reformat the time to the user's preference
	text_layer_set_text(time_layer, buffer); // Display the time in the text time layer

	animating = true;
	animate();
}

void populate_clock() // Initially populate the clock so the face doesn't start blank
{
	// Get a time structure so that the face doesn't start blank
	struct tm *t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);

	tick_handler(t, MINUTE_UNIT); // Manually call the tick handler when the window is loading
}

void battery_handler(BatteryChargeState charge_state)
{
	charge_level = charge_state.charge_percent;
}

void window_load(Window *window)
{
	// Get the starting battery level
	BatteryChargeState temp_charge_level = battery_state_service_peek();
	charge_level = temp_charge_level.charge_percent;

	// Time styling
	time_layer = text_layer_create(GRect(0, 112, 144, 30));
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_text_color(time_layer, GColorBlack);
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

	// Initialize the bitmaps
	frame_1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAYMAX_1);
	frame_2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAYMAX_2);
	frame_low_1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAYMAX_LOW_1);
	frame_low_2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BAYMAX_LOW_2);

	// Bitmap styling
	face_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	if (charge_level > low_battery_level)
	{
		bitmap_layer_set_bitmap(face_layer, frame_1);
	}
	else
	{
		bitmap_layer_set_bitmap(face_layer, frame_low_1);
	}
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(face_layer));

	// Add text last
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer)); // Load the text layer

	populate_clock(); // Initially populate the clock so it's not blank when the face loads
}

void window_unload(Window *window)
{
	bitmap_layer_destroy(face_layer);
	gbitmap_destroy(frame_1);
	gbitmap_destroy(frame_2);
	gbitmap_destroy(frame_low_1);
	gbitmap_destroy(frame_low_2);

	text_layer_destroy(time_layer);
}

void init()
{
	g_window = window_create();
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler); // Ask for an update every minute
	battery_state_service_subscribe(battery_handler); // Subscribe to battery changes

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