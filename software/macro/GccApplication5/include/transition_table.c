// Define States 
State  Idle ,   Short ,   Long  ;

// Define transitions for each state

Transition IdleTransitions[] = {
    
    { &SHORT, pred_button_short_push}  
    
};
Transition ShortTransitions[] = {
    
    { &IDLE, pred_button_released} ,  
    
    { &LONG, pred_button_long_push}  
    
};
Transition LongTransitions[] = {
    
    { &IDLE, pred_button_released}  
    
};

// Define states with transitions

State stateA = { "Idle", IdleAction, IdleTransitions, 1 };

State stateA = { "Short", ShortAction, ShortTransitions, 2 };

State stateA = { "Long", LongAction, LongTransitions, 1 };
