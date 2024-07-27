// Define States 
State  Idle ,   Short ,   Stop_released ,   Long ,   Stop  ;

// Define transitions for each state

Transition IdleTransitions[] = {
    
    { &Short, pred_button_short_push}  
    
};
Transition ShortTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Long, pred_button_long_push}  
    
};
Transition Stop_releasedTransitions[] = {
    
    { &Idle, pred_button_short_push}  
    
};
Transition LongTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Stop, pred_button_x_long}  
    
};
Transition StopTransitions[] = {
    
    { &Stop_released, pred_button_released}  
    
};

// Define states with transitions

State Idle = { "Idle", IdleAction, IdleTransitions, 1 };

State Short = { "Short", ShortAction, ShortTransitions, 2 };

State Stop_released = { "Stop_released", Stop_releasedAction, Stop_releasedTransitions, 1 };

State Long = { "Long", LongAction, LongTransitions, 2 };

State Stop = { "Stop", StopAction, StopTransitions, 1 };
