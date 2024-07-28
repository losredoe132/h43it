State  Activation ,   GoingToBed ,   Idle ,   MissedActivation ,   Reset  ;

// Define transitions for each state

Transition ActivationTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Reset, pred_button_push_long}  
    
};
Transition GoingToBedTransitions[] = {
    
};
Transition IdleTransitions[] = {
    
    { &Activation, pred_button_push_short} ,  
    
    { &MissedActivation, pred_activation_missed} ,  
    
    { &GoingToBed, pred_tired} ,  
    
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

State GoingToBed = { "GoingToBed", GoingToBedAction, GoingToBedTransitions, 0 };

State Idle = { "Idle", IdleAction, IdleTransitions, 4 };

State MissedActivation = { "MissedActivation", MissedActivationAction, MissedActivationTransitions, 1 };

State Reset = { "Reset", ResetAction, ResetTransitions, 1 };
