

bool pred_button_long_push() {
	if (consecutive_counts_pressed >10){
    return true; 
    }
    else{
        return false; 
    }
}

bool pred_button_released() {
	if (consecutive_counts_released >3){
    return true; 
    }
    else{
        return false; 
    }
}

bool pred_button_short_push() {
	if (consecutive_counts_pressed >3){
    return true; 
    }
    else{
        return false; 
    }
}

bool pred_button_x_long() {
	if (consecutive_counts_pressed >50){
    return true; 
    }
    else{
        return false; 
    } 
}

