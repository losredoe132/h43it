
// Condition functions


bool pred_button_released() {
	// TODO This is an auto-generated function, returning True everytime it gets called. PLease define the condition according to you needs. 
    if (consecutive_counts_released >3){
    return true; 
    }
    else{
        return false; 
    }
}

bool pred_button_x_long() {
	// TODO This is an auto-generated function, returning True everytime it gets called. PLease define the condition according to you needs. 
    if (consecutive_counts_pressed >50){
    return true; 
    }
    else{
        return false; 
    } 
}

bool pred_button_short_push() {
	// TODO This is an auto-generated function, returning True everytime it gets called. PLease define the condition according to you needs. 
	if (consecutive_counts_pressed >3){
    return true; 
    }
    else{
        return false; 
    }
}

bool pred_button_long_push() {
	// TODO This is an auto-generated function, returning True everytime it gets called. PLease define the condition according to you needs. 
	if (consecutive_counts_pressed >10){
    return true; 
    }
    else{
        return false; 
    }
}

