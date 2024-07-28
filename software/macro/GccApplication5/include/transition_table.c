State  Idle ,   Long ,   Short ,   Stop ,   Stop_released ,   Stop_released_wait  ;

// Define transitions for each state

Transition IdleTransitions[] = {
    
    { &Short, pred_button_short_push}  
    
};
Transition LongTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Stop, pred_button_x_long}  
    
};
Transition ShortTransitions[] = {
    
    { &Idle, pred_button_released} ,  
    
    { &Long, pred_button_long_push}  
    
};
Transition StopTransitions[] = {
    
    { &Stop_released, pred_button_released}  
    
};
Transition Stop_releasedTransitions[] = {
    
    { &Stop_released_wait, pred_button_short_push}  
    
};
Transition Stop_released_waitTransitions[] = {
    
    { &Idle, pred_button_released}  
    
};

// Define states with transitions

State Idle = { "Idle", IdleAction, IdleTransitions, 1 };

State Long = { "Long", LongAction, LongTransitions, 2 };

State Short = { "Short", ShortAction, ShortTransitions, 2 };

State Stop = { "Stop", StopAction, StopTransitions, 1 };

State Stop_released = { "Stop_released", Stop_releasedAction, Stop_releasedTransitions, 1 };

State Stop_released_wait = { "Stop_released_wait", Stop_released_waitAction, Stop_released_waitTransitions, 1 };
