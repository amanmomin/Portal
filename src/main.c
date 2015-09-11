#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;  
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;


static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char buffer[20] = "00:00";

strftime(buffer, sizeof(buffer),"%H\n%M", tick_time);


  text_layer_set_text(s_time_layer, buffer);
}


static void main_window_load(Window *window) {
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_portal);
  s_background_layer = bitmap_layer_create(GRect(0, -5, 144, 180));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_portal_48));

  static TextLayer *s_text_layer;
  s_text_layer = text_layer_create(GRect(0, 10, 144, 30));
  text_layer_set_text_color(s_text_layer, GColorBlack);
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_text(s_text_layer, "Portal");
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_text_layer));
 
  
//   s_text_layer = text_layer_create(GRect(0, 18, 144, 30));
//   text_layer_set_text_color(s_text_layer, GColorBlack);
//   text_layer_set_background_color(s_text_layer, GColorClear);
//   text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
//   text_layer_set_text(s_text_layer, "2");
//   layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_text_layer));
 
  s_time_layer = text_layer_create(GRect(-18, 30, 120, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");

  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    
    fonts_unload_custom_font(s_time_font);
    text_layer_destroy(s_time_layer);
    
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
 update_time();
 
}

  


static void init() {

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  update_time();
}

static void deinit() {
    window_destroy(s_main_window);
}



int main(void) {
  init();
  app_event_loop();
  deinit();
}