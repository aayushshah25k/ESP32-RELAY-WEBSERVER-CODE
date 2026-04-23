#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_HOTSPOT_NAME";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

// Relay pins
#define RELAY3 21
#define RELAY4 19

bool lightState = false;
bool fanState = false;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to Hotspot");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handleRoot);

  server.on("/r3on", [](){
    digitalWrite(RELAY3, LOW);
    lightState = true;
    server.send(200, "text/plain", "OK");
  });

  server.on("/r3off", [](){
    digitalWrite(RELAY3, HIGH);
    lightState = false;
    server.send(200, "text/plain", "OK");
  });

  server.on("/r4on", [](){
    digitalWrite(RELAY4, LOW);
    fanState = true;
    server.send(200, "text/plain", "OK");
  });

  server.on("/r4off", [](){
    digitalWrite(RELAY4, HIGH);
    fanState = false;
    server.send(200, "text/plain", "OK");
  });

  server.on("/state", [](){
    String json = "{\"light\":";
    json += lightState ? "1" : "0";
    json += ",\"fan\":";
    json += fanState ? "1" : "0";
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body { font-family: Arial; text-align:center; background:#0f172a; color:white;}
.card { background:#1e293b; margin:20px; padding:20px; border-radius:15px;}
.switch { position:relative; display:inline-block; width:60px; height:34px;}
.switch input {display:none;}
.slider { position:absolute; cursor:pointer; background:#ccc; border-radius:34px; top:0; bottom:0; left:0; right:0;}
.slider:before { content:""; position:absolute; height:26px; width:26px; left:4px; bottom:4px; background:white; border-radius:50%; transition:.4s;}
input:checked + .slider { background:#22c55e;}
input:checked + .slider:before { transform:translateX(26px);}
.spin { animation:spin 1s linear infinite;}
@keyframes spin {100%{transform:rotate(360deg);}}
</style>
</head>

<body>

<h1>ESP32 Control</h1>

<div class="card">
<h2>Light</h2>
<label class="switch">
<input type="checkbox" id="light">
<span class="slider"></span>
</label>
<p id="ls">OFF</p>
</div>

<div class="card">
<h2>Fan</h2>
<label class="switch">
<input type="checkbox" id="fan">
<span class="slider"></span>
</label>
<div id="fanIcon">🌀</div>
<p id="fs">OFF</p>
</div>

<script>
function load(){
fetch('/state').then(r=>r.json()).then(d=>{
light.checked=d.light;
fan.checked=d.fan;
update();
});
}

function update(){
ls.innerText = light.checked?"ON":"OFF";
fs.innerText = fan.checked?"ON":"OFF";
fanIcon.className = fan.checked?"spin":"";
}

light.onchange=()=>{
fetch(light.checked?'/r3on':'/r3off');
update();
}

fan.onchange=()=>{
fetch(fan.checked?'/r4on':'/r4off');
update();
}

load();
</script>

</body>
</html>
)====";

  server.send(200, "text/html", html);
}
