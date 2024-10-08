// Define the pins for each channel
const int channelPins[] = {A1, A2, A3, A4, A5, A6};
const int numChannels = 6;

// Variables to store the pulse width of each channel
unsigned long pulseWidths[numChannels];

// Variable to store the selected channel
int selectedChannel = -1;

// Setup function
void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Set the channel pins as input
  for (int i = 0; i < numChannels; i++) {
    pinMode(channelPins[i], INPUT);
  }

  // Prompt the user for channel input
  Serial.println("Enter the channel number (1-6) to display values:");
}

// Loop function
void loop() {
  // Check if there is any serial input
  if (Serial.available() > 0) {
    // Read the selected channel from the serial monitor
    selectedChannel = Serial.parseInt();
    // Clear any residual characters from the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }

    // Validate the selected channel
    if (selectedChannel < 1 || selectedChannel > numChannels) {
      Serial.println("Invalid channel. Please enter a number between 1 and 6.");
    } else {
      Serial.print("Displaying values for Channel ");
      Serial.println(selectedChannel);
    }
  }

  // If a valid channel is selected, display its value
  if (selectedChannel >= 1 && selectedChannel <= numChannels) {
    // Read the pulse width of the selected channel
    pulseWidths[selectedChannel - 1] = pulseIn(channelPins[selectedChannel - 1], HIGH);

    // Print the pulse width to the serial monitor
    Serial.print("Channel ");
    Serial.print(selectedChannel);
    Serial.print(": ");
    Serial.print(pulseWidths[selectedChannel - 1]);
    Serial.println(" us");
  }

  // Delay to make the serial monitor output readable
  delay(500);
}
