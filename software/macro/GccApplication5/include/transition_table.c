// Define States 
State  Stop ,   Short ,   Idle ,   Long  ;

// Define transitions for each state

Transition StopTransitions[] = {
    
};
Transition ShortTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Long, pred_button_long_push}  
    
};
Transition IdleTransitions[] = {
    
    { &Short, pred_button_short_push}  
    
};
Transition LongTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Stop, pred_button_x_long}  
    
};

// Define states with transitions

State Stop = { "Stop", StopAction, StopTransitions, 0 };

State Short = { "Short", ShortAction, ShortTransitions, 2 };

State Idle = { "Idle", IdleAction, IdleTransitions, 1 };

State Long = { "Long", LongAction, LongTransitions, 2 };
