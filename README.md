# H43it 
A minimialistic habit tracker to assist you by create a lifestyle full of good habits. 

## Dev Setup 
### Python Environment
1. Create a virtual environment in order to be able to excecute the python scripts: 
`python3 -m venv .venv` 
2. Activate it: `.\.venv\Scripts\activate` (powershell on Windows) `source ./.venv/bin/activate` (linux shell). 
3. Install the required python packages: `pip install -r requirements.txt `. 

Not you are good to go!


## Finite State Machine (FSM)
The implementation is done as FSM. You can take a look at [this graph visu](FSM.html) to get an overview. Connection to serial port is only possible in Edge and Chrome browsers.

## How to use 
1. Define the FSM in the [state_machine_translator.py](software\tools\state_machine_translator.py)
2. Execute state_maschine_translator.py to get the `FSM.html` and the `transition_table.c`. 
3. You have to define the used transition conditions in `transition_conditions.c`. 
4. Build and flash the code to the board