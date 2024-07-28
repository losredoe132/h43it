State  Activation ,   GoingToBed ,   Idle ,   JustWokeUp ,   MissedActivation ,   Reset  ;

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
    
    { &MissedActivation, pred_activation_missed} ,  
    
    { &GoingToBed, pred_tired} ,  
    
    { &Idle, pred_true}  
    
};
Transition JustWokeUpTransitions[] = {
    
    { &Idle, pred_true}  
    
};
Transition MissedActivationTransitions[] = {
    
    { &GoingToBed, pred_true}  
    
};
Transition ResetTransitions[] = {
    
    { &Idle, pred_button_released}  
    
};

// Define states with transitions

State Activation = { "Activation", ActivationAction, ActivationTransitions, 2 };

State GoingToBed = { "GoingToBed", GoingToBedAction, GoingToBedTransitions, 1 };

State Idle = { "Idle", IdleAction, IdleTransitions, 4 };

State JustWokeUp = { "JustWokeUp", JustWokeUpAction, JustWokeUpTransitions, 1 };

State MissedActivation = { "MissedActivation", MissedActivationAction, MissedActivationTransitions, 1 };

State Reset = { "Reset", ResetAction, ResetTransitions, 1 };
