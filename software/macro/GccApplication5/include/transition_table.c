State  Activation ,   GoingToBed ,   Idle ,   JustWokeUp ,   MissedActivation ,   PIT ,   Reset  ;

// Define transitions for each state

Transition ActivationTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Reset, pred_button_push_long}  
    
};
Transition GoingToBedTransitions[] = {
    
    { &JustWokeUp, pred_ISRs}  
    
};
Transition IdleTransitions[] = {
    
    { &Activation, pred_button_push_short} ,  
    
    { &GoingToBed, pred_tired} ,  
    
    { &Idle, pred_true}  
    
};
Transition JustWokeUpTransitions[] = {
    
    { &Idle, pred_is_button_ISR} ,  
    
    { &PIT, pred_is_PIT_ISR}  
    
};
Transition MissedActivationTransitions[] = {
    
    { &GoingToBed, pred_true}  
    
};
Transition PITTransitions[] = {
    
    { &MissedActivation, pred_activation_missed} ,  
    
    { &GoingToBed, pred_true}  
    
};
Transition ResetTransitions[] = {
    
    { &Idle, pred_button_released}  
    
};

// Define states with transitions

State Activation = { "Activation", ActivationAction, ActivationTransitions, 2 };

State GoingToBed = { "GoingToBed", GoingToBedAction, GoingToBedTransitions, 1 };

State Idle = { "Idle", IdleAction, IdleTransitions, 3 };

State JustWokeUp = { "JustWokeUp", JustWokeUpAction, JustWokeUpTransitions, 2 };

State MissedActivation = { "MissedActivation", MissedActivationAction, MissedActivationTransitions, 1 };

State PIT = { "PIT", PITAction, PITTransitions, 2 };

State Reset = { "Reset", ResetAction, ResetTransitions, 1 };
