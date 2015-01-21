#include <pebble.h>

Window *g_window;

GBitmap *g_frame_bitmap;
BitmapLayer *g_frame_layer;

void window_load(Window *window)
{
	// Bitmap styling
	g_frame_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACE);
	g_frame_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_bitmap(g_frame_layer, g_frame_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(g_frame_layer));
}

void window_unload(Window *window)
{
	gbitmap_destroy(g_frame_bitmap);
	bitmap_layer_destroy(g_frame_layer);
}

void init()
{
	g_window = window_create();
	window_set_window_handlers(g_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

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