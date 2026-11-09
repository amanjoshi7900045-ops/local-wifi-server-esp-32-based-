/*
  LocalLink - ESP32 Offline Emergency Information Server
  Board: ESP32 Dev Module
  Sensor: DHT11 module (VCC -> 3V3, DATA -> GPIO 4, GND -> GND)
  Wi-Fi AP: LocalLink / LocalLink123
  Open: http://192.168.4.1

  Arduino libraries:
    - DHT sensor library by Adafruit
    - Adafruit Unified Sensor

  This is an educational prototype. It does not detect disasters,
  contact emergency services, or provide internet connectivity.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const char* AP_SSID = "LocalLink";
const char* AP_PASSWORD = "LocalLink123";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

const char PAGE[] PROGMEM = R"HTML(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#101827">
<title>LocalLink | Offline Emergency Help</title>
<style>
:root{--bg:#0b1220;--panel:#151f30;--panel2:#1c2940;--text:#eef4ff;--muted:#aab8cc;--accent:#38bdf8;--green:#34d399;--red:#fb7185}
*{box-sizing:border-box}body{margin:0;background:linear-gradient(160deg,#101b2d,#080d17);color:var(--text);font:16px system-ui,Arial,sans-serif}
header{padding:25px 18px 18px;text-align:center;background:#111c2d;border-bottom:1px solid #29364b}
h1{margin:0;font-size:30px;letter-spacing:.5px}header p{color:var(--muted);margin:8px auto 0;max-width:650px}
main{max-width:900px;margin:auto;padding:18px}.notice{padding:13px;border:1px solid #755b2b;background:#392d18;color:#ffe4a3;border-radius:12px;margin-bottom:16px;font-size:14px}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:13px}.card{background:var(--panel);border:1px solid #29364b;border-radius:15px;padding:17px;margin-bottom:14px}
h2{font-size:19px;margin:0 0 12px}h3{font-size:16px;margin:0 0 6px}.muted{color:var(--muted);font-size:14px}
select,button{font:inherit;border-radius:10px;padding:11px;border:1px solid #40516b;background:var(--panel2);color:var(--text)}
select{width:100%;margin:5px 0 10px}button{cursor:pointer}button.primary{background:#075985;border-color:#0284c7}
.stat{font-size:27px;font-weight:700}.actions{display:flex;flex-wrap:wrap;gap:9px}.actions a{display:inline-block;padding:10px 13px;border-radius:10px;background:#173a35;color:#b7ffe6;text-decoration:none;border:1px solid #246451}
.guide{padding:12px;background:var(--panel2);border-radius:10px;margin:8px 0}.guide p{margin:5px 0;color:var(--muted);font-size:14px}
footer{text-align:center;padding:22px;color:var(--muted);font-size:13px}
#sensorStatus{font-size:13px;color:var(--muted)}
</style>
</head>
<body>
<header>
  <h1>LocalLink</h1>
  <p id="tagline">Offline emergency information for communities with unreliable internet</p>
  <div style="margin-top:13px"><button class="primary" onclick="setLang('en')">English</button> <button onclick="setLang('or')">ଓଡ଼ିଆ</button></div>
</header>
<main>
  <div class="notice" id="notice">This local page works without mobile data. It is an information prototype, not an emergency dispatch service. If possible, call the official emergency number.</div>
  <section class="card">
    <h2 id="districtTitle">Find district information</h2>
    <label for="district" id="districtLabel">Select your district</label>
    <select id="district" onchange="showDistrict()">
      <option value="">-- Select district --</option>
      <option>Angul</option><option>Balangir</option><option>Balasore</option><option>Bargarh</option>
      <option>Bhadrak</option><option>Boudh</option><option>Cuttack</option><option>Deogarh</option>
      <option>Dhenkanal</option><option>Gajapati</option><option>Ganjam</option><option>Jagatsinghpur</option>
      <option>Jajpur</option><option>Jharsuguda</option><option>Kalahandi</option><option>Kandhamal</option>
      <option>Kendrapara</option><option>Kendujhar</option><option>Khordha</option><option>Koraput</option>
      <option>Malkangiri</option><option>Mayurbhanj</option><option>Nabarangpur</option><option>Nuapada</option>
      <option>Puri</option><option>Rayagada</option><option>Sambalpur</option><option>Subarnapur</option>
      <option>Sundargarh</option>
    </select>
    <p class="muted" id="districtInfo">Select a district to view the district name and reminder to verify local contacts.</p>
  </section>
  <div class="grid">
    <section class="card">
      <h2 id="emergencyTitle">Emergency contacts</h2>
      <div class="actions">
        <a href="tel:112">112 · All-in-one emergency</a>
        <a href="tel:108">108 · Ambulance</a>
        <a href="tel:101">101 · Fire</a>
        <a href="tel:100">100 · Police</a>
      </div>
      <p class="muted" id="contactNote">Calling requires a working mobile network and SIM. Confirm numbers and local contacts independently.</p>
    </section>
    <section class="card">
      <h2 id="sensorTitle">Local environment reading</h2>
      <div class="grid">
        <div><div class="muted" id="tempLabel">Temperature</div><div class="stat" id="temp">-- °C</div></div>
        <div><div class="muted" id="humidityLabel">Humidity</div><div class="stat" id="humidity">-- %</div></div>
      </div>
      <p id="sensorStatus">Waiting for DHT11 reading…</p>
    </section>
  </div>
  <section class="card">
    <h2 id="safetyTitle">Basic safety guidance</h2>
    <div class="guide"><h3 id="floodHead">Flood</h3><p id="floodText">Move to higher ground. Avoid walking or driving through floodwater. Switch off electricity only if it is safe to do so.</p></div>
    <div class="guide"><h3 id="cycloneHead">Cyclone / severe storm</h3><p id="cycloneText">Stay indoors away from windows. Keep drinking water, a torch, medicines and charged devices ready. Follow official evacuation instructions.</p></div>
    <div class="guide"><h3 id="fireHead">Fire</h3><p id="fireText">Raise the alarm and leave by a safe exit. Do not use lifts. Stay low under smoke. Do not re-enter a burning building.</p></div>
    <div class="guide"><h3 id="heatHead">Extreme heat</h3><p id="heatText">Stay in shade or a cool place, drink water regularly and avoid strenuous outdoor activity during peak heat.</p></div>
  </section>
</main>
<footer>LocalLink · ESP32 local Wi-Fi prototype · Information should be verified with official authorities.</footer>
<script>
const translations={
en:{tag:"Offline emergency information for communities with unreliable internet",notice:"This local page works without mobile data. It is an information prototype, not an emergency dispatch service. If possible, call the official emergency number.",district:"Find district information",districtLabel:"Select your district",districtInfo:"Select a district to view the district name and reminder to verify local contacts.",emergency:"Emergency contacts",contact:"Calling requires a working mobile network and SIM. Confirm numbers and local contacts independently.",sensor:"Local environment reading",temp:"Temperature",humidity:"Humidity",waiting:"Waiting for DHT11 reading…",safety:"Basic safety guidance",floodH:"Flood",floodT:"Move to higher ground. Avoid walking or driving through floodwater. Switch off electricity only if it is safe to do so.",cycloneH:"Cyclone / severe storm",cycloneT:"Stay indoors away from windows. Keep drinking water, a torch, medicines and charged devices ready. Follow official evacuation instructions.",fireH:"Fire",fireT:"Raise the alarm and leave by a safe exit. Do not use lifts. Stay low under smoke. Do not re-enter a burning building.",heatH:"Extreme heat",heatT:"Stay in shade or a cool place, drink water regularly and avoid strenuous outdoor activity during peak heat."},
or:{tag:"ଅନିୟମିତ ଇଣ୍ଟରନେଟ୍ ଥିବା ଅଞ୍ଚଳ ପାଇଁ ଜରୁରୀ ସୂଚନା",notice:"ଏହି ପୃଷ୍ଠା ମୋବାଇଲ୍ ଡାଟା ବିନା ଚାଲେ। ଏହା ଏକ ସୂଚନାମୂଳକ ପ୍ରୋଟୋଟାଇପ୍, ଜରୁରୀ ସେବା ପଠାଏ ନାହିଁ। ସମ୍ଭବ ହେଲେ ସରକାରୀ ଜରୁରୀ ନମ୍ବରକୁ ଫୋନ୍ କରନ୍ତୁ।",district:"ଜିଲ୍ଲା ସୂଚନା ଖୋଜନ୍ତୁ",districtLabel:"ଆପଣଙ୍କ ଜିଲ୍ଲା ବାଛନ୍ତୁ",districtInfo:"ଜିଲ୍ଲା ବାଛନ୍ତୁ। ସ୍ଥାନୀୟ ଯୋଗାଯୋଗ ନମ୍ବର ସରକାରୀ ଉତ୍ସରୁ ଯାଞ୍ଚ କରନ୍ତୁ।",emergency:"ଜରୁରୀ ଯୋଗାଯୋଗ",contact:"ଫୋନ୍ କରିବା ପାଇଁ ମୋବାଇଲ୍ ନେଟୱର୍କ ଓ ସିମ୍ ଆବଶ୍ୟକ। ନମ୍ବରଗୁଡ଼ିକ ଯାଞ୍ଚ କରନ୍ତୁ।",sensor:"ସ୍ଥାନୀୟ ପରିବେଶ ମାପ",temp:"ତାପମାତ୍ରା",humidity:"ଆର୍ଦ୍ରତା",waiting:"DHT11 ରିଡିଂ ପାଇଁ ଅପେକ୍ଷା…",safety:"ମୌଳିକ ସୁରକ୍ଷା ନିର୍ଦ୍ଦେଶ",floodH:"ବନ୍ୟା",floodT:"ଉଚ୍ଚ ସ୍ଥାନକୁ ଯାଆନ୍ତୁ। ବନ୍ୟା ପାଣିରେ ଚାଲନ୍ତୁ ନାହିଁ କିମ୍ବା ଗାଡ଼ି ଚଳାନ୍ତୁ ନାହିଁ।",cycloneH:"ବାତ୍ୟା / ଝଡ଼",cycloneT:"ଝରକାଠାରୁ ଦୂରେ ଘର ଭିତରେ ରୁହନ୍ତୁ। ପାଣି, ଟର୍ଚ୍ଚ ଓ ଔଷଧ ପ୍ରସ୍ତୁତ ରଖନ୍ତୁ। ସରକାରୀ ନିର୍ଦ୍ଦେଶ ମାନନ୍ତୁ।",fireH:"ଅଗ୍ନିକାଣ୍ଡ",fireT:"ସତର୍କ କରନ୍ତୁ ଏବଂ ସୁରକ୍ଷିତ ରାସ୍ତାରେ ବାହାରିଯାଆନ୍ତୁ। ଲିଫ୍ଟ ବ୍ୟବହାର କରନ୍ତୁ ନାହିଁ।",heatH:"ଅତ୍ୟଧିକ ଗରମ",heatT:"ଛାଇ କିମ୍ବା ଥଣ୍ଡା ସ୍ଥାନରେ ରୁହନ୍ତୁ, ନିୟମିତ ପାଣି ପିଅନ୍ତୁ ଏବଂ ଅଧିକ ଗରମ ସମୟରେ ବାହାର କାମ ଏଡ଼ାନ୍ତୁ।"}
};
let lang="en";
function setLang(code){lang=code;const t=translations[lang];const ids={tagline:"tag",notice:"notice",districtTitle:"district",districtLabel:"districtLabel",districtInfo:"districtInfo",emergencyTitle:"emergency",contactNote:"contact",sensorTitle:"sensor",tempLabel:"temp",humidityLabel:"humidity",sensorStatus:"waiting",safetyTitle:"safety",floodHead:"floodH",floodText:"floodT",cycloneHead:"cycloneH",cycloneText:"cycloneT",fireHead:"fireH",fireText:"fireT",heatHead:"heatH",heatText:"heatT"};Object.keys(ids).forEach(id=>document.getElementById(id).textContent=t[ids[id]]);}
function showDistrict(){const d=document.getElementById("district").value;document.getElementById("districtInfo").textContent=d?(lang==="or"?"ବାଛିଥିବା ଜିଲ୍ଲା: ":"Selected district: ")+d+(lang==="or"?"। ସ୍ଥାନୀୟ ଜରୁରୀ ନମ୍ବର ସରକାରୀ ଉତ୍ସରୁ ଯାଞ୍ଚ କରନ୍ତୁ।":". Verify local emergency contacts with official authorities.") : translations[lang].districtInfo;}
async function readSensor(){try{const r=await fetch("/api/sensor",{cache:"no-store"});if(!r.ok)throw Error("HTTP "+r.status);const d=await r.json();if(d.ok){document.getElementById("temp").textContent=d.temperature.toFixed(1)+" °C";document.getElementById("humidity").textContent=d.humidity.toFixed(1)+" %";document.getElementById("sensorStatus").textContent="DHT11 · "+(lang==="or"?"ସାମ୍ପ୍ରତିକ ରିଡିଂ":"Live local reading");}else{document.getElementById("sensorStatus").textContent=lang==="or"?"ସେନ୍ସର ରିଡିଂ ଉପଲବ୍ଧ ନାହିଁ":"Sensor reading unavailable. Check DHT11 wiring."}}catch(e){document.getElementById("sensorStatus").textContent=lang==="or"?"ସେନ୍ସର ସହ ଯୋଗାଯୋଗ ହୋଇପାରିଲା ନାହିଁ":"Could not read sensor. Check connection.";}}
setInterval(readSensor,3000);readSensor();
</script>
</body></html>
)HTML";

void handleRoot() {
  server.send_P(200, "text/html; charset=utf-8", PAGE);
}

void handleSensor() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    server.send(503, "application/json", "{\"ok\":false,\"error\":\"DHT11 read failed\"}");
    return;
  }

  String json = "{\"ok\":true,\"temperature\":";
  json += String(temperature, 1);
  json += ",\"humidity\":";
  json += String(humidity, 1);
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();

  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASSWORD);

  if (apStarted) {
    Serial.println();
    Serial.println("LocalLink access point started.");
    Serial.print("Wi-Fi name: ");
    Serial.println(AP_SSID);
    Serial.print("Wi-Fi password: ");
    Serial.println(AP_PASSWORD);
    Serial.print("Open this address: http://");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("ERROR: Could not start Wi-Fi access point.");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/sensor", HTTP_GET, handleSensor);
  server.onNotFound([]() {
    server.send(404, "text/plain", "LocalLink: page not found");
  });

  server.begin();
  Serial.println("LocalLink web server is ready.");
}

void loop() {
  server.handleClient();
}
