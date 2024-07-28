

bool pred_activation_missed() {
	// TODO This is an auto-generated function, returning True everytime it gets called. PLease define the condition according to you needs. 
    return false; 
}

bool pred_button_push_long() {
	if (consecutive_counts_pressed >100){
		return true;
	}
	else{
		return false;
	}
}

bool pred_button_push_short() {
	if (consecutive_counts_pressed >3){
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


bool pred_tired() {
    if (n_counts_awake > 50){
		return true; 
	}
	else{
		return false;
	}
}

bool pred_true() {
    return true; 
}


bool pred_ISRs() {
	// This is not an actual condition but handled by the Interrupt Service Routines to be
	// wake up from sleep if a button gets pressed or the Periodic Interrupt Routine is firing. 
    return true; 
}



























