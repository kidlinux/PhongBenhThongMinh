#include <DHT.h>

#define AmThanh A0
#define GiuongBenh 5
#define DHTPIN 6
#define DHTTYPE DHT11
#define NVS 2

#define QUATGIO 11
#define QUATHUT 12
#define DENBAO 10
#define DENVS 8

DHT dht=DHT(DHTPIN,DHTTYPE);

float nhietdo;
float doam;

volatile int interruptCount = 0; // Biến liên tục được ngắt
int CoNguoiTrongNhaVeSinh = 0; // Biến phụ lưu trữ trạng thái trước đó của biến liên tục

bool CoNguoiTrongNhaVS=false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("Khởi động hệ thống...");
  KhaiBaoChan();
  dht.begin();

  //Gắn sự kiện ngắt cho cửa nhà vệ sinh
  attachInterrupt(digitalPinToInterrupt(NVS), KiemTraNhaVeSinh, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  DocDHT();
  DocAmThanh();
  
  KiemTraBenhNhan();

  // Kiểm tra xem trạng thái của biến liên tục có thay đổi không
  if (interruptCount != CoNguoiTrongNhaVeSinh) {
    Serial.print("Biến liên tục đã thay đổi thành: ");
    Serial.println(interruptCount);
    CoNguoiTrongNhaVeSinh = interruptCount; // Cập nhật trạng thái trước đó
    digitalWrite(DENVS,CoNguoiTrongNhaVeSinh);
  }
}

void DocDHT()
{
  //int chk = dht.read(DHTPIN); // Đọc dữ liệu từ cảm biến DHT11
  nhietdo=dht.readTemperature();
  doam=dht.readHumidity();
  Serial.print("Nhiệt độ (C): ");
  Serial.println(nhietdo);
  Serial.print("Độ ẩm (%): ");
  Serial.println(doam);
  
  delay(300);
}

void KiemTraBenhNhan()
{
  int benhnhan=digitalRead(GiuongBenh);

  // Kiểm tra nếu có bệnh nhân
  if(benhnhan==0 && nhietdo>31)
  {
    BatQuatGio();
  }else
  {
    TatQuatGio();
  }

  // Nếu có bệnh nhân và độ ẩm cao 
  if(benhnhan==0 && doam>70)
  {
    BatQuatHut();
  }else
  {
    TatQuatHut();
  }

  Serial.print("Bệnh nhân: ");
  Serial.println(benhnhan);
  delay(300);
}

void DocAmThanh()
{
  int amthanh=analogRead(AmThanh);
  Serial.print("Âm thanh: ");
  Serial.println(amthanh);

  //Cường độ âm thanh càng lớn thì giá trị trả về càng nhỏ
  if (amthanh<25){
    BatDenBao();
  }else{
    TatDenBao();
  }
}

void KiemTraNhaVeSinh()
{
  // Cập nhật biến liên tục
  interruptCount=digitalRead(NVS);
}

void BatQuatGio(){
  digitalWrite(QUATGIO,LOW);
}


void TatQuatGio(){
  digitalWrite(QUATGIO,HIGH);
}

void BatQuatHut(){
  digitalWrite(QUATHUT,LOW);
}


void TatQuatHut(){
  digitalWrite(QUATHUT,HIGH);
}

void BatDenBao(){
  digitalWrite(DENBAO,HIGH);
  delay(200);
  digitalWrite(DENBAO,LOW);
  delay(200);
  digitalWrite(DENBAO,HIGH);
  delay(200);
  digitalWrite(DENBAO,LOW);
  delay(200);
  digitalWrite(DENBAO,HIGH);
  delay(200);
  digitalWrite(DENBAO,LOW);
  delay(200);
}


void TatDenBao(){
  digitalWrite(DENBAO,LOW);
}

void KhaiBaoChan(){
  pinMode(AmThanh,INPUT);
  pinMode(GiuongBenh,INPUT);
  pinMode(DHTPIN,INPUT);
  pinMode(NVS,INPUT);

  pinMode(QUATGIO,OUTPUT);
  pinMode(QUATHUT,OUTPUT);
  pinMode(DENBAO,OUTPUT);
  pinMode(DENVS,OUTPUT);

  //Tắt quạt
  digitalWrite(QUATGIO, HIGH);
  digitalWrite(QUATHUT, HIGH);
}