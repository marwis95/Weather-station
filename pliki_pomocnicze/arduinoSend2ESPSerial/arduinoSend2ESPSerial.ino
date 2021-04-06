
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  float humidity = 133.123;
  float pressure = 1268;
  float temperature = 22.03;
  char output_str[40];
  
  char char_humidity[10];
  char char_pressure[10];
  char char_temperature[10];
  
  ftoa(humidity, char_humidity, 2);
  ftoa(pressure, char_pressure, 2);
  ftoa(temperature, char_temperature, 2);

  
  sprintf(output_str, "s%s,%s,%se", char_pressure, char_temperature, char_humidity);

  Serial.print(output_str); 

  delay(1000);   

  
}


void ftoa(float n, char* res, int afterpoint) 
{ 
    int ipart = (int)n; 
  
    float fpart = n - (float)ipart; 
  
    int i = intToStr(ipart, res, 0); 
  
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 

        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}

void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 


int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
