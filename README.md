# H43it 
A minimialistic habit tracker to assist you by create a lifestyle full of good habits. 


# Finite State Machine (FSM)
The implementation is done as FSM. You can take a look at [this graph visu](FSM.html) to get an overview. 

# How to use 
1. Define teh FSM in the json 
2. Execute state_maschine_translator.py to get the `FSM.html` and the `transition_table.c`. 
3. You have to define the used transition conditions in `transition_conditions.c`. 
4. Build and flash the code to the board  