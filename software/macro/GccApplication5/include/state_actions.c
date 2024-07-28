
void IdleAction() {
	LEDOnById(1);
    printf("Entering state Idle \n");
}

void LongAction() {
	LEDOnById(2);
    printf("Entering state Long \n");
}

void ShortAction() {
	LEDOnById(3);
    printf("Entering state Short \n");
}

void StopAction() {
	LEDOnById(4);
    printf("Entering state Stop \n");
}

void Stop_releasedAction() {
	LEDOnById(5);
    printf("Entering state Stop_released \n");
}

void Stop_released_waitAction() {
	LEDOnById(6);
    printf("Entering state Stop_released_wait \n");
}


