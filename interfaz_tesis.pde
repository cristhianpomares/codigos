import processing.serial.*;
Serial port;
int end = 10; // the number 10 is ASCII for linefeed (end of serial.println), later we will look for this to break up individual messages
String serial;
PrintWriter output; //Para crear el archivo de texto donde guardar los datos}
String valor;
String valor2;
String valor3;
boolean status=false;
///////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
 println(Serial.list()); //Visualiza los puertos serie disponibles en la consola de abajo
 port = new Serial(this, Serial.list()[0], 9600); //Abre el puerto serie COM3
 port.clear();
 serial = port.readStringUntil(end);
 serial = null;
 size(800, 600); //Creamos una ventana de 800 píxeles de anchura por 600 píxeles de altura

 output = createWriter("Parametros_datos.txt");

}
/////////////////////////////////////////////////////////////////////////////////////////////

void draw()
{
stroke(0);
background(21,194,227);//Fondo de color blanco
fill(0,0,0);
textSize(10);
textSize(15);
text("Monitoreo de Parametros Fisico-Quimicos",300,10);
textSize(10);
text(hour( )+":",10,10);
text(minute( )+":",26,10);
text(second( ),41,10);
text(year( ),60,10);
output.print(valor + " ºC ");
 output.print(hour( )+":");
 output.print(minute( )+":");
 output.println(second( ));
 output.println("");
 output.print(valor2 + " uS ");
 output.print(hour( )+":");
 output.print(minute( )+":");
 output.println(second( ));
 output.println("");

output.print(valor3 + " PH ");
 output.print(hour( )+":");
 output.print(minute( )+":");
 output.println(second( ));
 output.println("");
//////////////////////////////////////////////////////////
textSize(15);
text("TEMPERATURA",45,90);
text("CONDUCTIVIDAD",350,85);
text("PH",700,85);
text("Control Vehiculo Movil",300,470);
ellipseMode(CENTER);
fill(46,245,17);
ellipse(100,200,150,150);
ellipse(400,200,180,180);
ellipse(700,200,150,150);
///////////////////////////////////////////////////////////////
while (port.available() > 0) { //as long as there is data coming from serial port, read it and store it
 serial = port.readStringUntil(end);
 }
 if (serial != null) { //if the string is not empty, print the following

 /* Note: the split function used below is not necessary if sending only a single
variable. However, it is useful for parsing (separating) messages when
 reading from multiple inputs in Arduino. Below is example code for an Arduino
sketch
 */

 String[] a = split(serial, ','); //a new array (called 'a') that stores values into separate cells (separated by commas specified in your Arduino program)
 println(a[0]); //print Value1 (in cell 1 of Array - remember that arrays are zeroindexed)
 println(a[1]); //print Value2 value
 println(a[2]);
valor=a[0];
valor2=a[1];
valor3=a[2];

 }
//////////////////////////////////////////////////////////a
/*fill(0);
textSize(20);
text(valor, 70, 200);
text("ºC",127,200);
text(valor2, 330, 200);
text("uS/cm",420,200);
text(valor3, 690, 200);*/


line(0, 450 , 800, 450);
//line(280, 450 , 450, 600);
rectMode(CENTER);
fill(155);
rect (120,480,40,40);
rect (80,520,40,40);
rect (160,520,40,40);
rect (120,560,40,40);
fill(155);
rect (230,520,40,40);
textSize(20);
fill(0);
text("P",222,525);
text("Stop",210,560);
fill(0);
textSize(20);
text("W",112,485);
text("A",75,525);
text("D",150,525);

text("S",115,565);
}
void keyPressed()
{
if(key=='a'||key=='A')
 {

 port.write("A");
 }

if(key=='d'||key=='D')
 {

 port.write("D");
 }

if(key=='w'||key=='W')
 {

 port.write("W");
 }

if(key=='s'||key=='S')
 {

 port.write("S");
 }

if(key=='p'||key=='P')
 {

 port.write("P");
 }

}
