#include <ESP8266WiFi.h>
#include <os_type.h>

const int ACTIVATE_HIGH_VOLTAGE_PIN = 4;
const int BUTTON_TO_ACTIVATE_PIN = 5;
const int WELDER_READY_PIN = 14;
const int BUTTON_SAFETY_DELAY_MILLISECONDS = 2000;

const char* ssid = "<your ssid>";
const char* password = "<your password>";

boolean debounce;
boolean buttonPress;

os_timer_t debounceTimer;
os_timer_t turnSpotWelderOff;
int highVoltageOnMilliSeconds = 50;

WiFiServer server(80);

void resetDebounce(void *pArg) {
  debounce = false;
  digitalWrite(WELDER_READY_PIN, HIGH);
  Serial.println("Button ready");
}

void spotWelderOff(void *pArg) {
  digitalWrite(ACTIVATE_HIGH_VOLTAGE_PIN, LOW);
  Serial.println("Welder off");
  os_timer_arm(&debounceTimer, BUTTON_SAFETY_DELAY_MILLISECONDS, false);
}

void readButtonPress() {
  if (!debounce) {
    if (digitalRead(BUTTON_TO_ACTIVATE_PIN) == HIGH) {
      buttonPress = true;
      Serial.println("Welder on");
      digitalWrite(WELDER_READY_PIN, LOW);
      digitalWrite(ACTIVATE_HIGH_VOLTAGE_PIN, HIGH);
    }
    debounce = true;
    os_timer_arm(&turnSpotWelderOff, highVoltageOnMilliSeconds, false);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Initializing Spot Welder...");

  debounce = false;
  buttonPress = false;
  pinMode(ACTIVATE_HIGH_VOLTAGE_PIN, OUTPUT);
  pinMode(WELDER_READY_PIN, OUTPUT);
  pinMode(BUTTON_TO_ACTIVATE_PIN, INPUT);
  digitalWrite(ACTIVATE_HIGH_VOLTAGE_PIN, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTON_TO_ACTIVATE_PIN), readButtonPress, CHANGE);
  Serial.println(highVoltageOnMilliSeconds);
  Serial.println(BUTTON_SAFETY_DELAY_MILLISECONDS);
  os_timer_setfn(&debounceTimer, resetDebounce, NULL);
  os_timer_setfn(&turnSpotWelderOff, spotWelderOff, NULL);
  digitalWrite(WELDER_READY_PIN, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  server.begin();


  Serial.println("Spot Welder Initialized");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int index = request.indexOf("ontime");
  if (index > -1) {
    int temp = request.substring(index + 7).toInt();
    if (temp > 0) {
      highVoltageOnMilliSeconds = temp;
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html><head><title>Spot Welder Controller</title>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\"></head>");
  client.println("<body class=\"container\"><div class=\"row\"><div class=\" col-md-6 col-sm-3\"><h2>Spot Welder Controller</h2>");
  client.print("<p>Current time in milliseconds: <strong>");
  client.print(highVoltageOnMilliSeconds);
  client.println("</strong></p>");
  client.println("<form method=\"GET\" action=\"/\">");
  client.print("<div clas=\"form-group\"><label for=\"ontime\">On time in milliseconds</label> <input id=\"ontime\" class=\"form-control\" type=\"text\" name=\"ontime\" value=\"");
  client.print(highVoltageOnMilliSeconds);
  client.println("\" /></div>");
  client.println("<button class=\"btn btn-primary\" type=\"submit\">Update</button>");
  client.println("</form>");
  client.println("</div></div></body></html>");
  client.println("");
}
