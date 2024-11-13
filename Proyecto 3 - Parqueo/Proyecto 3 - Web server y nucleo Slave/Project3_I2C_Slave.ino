#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>

// Configuración de WiFi
const char* ssid = "POCO X4 Pro 5G";  // Tu SSID aquí
const char* password = "12345678";    // Tu contraseña aquí

// Crear servidor web en el puerto 80
WebServer server(80);

// Dirección I2C
#define ESP32_ADDRESS 0x20

// Estado de los parqueos (hasta 8)
String estadoParqueo[8] = {"Libre", "Libre", "Libre", "Libre", 
                           "Libre", "Libre", "Libre", "Libre"}; 

int contador = 8;  // Contador inicial para los espacios libres

// URL de imágenes según el valor del contador
String imagenes[9] = {
  "https://svgsilh.com/svg/1280998.svg",
  "https://image.spreadshirtmedia.net/image-server/v1/products/T1459A839MPA4459PT28D304954341W7536H10000/views/1,width=800,height=800,appearanceId=839,backgroundColor=F2F2F2/la-camiseta-numero-1-del-primer-cumpleanos-la-numero-uno-pegatina.jpg",
  "https://image.spreadshirtmedia.net/image-server/v1/products/T1459A839PA4459PT28D187867945W7609H10000/views/1,width=550,height=550,appearanceId=839,backgroundColor=F2F2F2/numero-de-pegatina-numero-2-letterbox-pegatinas-pegatina.jpg",
  "https://upload.wikimedia.org/wikipedia/commons/0/09/N%C3%BAmero_3.svg",
  "https://upload.wikimedia.org/wikipedia/commons/thumb/9/9f/N%C3%BAmero_4.svg/2048px-N%C3%BAmero_4.svg.png",
  "https://image.spreadshirtmedia.net/image-server/v1/products/T1459A839MPA4459PT28D304954551W8371H10000/views/1,width=800,height=800,appearanceId=839,backgroundColor=F2F2F2/el-cumpleanos-numero-5-pegatina.jpg",
  "https://image.spreadshirtmedia.net/image-server/v1/products/T1459A839MPA4459PT28D195808123W8333H10000/views/1,width=800,height=800,appearanceId=839,backgroundColor=F2F2F2/numero-6-negro-pegatina.jpg",
  "https://upload.wikimedia.org/wikipedia/commons/thumb/e/e1/N%C3%BAmero_7.svg/2048px-N%C3%BAmero_7.svg.png",
  "https://image.spreadshirtmedia.net/image-server/v1/products/T1459A839MPA4459PT28D304954776W8210H10000/views/1,width=800,height=800,appearanceId=839,backgroundColor=F2F2F2/la-camiseta-numero-8-pegatina.jpg"
};

void setup() {
  Serial.begin(115200);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  // Configurar servidor web
  server.on("/", conexion);
  server.on("/estado", handleEstado);
  server.begin();

  // Iniciar el bus I2C en modo esclavo
  Wire.begin(ESP32_ADDRESS);

  // Configurar función de callback para recibir datos I2C
  Wire.onReceive(receiveEvent);
}

void loop() {
  server.handleClient();
}

// Función de callback que se llama cuando el ESP32 recibe datos en I2C
void receiveEvent(int bytes) {
  char buffer[50];
  int i = 0;

  while (Wire.available() && i < sizeof(buffer) - 1) {
    buffer[i++] = Wire.read();
  }
  buffer[i] = '\0';  // Convertir a cadena

  Serial.print("Datos recibidos: ");
  Serial.println(buffer);

  // Determinar el estado de los parqueos basado en los comandos recibidos
  for (int i = 0; i < 8; i++) {
    if (strcmp(buffer, ("S" + String(i + 1) + "_LOW").c_str()) == 0) {
      if (estadoParqueo[i] == "Libre") {
        estadoParqueo[i] = "<span class='status ocupado'>Ocupado</span>";
        contador--;  // Decrementar contador
      }
    } else if (strcmp(buffer, ("S" + String(i + 1) + "_HIGH").c_str()) == 0) {
      if (estadoParqueo[i] == "<span class='status ocupado'>Ocupado</span>") {
        estadoParqueo[i] = "Libre";
        contador++;  // Incrementar contador
      }
    }
  }
}

// Función para servir la página principal
void conexion() {
  String text = "<!DOCTYPE html><html>\n";
  text += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  text += "<title>SISTEMA DE PARQUEOS</title>\n";
  text += "<style>";
  text += "body { background-color: #282c34; font-family: Arial, sans-serif; color: white; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; }";
  text += ".grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; padding: 10px; max-width: 600px; }";
  text += ".parqueo { background: #444; padding: 10px; border-radius: 5px; text-align: center; }";
  text += ".status { font-weight: bold; padding: 5px; border-radius: 5px; color: white; }";
  text += ".libre { background-color: green; }";
  text += ".ocupado { background-color: red; }";
  text += ".imagen-recuadro { margin-top: 20px; display: flex; justify-content: center; }";
  text += ".imagen-recuadro img { width: 300px; height: auto; border-radius: 10px; }";
  text += "</style>";
  text += "<script>\n";
  text += "function actualizarEstado() {\n";
  text += "  var xhr = new XMLHttpRequest();\n";
  text += "  xhr.onreadystatechange = function() {\n";
  text += "    if (xhr.readyState == 4 && xhr.status == 200) {\n";
  text += "      var response = JSON.parse(xhr.responseText);\n";
  text += "      for (var i = 0; i < response.estados.length; i++) {\n";
  text += "        document.getElementById('parqueo' + (i + 1)).innerHTML = response.estados[i];\n";
  text += "        document.getElementById('parqueo' + (i + 1)).className = 'status ' + (response.estados[i].includes('Libre') ? 'libre' : 'ocupado');\n";
  text += "      }\n";
  text += "      document.getElementById('imagen').src = response.imagen;\n";
  text += "    }\n";
  text += "  };\n";
  text += "  xhr.open('GET', '/estado', true);\n";
  text += "  xhr.send();\n";
  text += "}\n";
  text += "setInterval(actualizarEstado, 1000);\n";  // Actualizar cada segundo
  text += "</script>\n";
  text += "</head>\n";
  text += "<body>\n";
  text += "<h1>Sistema de Parqueos</h1>\n";
  text += "<div class=\"grid\">\n";
  for (int i = 0; i < 8; i++) {
    text += "<div class=\"parqueo\">PARQUEO " + String(i + 1) + ": <span id='parqueo" + String(i + 1) + "' class=\"status\">" + estadoParqueo[i] + "</span></div>";
  }
  text += "</div>\n";
  text += "<div class=\"imagen-recuadro\"><img id='imagen' src='" + imagenes[contador] + "' alt='Recuadro'></div>\n";
  text += "</body></html>";

  server.send(200, "text/html", text);
}


// Función para manejar la solicitud AJAX
void handleEstado() {
  String estados = "[";
  for (int i = 0; i < 8; i++) {
    estados += "\"" + estadoParqueo[i] + "\"";
    if (i < 7) estados += ",";
  }
  estados += "]";
  String respuesta = "{\"estados\":" + estados + ",\"imagen\":\"" + imagenes[contador] + "\"}";
  server.send(200, "application/json", respuesta);
}
