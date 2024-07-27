// State action functions

void StopAction() {
	LEDOnById(1);
    printf("Entering state Stop \n");
}

void ShortAction() {
	LEDOnById(2);
    printf("Entering state Short \n");
}

void IdleAction() {
	LEDOnById(3);
    printf("Entering state Idle \n");
}

void LongAction() {
	LEDOnById(4);
    printf("Entering state Long \n");
}


