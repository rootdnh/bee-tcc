#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

const char* ssid = "";
const char* password = "";
const char* serverIP = "192.168.1.6";
const int serverPort = 3002;
const float pesoFixo = 28.3;

#define DHTPIN 4        
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE);  

int pino_sensor = 5;
int estado_sensor = 0;
int contagemDetecoes = 0;
unsigned long ultimoSegundo = 0;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  dht.begin();
}

void loop() {
  unsigned long tempoAtual = millis();

  if (tempoAtual - ultimoSegundo >= 1000) {
    Serial.print("Detecções por segundo: ");
    Serial.println(contagemDetecoes);

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      String payload = "{\"contagem\":" + String(contagemDetecoes) + 
                       ",\"temperatura\":" + String(dht.readTemperature()) + 
                       ",\"umidade\":" + String(dht.readHumidity()) + 
                       ",\"peso\":" + String(pesoFixo) + 
                       "}";

      if (http.begin(client, "http://" + String(serverIP) + ":" + String(serverPort) + "/dados")) {
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(payload);

        if (httpResponseCode > 0) {
          Serial.print("Resposta do servidor: ");
          Serial.println(http.getString());
        } else {
          Serial.print("Erro na requisição. Código de erro: ");
          Serial.println(httpResponseCode);
        }

        http.end();
      } else {
        Serial.println("Falha ao iniciar a conexão HTTP.");
      }
    }
    
    contagemDetecoes = 0;
    ultimoSegundo = tempoAtual;
  }

  estado_sensor = digitalRead(pino_sensor);
  if (estado_sensor == 0) {
    contagemDetecoes += 1;
  } 

  delay(100);
}
