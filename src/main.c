#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static void window_load(Window *window)
{
  GRect bounds = layer_get_bounds(window);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window)
{
  text_layer_destroy(text_layer);
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
