  // Add your new core rgb matrix effect here, order determines enum order
 

#if defined(TRIKL_RGB)
  RGB_MATRIX_EFFECT(TRIKL_SOLID)
  #ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

/* #if defined(TRIKL_IX10_T)
const bool LED_MASK[MATRIX_ROWS][MATRIX_COLS] = {
  // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15  
  {  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
  };
#elif defined(TRIKL_IX11_T)
const bool LED_MASK[MATRIX_ROWS][MATRIX_COLS] = {
  // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15  
  {  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
  };
#elif defined(TRIKL_IX11CR_T)
const bool LED_MASK[MATRIX_ROWS][MATRIX_COLS] = {
  // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15  
  {  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
  };
#elif defined(TRIKL_IXLR_T)
const bool LED_MASK[MATRIX_ROWS][MATRIX_COLS] = {
  // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15  
  {  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
  };
#elif defined(TRIKL_IXLL_T)
const bool LED_MASK[MATRIX_ROWS][MATRIX_COLS] = {
  // 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15  
  {  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
  };
#else
	#error "Unexpected PRODUCT!"
#endif */

extern uint8_t below_press_num;
bool TRIKL_SOLID(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    //RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
	//RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
        //uint8_t row = i % MATRIX_ROWS;
        //uint8_t col = i / MATRIX_ROWS;
        //RGB_MATRIX_TEST_LED_FLAGS();
			
#if defined(TRIKL_IX10_T)   //r1        r2                                  r4                  r5                  r6=space
		if ((i<=13) || (i>=17 && i<=27) ||                     (i>=49 && i<=54) || (i>=62 && i<=67) || (i==76)) {
#elif defined(TRIKL_IX11_T) //r1        r2             r3                    r4                  r5           r6=space
		if ((i<=12) || (i>=16 && i<=27) || (i>=31 && i<=42) || (i>=49 && i<=54) || (i>=62 && i<=67) || (i==76)) {
#elif defined(TRIKL_IX11CR_T) //r1        r2             r3                    r4                  r5           r6=space
		if ((i<=12) || (i>=16 && i<=27) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69) || (i==76)) {
#elif defined(TRIKL_IXLR_T) //r1   r2             r3                    r4                  r5           r6=space
		if ((i<=13) || (i>=16 && i<=28) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69) || (i==76)) {
#elif defined(TRIKL_IXLL_T) //r1   r2             r3                    r4                  r5           r6=space
		if ((i<=13) || (i>=16 && i<=28) || (i>=31 && i<=42) || (i>=46 && i<=56) || (i>=59 && i<=69) || (i==76)) {
#else
	#error "Unexpected PRODUCT!"
#endif


	    //if ((i>=17 && i<=27) || (i>=49 && i<=54) || (i>=62 && i<=67) || (i==76)) {
			rgb_matrix_set_color(i, LED_OVERRIDES[i].R, LED_OVERRIDES[i].G, LED_OVERRIDES[i].B);
		} else if (i != TRANS_BUT_NUM){//don't light the transpose button
		//} else if (i > 13 && i != TRANS_BUT_NUM){
			if (below_press_num != 0) 
				rgb_matrix_set_color(i, 255-LED_OVERRIDES[76].G, 255, LED_OVERRIDES[76].B);//when below pressed force green on
			else
				rgb_matrix_set_color(i, 255-LED_OVERRIDES[76].G, LED_OVERRIDES[76].G, LED_OVERRIDES[76].B);
		//rgb_matrix_set_color(i, 0, 0, 0);
		}
		/*if (LED_MASK[row][col] == 0) {//XAM: if unmasked, apply solid color
            //rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			rgb_matrix_set_color(i, LED_OVERRIDES[i].R, LED_OVERRIDES[i].G, LED_OVERRIDES[i].B);
		} else { //masked ones are off (can be turned on by override)
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            //rgb_matrix_set_color(i, 0, 0, 0);
		}*/
    }
    return 1;//rgb_matrix_check_finished_leds(led_max);
}

  #endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#else
  RGB_MATRIX_EFFECT(TRIKL_NONE)
  #ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
  bool TRIKL_NONE(effect_params_t* params) {
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
      if (i != TRANS_BUT_NUM){
  			rgb_matrix_set_color(i, 0, 0, 0);
      }
    }
    return 1;
}

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#endif // TRIKL_RGB

/* RGB_MATRIX_EFFECT(TRIKL_NONE)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
bool TRIKL_NONE(effect_params_t* params) {
    for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++) {
      if (i != TRANS_BUT_NUM){
  			rgb_matrix_set_color(i, 0, 0, 0);
      }
    }
    return 1;
}

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS





#if defined(TRIKL_RGB)
  RGB_MATRIX_EFFECT(TRIKL_HEATMAP)
  #if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(RGB_MATRIX_CUSTOM_EFFECT_IMPLS) 

#        ifndef RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS
#            define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 25
#        endif


  
void process_rgb_matrix_typing_heatmap(uint8_t row, uint8_t col) {
    uint8_t m_row = row - 1;
    uint8_t p_row = row + 1;
    uint8_t m_col = col - 1;
    uint8_t p_col = col + 1;

    if (m_col < col) g_rgb_frame_buffer[row][m_col] = qadd8(g_rgb_frame_buffer[row][m_col], 16);
    g_rgb_frame_buffer[row][col] = qadd8(g_rgb_frame_buffer[row][col], 32);
    if (p_col < MATRIX_COLS) g_rgb_frame_buffer[row][p_col] = qadd8(g_rgb_frame_buffer[row][p_col], 16);

    if (p_row < MATRIX_ROWS) {
        if (m_col < col) g_rgb_frame_buffer[p_row][m_col] = qadd8(g_rgb_frame_buffer[p_row][m_col], 13);
        g_rgb_frame_buffer[p_row][col] = qadd8(g_rgb_frame_buffer[p_row][col], 16);
        if (p_col < MATRIX_COLS) g_rgb_frame_buffer[p_row][p_col] = qadd8(g_rgb_frame_buffer[p_row][p_col], 13);
    }

    if (m_row < row) {
        if (m_col < col) g_rgb_frame_buffer[m_row][m_col] = qadd8(g_rgb_frame_buffer[m_row][m_col], 13);
        g_rgb_frame_buffer[m_row][col] = qadd8(g_rgb_frame_buffer[m_row][col], 16);
        if (p_col < MATRIX_COLS) g_rgb_frame_buffer[m_row][p_col] = qadd8(g_rgb_frame_buffer[m_row][p_col], 13);
    }
}

// A timer to track the last time we decremented all heatmap values.
static uint16_t heatmap_decrease_timer;
// Whether we should decrement the heatmap values during the next update.
static bool decrease_heatmap_values;

bool TRIKL_HEATMAP(effect_params_t* params) {
    // Modified version of RGB_MATRIX_USE_LIMITS to work off of matrix row / col size
    uint8_t led_min = RGB_MATRIX_LED_PROCESS_LIMIT * params->iter;
    uint8_t led_max = led_min + RGB_MATRIX_LED_PROCESS_LIMIT;
    if (led_max > sizeof(g_rgb_frame_buffer)) led_max = sizeof(g_rgb_frame_buffer);

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
    }

    // The heatmap animation might run in several iterations depending on
    // `RGB_MATRIX_LED_PROCESS_LIMIT`, therefore we only want to update the
    // timer when the animation starts.
    if (params->iter == 0) {
        decrease_heatmap_values = timer_elapsed(heatmap_decrease_timer) >= RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS;

        // Restart the timer if we are going to decrease the heatmap this frame.
        if (decrease_heatmap_values) {
            heatmap_decrease_timer = timer_read();
        }
    }

    // Render heatmap & decrease
    for (int i = led_min; i < led_max; i++) {
        uint8_t row = i % MATRIX_ROWS;
        uint8_t col = i / MATRIX_ROWS;
        uint8_t val = g_rgb_frame_buffer[row][col];//val is the "temperature", the bigger the hotter

        // set the pixel colour
        uint8_t led[LED_HITS_TO_REMEMBER];
        uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);
        for (uint8_t j = 0; j < led_count; ++j) {
            if (!HAS_ANY_FLAGS(g_led_config.flags[led[j]], params->flags)) continue;//skip not flagged leds
			HSV hsv = rgb_matrix_config.hsv;
			if (LED_MASK[row][col] == 1) {//XAM: if unmasked, apply effect
				     // 170=blue, 0=red
				hsv.h = 170 - qsub8(val, 85);
				hsv.v = scale8((qadd8(170, val) - 170) * 3, rgb_matrix_config.hsv.v);
			}
			RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
			rgb_matrix_set_color(led[j], rgb.r, rgb.g, rgb.b);
        }

        if (decrease_heatmap_values) {
            g_rgb_frame_buffer[row][col] = qsub8(val, 1);
        }
    }

    return led_max < sizeof(g_rgb_frame_buffer);
}


#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // TRIKL_RGB */