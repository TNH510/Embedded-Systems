#define R D4
#define G D3
#define B D2
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer webServer(80);

//Set up Station Mode
char* ssid = "405";
char* pass = "1234567890";

const char MainPage[] PROGMEM = R"=====(
   <!DOCTYPE html>
   <html>
    <head>
       <title>HOME PAGE</title> 
       <style> 
          body {text-align:center;}
          h1 {color:#003399;}
          a {text-decoration: none;color:#FFFFFF;}
          .bt_on {height:50px; width:100px; margin:10px 0;background-color:#FF6600;border-radius:5px;}
          .bt_off {height:50px; width:100px; margin:10px 0;background-color:#00FF00;border-radius:5px;}
       </style>
       <meta charset="UTF-8">
    </head>

    <body>
      <h1>ESP8266 Web Server</h1> 
      <div>Trạng thái trên chân R: <b><pan id="trangthaiR"><pan></b></div>
      <div>
        <button class="bt_on" onclick="getdata('onR')">ON</button>
        <button class="bt_off" onclick="getdata('offR')">OFF</button>
      </div>
      <div>Trạng thái trên chân G: <b><pan id="trangthaiG"><pan></b></div>
      <div>
        <button class="bt_on" onclick="getdata('onG')">ON</button>
        <button class="bt_off" onclick="getdata('offG')">OFF</button>
      </div>
      <div id="reponsetext"></div>
      <script>
        //------Tao doi tuong request---------//
        function create_obj()
        {
          td = navigator.appName;
          if (td == "Microsoft Internet Explorer")
          { 
            obj = new ActiveXObject("Microsoft.XMLHTTP");  
          }  
          else
          {
            obj = new XMLHttpRequest();  
          }
          return obj;
        }
        var xhttp= create_obj();

       //-----------Thiết lập dữ liệu và gửi request-------
        function getdata(url){
          xhttp.open("GET","/"+url,true);
          xhttp.onreadystatechange = process;//nhận reponse 
          xhttp.send();
        }
        //-----------Kiểm tra response--------------------
        function process(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Updat data sử dụng javascript----------
            ketqua = xhttp.responseText; 
            document.getElementById("reponsetext").innerHTML=ketqua;       
          }
        }
        //------------Kiểm tra trạng thái chân D1 D2------
        function getstatusRG(){
          xhttp.open("GET","getstatusRG",true);
          xhttp.onreadystatechange = process_json;
          xhttp.send();
        }
        //-----------Kiểm tra response-------------------------------
        function process_json(){
          if(xhttp.readyState == 4 && xhttp.status == 200){
            //------Update data sử dụng javascript-------------------
            var trangthaiRG_JSON = xhttp.responseText;
            var Obj = JSON.parse(trangthaiRG_JSON);
            document.getElementById("trangthaiR").innerHTML = Obj.R;
            document.getElementById("trangthaiG").innerHTML = Obj.G;
          }
        }
        //---Ham update duu lieu tu dong---
        setInterval(function() {
          getstatusRG();
        }, 1000);
      </script>   
    </body>
   </html>
)====="; 

void setup()
{
  //Khai bao chan R,G la OUTPUT
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  
  //Thiet lap ket noi mang wifi
  WiFi.begin(ssid,pass);
  Serial.begin(9600);
  Serial.print("Connecting");
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println(WiFi.localIP());
  
  webServer.on("/", mainpage);
  webServer.on("/onR", onR);
  webServer.on("/offR", offR);
  webServer.on("/onG", onG);
  webServer.on("/offG", offG);
  webServer.on("/getstatusRG",get_statusRG);
  webServer.begin();        
}

void loop()
{
  webServer.handleClient();
}
// Chuong trinh phuc vu webserver
void mainpage()
{
  String s = FPSTR(MainPage);
  webServer.send(200, "text/html", s);  
}
void onR()
{
  digitalWrite(R, HIGH);
  webServer.send(200,"text/html","CHÂN R ĐÃ ON");
    
}
void offR()
{
  digitalWrite(R, LOW);
  webServer.send(200,"text/html","CHÂN R ĐÃ OFF");
}
void onG()
{
  digitalWrite(G, HIGH);
  webServer.send(200,"text/html","CHÂN G ĐÃ ON");
}
void offG()
{
  digitalWrite(G, LOW);
  webServer.send(200,"text/html","CHÂN G ĐÃ OFF");
}

void get_statusRG(){
  String r,g;
  if(digitalRead(R)==0){
    r = "OFF";
  }else{
    r = "ON";
  }
  if(digitalRead(G)==0){
    g = "OFF";
  }else{
    g = "ON";
  }
  String s = "{\"R\": \""+ r +"\",\"G\": \""+ g +"\"}";
  webServer.send(200,"application/json",s);
}
