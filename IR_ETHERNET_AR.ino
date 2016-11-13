/*  Arduino + Ethernet Shield + TV
    Versão: 1.0
    Feito por Guilherme Lirio Tomasi de Oliveira
    Retirado de: http://arduinodemcu.blogspot.com
*/

#include <SPI.h>
#include <Ethernet.h>
#include <IRremote.h>

IRsend irsend;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x9B, 0x36 }; //MAC ADRESS UNIVERSAL PARA PLACAS GENÉRICAS
byte ip[] = { 192, 168, 1, 99 }; // ENDEREÇO DE IP - DEFINIDO PELO USUÁRIO
byte gateway[] = { 192, 168, 1, 1 }; // ENDEREÇO GATEWAY DO ROTEADOR
byte subnet[] = { 255, 255, 255, 0 }; //SUBMASCARA DO ROTEADOR
EthernetServer server(80); //PORTA QUE SERÁ ABERTA

String readString;

//////////////////////

void setup() {

  Ethernet.begin(mac, ip, gateway, subnet); //COMEÇAR O ETHERNET SHIELD, COM OS DADOS ACIMA
  server.begin(); //COMECAR O WEBSERVER
  Serial.begin(9600);  //HABILITAR O SERIAL
  //Serial.println("Arduino + Ethernet Shield + TV"); //IMPRIMIR NO SERIAL
}

void loop() {
  // CRIAR A CONEXÃO COM O CLIENTE
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {

          Serial.println(readString); //Imprime no Serial para acompanhamento
          if (readString.indexOf("?ligaar") > 0) //checks for on
          {
            ligaar();
            Serial.print("Ligou Ar");
          }
          
          //clearing string for next read
          readString = "";


          /////////////// PAGINA HTML //////////////////////////

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<html>");
          client.println("<title>Arduino + Ethernet Shield + AR</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>");
          client.println("<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'>");
          client.println("<body>");
          client.println("<h2>Arduino + Ethernet Shield + AR</h2>");
          client.println("<hr>");
          client.println("<a href='?ligaar'>LIGA AR</a>");
          client.println("</body>");
          client.println("</html>");

          delay(1);
          //stopping client
          client.stop();
        }
      }
    }
  }
}

void ligaar() {
  int khz = 38; // 38kHz carrier frequency for the NEC protocol
  unsigned int irSignal[] = {4776, 2380, 628, 572, 548, 656, 548, 652, 548, 1856, 548, 652, 548, 656, 620, 556, 572, 652, 548, 656, 548, 652, 548, 656, 624, 576, 548, 1800, 680, 576, 624, 516, 688, 576, 548, 1856, 624, 1776, 624, 1684, 720, 580, 548, 652, 548, 632, 644, 580, 620, 512, 692, 576, 624, 1780, 548, 656, 544, 636, 568, 652, 548, 656, 548, 652, 548, 652, 548, 580, 624, 652, 548, 656, 544, 656, 548, 652, 548, 656, 548, 1852, 548, 656, 548, 656, 544, 656, 548, 652, 548, 1856, 548, 652, 548, 652, 552, 652, 548, 576, 624, 1856, 548, 652, 548, 656, 548, 628, 572, 656, 548, 616, 584, 652, 548, 656, 548, 1852, 552, 1852, 548, 656, 548, 652, 548, 1856, 548, 652, 548, 628, 572, 588, 616, 652, 548, 1856, 548, 652, 548, 624, 580, 652, 548, 652, 548, 656, 548, 628, 572, 656, 548, 600, 600, 652, 548, 1856, 548, 652, 548, 656, 548, 1856, 548, 652, 548, 628, 572, 656, 548, 652, 548, 580, 624, 652, 548, 652, 548, 656, 548, 652, 548, 652, 620, 584, 548, 652, 548, 652, 548, 656, 548, 652, 548, 652, 552, 652, 548, 652, 624, 576, 552, 652, 548, 632, 568, 656, 548, 652, 548, 632, 644, 580, 620, 580, 624, 580, 548, 652, 548, 652, 628, 552, 572, 656, 620, 580, 620, 572, 628, 584, 544, 656, 624, 1780, 620, 1780, 548, 656, 548, 1852, 548, 1856, 548, 1856, 548, 652, 548, 1856, 548, 1832, 572, 1776, 628, 652, 620, 1784, 548, 652, 624, 1780, 620, 556, 648, 1760, 568, 1856, 620, 1784, 548, 1852, 624, 556, 644, 1780, 628, 1780, 620}; //AnalysIR Batch Export (IRremote) - RAW
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.
}


