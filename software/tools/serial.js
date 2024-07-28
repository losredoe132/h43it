// Check if the Web Serial API is supported
if ("serial" in navigator) {
  // Function to connect to the serial port
  async function connectSerial() {
    try {
      // Request a port and open a connection
      const port = await navigator.serial.requestPort();
      await port.open({ baudRate: 9600 });

      // Setup the reader
      const reader = port.readable.getReader();

      // Buffer to store incoming data until a newline character is encountered
      let buffer = "";

      // Read loop
      while (true) {
        const { value, done } = await reader.read();
        if (done) {
          // Allow the serial port to be closed later.
          reader.releaseLock();
          break;
        }
        // Decode the received data and add it to the buffer
        buffer += new TextDecoder().decode(value);

        // Check for newline character
        let newlineIndex;
        while ((newlineIndex = buffer.indexOf("\n")) >= 0) {
          // Extract the line
          const line = buffer.slice(0, newlineIndex + 1).trim();
          // Remove the line from the buffer
          buffer = buffer.slice(newlineIndex + 1);
          // Process the line (print it to the console in this case)
          document.getElementById("message-windows").innerHTML =
            line +
            "<br>" +
            document.getElementById("message-windows").innerHTML;
          if (line.startsWith("Entering ")) {
            state_serial = line.split(" ")[2];
            console.log(state_serial);
            var states = document.getElementsByClassName("state-circle");
            console.log(states);
            for (var index = 0; index < states.length; index++) {
              state = states[index];
              console.log(state);

              if (state.id == state_serial) {
                document.getElementById(state.id).style.fill = "blue";
              } else {
                document.getElementById(state.id).style.fill = "lightblue";
              }
            }
          }
        }
      }
    } catch (error) {
      console.error("There was an error:", error);
    }
  }

  // Connect to the serial port when the button is clicked
  document
    .getElementById("connectButton")
    .addEventListener("click", connectSerial);
} else {
  console.error("Web Serial API not supported.");
}
