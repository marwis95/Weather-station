#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "UPC6A852F6";
const char* password = "eywavRxp5t6w";


void setup() {
  
  Serial.begin(9600);
  Serial.println();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}


void loop() 
{
  if(WiFi.isConnected())
  {
    if(Serial.available())
    {
      String temp1 = "";
      String temp2 = "";
      String humidity = "";
      String pressure = "";
      bool t1 = false;
      bool t2 = false;
      bool h = false;
      bool p = false;
      delay(100);
      while (Serial.available()) 
      {
        char c = (char)Serial.read();
        Serial.print(c);
        if(c == 's')
        {
          t1 = true;
          continue;
        }
        else if(c == 'e')
        {
          break;
        }
        else if(c == ',')
        {
          if(t1)
          {
            t1 = false;
            t2 = true;
          }
          else if(t2)
          {
            t2 = false;
            h = true;
          }
          else if(h)
          {
            h = false;
            p = true;
          }
          continue;
        }

        if(t1) temp1 += c;
        else if(t2) temp2 += c;
        else if(h) humidity += c;
        else if(p) pressure += c;
      }
      Serial.println(" ");
      Serial.print(temp1);
      Serial.print(" ");
      Serial.print(temp2);
      Serial.print(" ");
      Serial.print(humidity);
      Serial.print(" ");
      Serial.println(pressure);
      postDataToSQL(temp1, temp2, humidity, pressure);
      delay(100);

    }  
  } 
  else
  {
    delay(1000);
    ESP.restart();
  }
}




void postDataToSQL(String temp1, String temp2, String humidity, String pressure)
{
  HTTPClient http;
  String postData= "temp1=" + temp1 + "&temp2=" + temp2 + "&humidity=" + humidity  + "&pressure=" + pressure;
  http.begin("http://www.stacjapogodowa.ugu.pl/insert_to_sql.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  int httpCode = http.POST(postData); 
  String payload = http.getString();  
  Serial.println(payload);
  Serial.println('test');
  http.end();
}
