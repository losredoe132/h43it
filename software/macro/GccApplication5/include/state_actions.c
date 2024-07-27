// State action functions

void IdleAction() {
	LEDOnById(1);
    printf("Entering state Idle \n");
}

void ShortAction() {
	LEDOnById(2);
    printf("Entering state Short \n");
}

void Stop_releasedAction() {
	LEDOnById(5);
    printf("Entering state Stop_released \n");
}

void LongAction() {
	LEDOnById(3);
    printf("Entering state Long \n");
}

void StopAction() {
	LEDOnById(4);
    printf("Entering state Stop \n");
}


