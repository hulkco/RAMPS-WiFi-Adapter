// This sketch configures the RN171 on the RAMPS WiFi Adapter. If successful, the RN171 will
// associate with your wireless network. Use the Arduino IDE Serial Monitor to observe output.
// Modify the following settings for your router. Make sure the Reset and Default pin jumpers are
// in place!

//////////////////////////////////////////////////
// Modify these settings to connect to your router
// Make sure they are exact! Double check!
//////////////////////////////////////////////////

// Type your router name here
String SSID = "My_SSID";

// Type your passphrase here
String Passphrase = "My_Passphrase";

// Uncomment ONE of the following security types...
//int Auth_Type = 0; // Open
int Auth_Type = 2; // WPA2
//int Auth_Type = 3; // WPA1
//int Auth_Type = 4; // Mixed

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

// Configuration settings for the RN171
long baud_rate = 115200;  // The baud rate between the RN171 and Mega, this must match the baud rate set in Marlin
String device_id = "RAMPS WiFi Adapter";  // If using more than one adapter, each device_id should be unique

int RST = 23;
int DFLT = 25;

const int command_buffer_size = 64;
char command[command_buffer_size];


void establish_comm() {
  Serial.begin(9600);
  Serial2.begin(9600);
}

void send_command(String str_command) {
  if (str_command != "$$$") {
      str_command += "\r\n";
  }
  str_command.toCharArray(command, command_buffer_size);
  Serial2.write(command);
}

void print_response() {
  delay(1000);
  while (Serial2.available() > 0) {
    Serial.write(Serial2.read());
  }
  Serial.write("\r\n");
}

void clear_receive_buffer() {
  delay(1000);
  while (Serial2.available() > 0) {
    Serial2.read();
  }
}

String escape_space(String string) {

  string.replace(" ", "$");
  return string;
}

void setup() {
  pinMode(RST, OUTPUT);
  pinMode(DFLT, OUTPUT);

  // The following write sequence restores the RN171 to factory defaults
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(RST, LOW);
  delay(1000);
  digitalWrite(RST, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);
  digitalWrite(DFLT, HIGH);
  delay(1000);
  digitalWrite(DFLT, LOW);
  delay(1000);

  // Reset the RN171
  digitalWrite(RST, LOW);
  delay(1000);
  digitalWrite(RST, HIGH);
  delay(1000);

  establish_comm();

  send_command("$$$");
  print_response();

  send_command("");  // A "hiccup" sometimes returns an error for the first commmand after "$$$", so send this and ignore response
  clear_receive_buffer();

  send_command(String(("set option device_id ") + escape_space(device_id)));
  print_response();
  
  send_command((String("set wlan ssid ") + escape_space(SSID)));
  print_response();

  send_command((String("set wlan auth ") + Auth_Type));
  print_response();

  send_command((String("set wlan phrase ") + escape_space(Passphrase)));
  print_response();

  send_command("set wlan linkmon 0");  // WiFly de-authenticates and re-authenticates continously with the AP when this is > 0, not sure why...
  print_response();
  
  send_command("set wlan join 1");
  print_response();
  
  send_command("set ip dhcp 1");
  print_response();

  send_command((String("set uart baud ") + baud_rate));
  print_response();

  send_command("save");
  print_response();
  
  // Reset the RN171 again
  digitalWrite(RST, LOW);
  delay(1000);
  digitalWrite(RST, HIGH);
  delay(1000);
}

void loop() {
}
