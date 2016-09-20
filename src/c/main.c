#include <pebble.h>

// pointer to main window
static Window *s_main_window;

// pointer to main window layer
static Layer *s_main_window_layer;

// pointer to graph layer
static Layer *s_graph_layer;



static void getSteps(time_t start, time_t end){
  
  // Check data is available
  HealthServiceAccessibilityMask result = health_service_metric_accessible(HealthMetricStepCount, start, end);

  if(result & HealthServiceAccessibilityMaskAvailable) {
    // Data is available! Read it
    int steps = (int) health_service_sum(HealthMetricStepCount, start, end);

    APP_LOG(APP_LOG_LEVEL_INFO, "Steps: %d", steps);
    
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "No data available!");
  }
          
  
  
}




static void graph_update_proc(Layer *this_layer, GContext *ctx) {
  
  // Set start time
  //time_t startTime = time_start_of_today();
  
  // Set end time
  //time_t endTime = time(NULL);
  
  
  // Get current time
  time_t endTime = time(NULL);
  // Get time at beginning of current hour
  endTime = endTime - (endTime % 3600);
  // Set start time to 12 hours before end time
  time_t startTime = endTime - (12 * 3600);
  
  
  
  // Log the times we are using
  APP_LOG(APP_LOG_LEVEL_INFO, "S: %d E: %d", (int) startTime, (int) endTime);
    
  // Get the step count between the times
  getSteps(startTime, endTime);
  
  
}


static void main_window_load(Window *window) {
  
  // get the main window layer
  s_main_window_layer = window_get_root_layer(s_main_window);
  
  window_set_background_color(s_main_window, GColorBlack);
  
  // Create the layer we will draw on
  s_graph_layer = layer_create(layer_get_bounds(s_main_window_layer));
  
  // Add the layer to our main window layer
  layer_add_child(s_main_window_layer, s_graph_layer);

  // Set the update procedure for our layer
  layer_set_update_proc(s_graph_layer, graph_update_proc);
  
  
}


static void main_window_unload(Window *window) {
    
}


static void init(void) {
  
  // Create the main window
  s_main_window = window_create();
  
  // set the background colour
  window_set_background_color(s_main_window, GColorBlack);
  
  // set the window load and unload handlers
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  // show the window on screen
  window_stack_push(s_main_window, true);
  
}



static void deinit(void) {
  
  // Destroy the main window
  window_destroy(s_main_window);
  
}




int main(void) {
  
  init();
  app_event_loop();
  deinit();
  
}