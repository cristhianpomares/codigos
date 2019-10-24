 
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#define DS1307_I2C_ADDRESS 0x68
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);
const int button1Pin = 51;
const int button2Pin = 52;
const int button3Pin = 53;
byte a;

int button1S= 0; 
int button2S= 0; 
int button3S= 0; 
//reloj

int maximumRange = 1024; // maximo rango
int minimumRange = 0;   // minimo rango
float humedad, actual, sp=512, error, previous_error, drive, derivative=0, integral=0, kp=0.2, kd=0.1, ki=0.001,bandera=0;


 
//Ponemos la dirección MAC e IP que queremos que use nuestro Arduino para conectarse al Router
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0, 35);
 
String readString;
 
//Definimos que el puerto HTTP sera el 81
EthernetServer server2(80);
 
//Variables de control
int rele = 7;
 
void setup()
{
  //Iniciar la conexión de red y serie
  Ethernet.begin(mac, ip);
  server2.begin();
  Serial.begin(9600);
  pinMode(rele,OUTPUT); //Valvula Manual
  pinMode(22,OUTPUT); //electrovalvula
  
/////////////////////////reloj/////////////////////  
  pinMode(button1Pin, INPUT); 
  pinMode(button2Pin, INPUT); 
  pinMode(button3Pin, INPUT); 

  lcd.begin(16, 2); // Establecer el número de la pantalla LCD de columnas y filas:
  
  lcd.setCursor(2, 0); // arriba a la derecha
  lcd.print("RELOJ DIGITAL"); // Muestra un mensaje en la pantalla LCD.
/////////////////////////reloj///////////////////////////////////
}

int CAP=0;
int CA=0;
int H=0;
int H1=0;
int M=0;
int S=0;
int CB=0;
int H2;
int HR;
int HR2;
int M2=0;
int S2=0;


int h;
int m;
int s;
int mo;
int d;
int dM;
int y;
 
void loop()
{
 
  Serial.println(analogRead(A0)); //lectura analógica
  delay(10);//datos del sensor de humedad


//se calcula la humedasden % del sensor
humedad = analogRead(A0);

////////////////////reloj//////////////
  button1S = digitalRead(button1Pin);
  button2S = digitalRead(button2Pin);
  button3S = digitalRead(button3Pin);
 
 //////////////////////////////////  Modo 24 Horas //////////////////////////////////////////

 /////////////////////////////////// Control del Seleccionador ///////////////////////////////////////////////////////     
   if(button2S==1){
    CAP=1;
    CB=CB+1;
    delay(200);
   }


  //////////////////// 24 Horas//////////////////
  if(button1S==1 && CB==1){
     if(H1==0){
     H1=24;}
     H1=H1-1;
     H2=((H1/10)*16) + (H1%10);
     delay(200);}
     
      
  if(button3S==1 && CB==1){            
     H1=H1+1;
     H2=((H1/10)*16) + (H1%10);
     delay(200);        
     if(H1>=24){
     H1=0; }}
     
  /////////////////////////////////// Control de Inicio de Minutos /////////////////////////////////////////      
  if(button1S==1 && CB==2){
     if(M==0){
     M=60;}
     M=M-1;
     M2=((M/10)*16) + (M%10);
     delay(200);}
     
      
  if(button3S==1 && CB==2){            
     M=M+1; 
     M2=((M/10)*16) + (M%10);
     delay(200);        
     if(M>=60){
     M=0; }}
     
  /////////////////////////////////// Control de Inicio de Segundos /////////////////////////////////////////      
  if(button1S==1 && CB==3){
     if(S==0){
     S=60;}
     S=S-1;
     S2=((S/10)*16) + (S%10);
     delay(200);}
     
      
  if(button3S==1 && CB==3){            
     S=S+1;
     S2=((S/10)*16) + (S%10);
     delay(200);        
     if(S>=60){
     S=0; }}     
     
 if(CAP==1){
      
       if(CA==1){       
         lcd.setCursor(2, 1); // arriba a la derecha
         lcd.print("FORMATO 24 HORAS"); 
         delay(100);}

           if(CB==1){
           
           lcd.setCursor(2, 1); // arriba a la derecha
           lcd.print("Hora:                   ");            
           CA=0;
           lcd.setCursor(2, 1); // arriba a la derecha           
           lcd.print("Hora: ");            
           lcd.print(H1);}

           
            
           if(CB==2){
           
           lcd.setCursor(2, 1); // arriba a la derecha
           lcd.print("Minutos:                   ");            
           CA=0;
           lcd.setCursor(2, 1); // arriba a la derecha           
           lcd.print("Minutos: "); 
           lcd.print(M);}

               
           if(CB==3){
           
           lcd.setCursor(2, 1); // arriba a la derecha
           lcd.print("Segundos:                   ");            
           CA=0;
           lcd.setCursor(2, 1); // arriba a la derecha           
           lcd.print("Segundos: "); 
           lcd.print(S);}       
           
      

///////////////////////////////hora de riego///////////////////////////////////////
  if(button1S==1 && CB==4){ 
     if(HR==0){
     HR=24;}
     HR=HR-1;
     HR2=((HR/10)*16) + (HR%10);
     delay(200);}
     
      
  if(button3S==1 && CB==4){            
     HR=HR+1;
     HR2=((HR/10)*16) + (HR%10);
     delay(200);        
     if(HR>=24){
     HR=0; }}
     
 if(CAP==1){
      
       if(CA==1){        
         delay(100);}

           if(CB==4){
         lcd.setCursor(2, 0); // arriba a la derecha
         lcd.print("HORA DE RIEGO");      
           lcd.setCursor(2, 1); // arriba a la derecha
           lcd.print("Hora:                   ");            
           CA=0;
           lcd.setCursor(2, 1); // arriba a la derecha           
           lcd.print("Hora: ");            
           lcd.print(HR);}

           
         
////////////////////hora de regado///////////////////////////

          
           if(CB==5){           
             
           Wire.begin();
           
           Wire.beginTransmission(0x68); //star direccion de DS1307 en modo write       
           Wire.write(0x00);
           Wire.write(S2); //seg
           Wire.write(M2); //min
           Wire.write(H2);//hora
           Wire.write(0x07);//dia semana
           Wire.write(0x20);//dia
           Wire.write(0x10);//mes
           Wire.write(0x10);//anio
           Wire.endTransmission();
           
           }
         

           
        if (CB==6){ 
          
           Wire.beginTransmission(104); // transmit to device #104, the ds1307
           Wire.write(0x00);
           Wire.endTransmission(); // stop transmitting

           Wire.requestFrom(104, 7); // request 7 bytes from slave ds1307, we'll assume 
          // it'll send them all even though it doesn't have to
          s = Wire.read();
          m = Wire.read();
          h = Wire.read();
          d = Wire.read();
          dM = Wire.read();
          mo = Wire.read();
          y = Wire.read();

         // Convertir todos los valores BCD que podrían tener "decenas" a decimal. 
         // Mucha gente en arduino hace esto w/shits pero esto solamente lo veo más fácil.
         h=h/16 * 10 + h % 16;
         m=m/16 * 10 + m % 16;
         s=s/16 * 10 + s % 16;
                    
            lcd.setCursor(2, 0); // arriba a la derecha
        lcd.write("RELOJ DIGITAL");
        lcd.setCursor(2, 1); // arriba a la derecha
        lcd.print(h); // Muestra un mensaje en la pantalla LCD
        lcd.print(":");
        lcd.print(m); // Muestra un mensaje en la pantalla LCD
        lcd.print(":");
        lcd.print(s); // Muestra un mensaje en la pantalla LCD

 
        delay(100);
        lcd.setCursor(2, 1); // arriba a la derecha
        lcd.print("              "); // Muestra un mensaje en la pantalla LCD 
          }
              if(CB==7){
           
           CA=1;
           CB=0;
           
           } 
 }
 }
///////////////////////reloj////////////////////////     

if (h==HR || h==16){

//se define para un mejor control
if (humedad >maximumRange || humedad <minimumRange){
/*se envia -1 para indicar el duera de rango*/
Serial.println ("-1");
}

 else{
 if(humedad > sp)
 {
    digitalWrite(22,HIGH);
    error = sp - actual;
    integral = integral+ (error); //se calcula el integral
    derivative = (error - previous_error);// se calcula el deriativo
    drive = (error*kp) + (integral*ki) + (derivative*kd);//ecuaciones del PID
    previous_error = error;
 }

  else{
  
  digitalWrite(22,LOW);
  error= sp- actual;
  drive = (error*kp) + (integral*ki) + (derivative*kd);
  previous_error = error;
  
  
   }  }
}
 else{
  digitalWrite(22,LOW);
}
  
  Serial.print ("actual:");
  Serial.print (actual);
  Serial.print ("error");
  Serial.print (error);
  Serial.print ("drive:");
  Serial.print (drive);
  
  actual = humedad;
  //delay 10ms proxima lectura
  delay(10);
  // Esperamos que venga algún cliente web al puerto 81
  EthernetClient cliente = server2.available();
  if (cliente) {
    boolean currentLineIsBlank = true;
    while (cliente.connected()) {
      if (cliente.available()) {
        char c = cliente.read();
        if (readString.length() < 100) {
          //Almacenar caracteres en un string
          readString += c;
        }
       //Si ha terminado la petición del cliente
        if (c == '\n') {
           if(readString.indexOf("?encender") >0)
           {
                digitalWrite(rele,HIGH);
           }
           if(readString.indexOf("?apagar") >0)
           {
                digitalWrite(rele,LOW);
           }
          //Enviamos la cabecera web
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          //cliente.println();
          cliente.println("Connection: close");
   cliente.println("Refresh: 5");
          cliente.println();
          cliente.println("<!DOCTYPE HTML>");
          cliente.println("<html lang='es'>");
          cliente.println("<head>");
          cliente.println("<meta charset='UTF-8'>");
          cliente.println("<title>");
          cliente.println("Control de Agromatik");
          cliente.println("</title>");
          cliente.println("<link");
          cliente.println("href='http://fonts.googleapis.com/css?family=Arimo:400,700' rel='stylesheet' type='text/css'");
          cliente.println("rel='stylesheet'");
          cliente.println("type='text/css'/>");
          cliente.println("<link rel='stylesheet'");
          cliente.println("href='http://kevcorts.neocities.org/agromatikapp/styleK.css'>");
          cliente.println("</head>");
          cliente.println("<body>");
          cliente.println("<div class='page-wrap'>");
          cliente.println("<header class='header'>");
          cliente.println("<center> <h1>Bienvenido a Agromatikapp</h1> "); 
          cliente.println("<br> Clic en los enlace para encender o apagar:");  
          cliente.println("</header>");
          cliente.println("<section class='content-wrap'>");
          cliente.println("<div class='device'>");
          cliente.println("<div class='device-name'>");
          cliente.println("<h2>");
          cliente.println("CONTROL MANUAL");
          cliente.println("</h2>");
          cliente.println("</div>");
          cliente.println("<div class='forms'>");
          cliente.println("<form class='transition button on'>");
          cliente.println("<input type='button' value='ON'/>");
          cliente.println("</form></div><div class='forms'>");
          cliente.println("<form class='transition button off'>");
          cliente.println("<input type='button' value='OFF'/>");
          cliente.println("</form>");
          cliente.println("</div>");
          cliente.println("</div>");
          cliente.println("<div class='device'>");
          cliente.println("<div class='device-name'>");
          cliente.println("<h2>");
          cliente.println("Dispositivo #2");
          cliente.println("</h2>");
          cliente.println("</div>");
          cliente.println("<div class='forms'>");
          cliente.println("<form class='transition button on'>");
          cliente.println("<input type='button' value='ON'/>");
          cliente.println("</form>");
          cliente.println("</div>");
          cliente.println("<div class='forms'>");
          cliente.println("<form class='transition button off'>");
          cliente.println("<input type='button' value='OFF'/>");
          cliente.println("</form>");
          cliente.println("</div>");
          cliente.println("</div>");
          cliente.println("</section>");
          cliente.println("</div>");
          cliente.println("<br> <a href='./?encender'>Encender</a> - <a href='./?apagar'>Apagar</a>");  
          cliente.println("<button onClick=location.href='./?encender' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
          
          cliente.println(" </center>");             
          cliente.println("<br />");
          cliente.println("</body>");
          cliente.println("</html>");
          
           
          break;
        }
         
      }
    }
    // Darle un respiro al navegador web para recibir los datos
    delay(0.2);
    //Limpiar String para proxima etición HTTP
    readString="";
    //Cerrar conexión
    cliente.stop();
  }
}
