#include <SoftwareSerial.h>
const byte Rx = 3;
const byte Tx = 2;
SoftwareSerial uno = SoftwareSerial(Rx, Tx);
#include <TimerOne.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define nut 4
#define RST_PIN         9
#define SS_PIN          10
#define TIMER_US 500000
unsigned long UID[4];
unsigned long i;
int diachi = 1; int diachi2 = 0; int a;
int gtmas = 0; int tong = 0; int gttong = 0;
int bandau;
int menu = 7; int gtmenu = 0;
int len = 5; int gtlen = 0;
int xuong = 6; int gtxuong = 0;
int macdinh = 1;
int dem_tong = 0; int dem_menu = 0; int dem = 0;
int relay = 8;

int nuthientai;
int nutmacdinh = 1;
int nutdem;
int trangthailed = 0;
const int coi = A0;
const int dendung = A1;
const int densai = A2;
unsigned long timer, last;

String DataSend = "";
void Json(String khoa);
void button (void);
void control_led(void);
//void ngat (void);
void yes(void);
void no(void);
MFRC522 mfrc522(SS_PIN, RST_PIN);

int id_moi[4]; int id_er[4];

int diachi_zero;
int m = 5; int moi = 0;
int gt_zero;
int gt_er = 0;
int n = 4; int o = 0;
int demco = 0; int demchua = 0; int demmas = 0;
String val;
void manhinh()
{
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=>  KHOA CUA  <=");
  lcd.setCursor(0, 1);
  lcd.print("=> MOI NHAP THE");
}
void nhap_mas()
{
  lcd.setCursor(2, 0);
  lcd.print("SETUP MASTER");
  lcd.setCursor(0, 1);
  lcd.print("=> MOI NHAP THE");
}

void menu_tong()
{
  if (dem_tong == 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">BACK");
    lcd.setCursor(0, 1);
    lcd.print(" THEM THE");
  }
  else if (dem_tong == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" BACK");
    lcd.setCursor(0, 1);
    lcd.print(">THEM THE");
  }
  else if (dem_tong == 2)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" THEM THE");
    lcd.setCursor(0, 1);
    lcd.print(">XOA THE BAT KY");
  }
  else if (dem_tong == 3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" XOA THE BAT KY");
    lcd.setCursor(0, 1);
    lcd.print(">XOA TAT CA THE");
  }
}

void chon_menu()
{
  if (dem_tong == 1) //Nhập thẻ mới
  {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("THEM THE MOI");
    lcd.setCursor(0, 1);
    lcd.print("=> MOI NHAP THE");
  }
  else if (dem_tong == 2) //Xóa thẻ bất kỳ
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" XOA THE BAT KY ");
    lcd.setCursor(0, 1);
    lcd.print("=>MOI NHAP THE");
  }
  else if (dem_tong == 3) //XÓA TẤT CẢ THẺ
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("XOA TAT CA THE");
  }
}

void tim_zero()
{
  while ( n < a) //Tìm ô nhớ có GT 0 đầu tiên
  {
    gt_zero = EEPROM.read(n);
    if (gt_zero == 0)
    {
      diachi_zero = n;
      break;
    }
    n++;
  }

  if (diachi_zero == 0)
  {
    diachi_zero = a; //Nếu trong đoạn từ 4 đến số ô đã sử dụng không có ô nào có GT 0
    // thì diachi_zero = a là ô cuối cùng lưu thẻ
    n = 0;
  }
}

void ss_epprom()
{

  while (m < a) //chạy từ ô 5 đến ô sử dụng cuối ... 5 < 20
  {
    moi = m + 4; //moi = 9
    for (m; m < moi; m++)
    {
      gt_er = EEPROM.read(m);

      if (o <= 3) //Lưu giá trị đọc từ eeprom vào id_er
      {
        id_er[o] = gt_er;
      }
      o++;
    }
    if (id_moi[0] == id_er[0] && id_moi[1] == id_er[1] && id_moi[2] == id_er[2] && id_moi[3] == id_er[3]) //Nếu thẻ có trong EEPR
    {
      demco = 1; demchua = 0; demmas = 0;
      break;
    }
    else if (id_moi[0] != id_er[0] or id_moi[1] != id_er[1] or id_moi[2] != id_er[2] or id_moi[3] != id_er[3]) //Nếu thẻ KO có trong EEPR
    { // Dùng or vì chỉ cần  1 byte khác với 1 byte lưu trong eeprom thì thẻ đó là thẻ khác
      // Nếu dùng && có thể có thẻ sẽ trùng 1 đến 2 byte
      demchua += 1; demco = 0; demmas = 0;
    }
    if (id_moi[0] == EEPROM.read(1) && id_moi[1] == EEPROM.read(2) &&
        id_moi[2] == EEPROM.read(3) && id_moi[3] == EEPROM.read(4)) //THẺ mới nhập là MASTER
    {
      demmas = 1; demco = 0; demchua = 0;
    }
    o = 0;
    m = moi;
  }
}

void in()
{
  for (int g = 0; g < 30; g++)
  {
    Serial.print(g); Serial.print("= "); Serial.print(EEPROM.read(g)); Serial.print(" ");
  }
  Serial.println(" ");
}

void setup()
{
  Serial.begin(9600);
  uno.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();

  pinMode(menu, INPUT_PULLUP);
  pinMode(len, INPUT_PULLUP);
  pinMode(xuong, INPUT_PULLUP);
  pinMode(nut, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  pinMode (A0, OUTPUT);
  pinMode (A1, OUTPUT);
  pinMode (A2, OUTPUT);
  //in();
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
  Timer1.initialize(TIMER_US);
  Timer1.attachInterrupt(ngat);
}

void loop()
{
  //ngat();

  bandau = true;
  gtmenu = digitalRead(menu);
  gtlen = digitalRead(len);
  gtxuong = digitalRead(xuong);
  button();
  if (bandau == true) //Kiểm tra có thẻ MAS chưa
  {
    for (int j = 1; j < 4; j++) // đọc từ 1 - 4 xem trong ô nhớ có gì ko, Nếu không là chưa có thẻ MAS
    {
      gtmas = EEPROM.read(j);
      gttong = gttong + gtmas;
      //Serial.print("Gtmas: "); Serial.print(gtmas); Serial.print("  ");
      //Serial.print("Tổng: "); Serial.print(tong); Serial.print("  ");
    }
  }

  if (gttong == 0) //Chưa có thẻ MAS
  {
    tong = 0;
    if (tong == 0 && dem == 0)//Nếu ô 1, 2, 3, 4 chưa có Mas thì nhập Mas
    {
      nhap_mas();

      if ( ! mfrc522.PICC_IsNewCardPresent())
      {
        return;
      }
      if ( ! mfrc522.PICC_ReadCardSerial())
      {
        return;
      }
      for (byte i = 0; i < 4; i++)
      {
        // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " " :" ");
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        UID[i] = mfrc522.uid.uidByte[i];

        EEPROM.write(diachi, UID[i]); //bắt đầu lưu từ ô 1-4
        diachi = diachi + 1;
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      yes();
      analogWrite(dendung, 0);
      delay(50);
      lcd.clear();
    }
  }
  else // Đã có thẻ MAS
  {
    tong = 1; bandau = false;
    if (tong == 1 && dem == 0) // Đã có MASTER
    {
      manhinh();
      if ( ! mfrc522.PICC_IsNewCardPresent())
      {
        return;
      }
      if ( ! mfrc522.PICC_ReadCardSerial())
      {
        return;
      }
      for (byte i = 0; i < 4; i++)
      {
        UID[i] = mfrc522.uid.uidByte[i];
        id_moi[i] = UID[i];
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      if (id_moi[0] == EEPROM.read(1) && id_moi[1] == EEPROM.read(2)
          && id_moi[2] == EEPROM.read(3) && id_moi[3] == EEPROM.read(4)) //Check xem phải thẻ MAS ko
      {
        dem = 1;
        if (tong == 1 && dem == 1) //Để thẻ MASTER để vào vào menu
        {
          menu_tong();
        }
      }
      else //Nếu không phải thẻ MASTER
      {
        ss_epprom(); //So sánh thẻ mới với EEPROM
        if (demco == 1 && demchua == 0 && demmas == 0) //Nếu thẻ có trong EEPROM
        {
          // Serial.print("................THE DA CO TRONG EEPROM..................");
          // Serial.print(" Zero: "); Serial.print(diachi_zero);
          // Serial.print("   Demco: "); Serial.println(demco);
          trangthailed = 1;
          digitalWrite(relay, HIGH);
          o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
          yes();
          lcd.setCursor(0, 1);
          lcd.print("=>   MO CUA   <=");
          delay(5000);
          analogWrite(dendung, 0);
          digitalWrite(relay, LOW);
          trangthailed = 0;
          lcd.clear();
        }
        else if (demchua > 0 && demco == 0 && demmas == 0) //Nếu thẻ chưa có trong EEPROM
        {
          //   Serial.print("................THE CHUA CO..................");
          //   Serial.print(" Zero: "); Serial.print(diachi_zero);
          //   Serial.print("   Demchua: "); Serial.print(demchua);
          // Serial.print("  ID mới: ");Serial.print(id_moi[0]);Serial.print(":");Serial.print(id_moi[1]);Serial.print(":");
          // Serial.print(id_moi[2]);Serial.print(":");Serial.println(id_moi[3]);
          digitalWrite(relay, LOW);
          o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
          lcd.setCursor(0, 1);
          lcd.print("    SAI THE!    ");
          no();
          delay(1000);
          lcd.clear();
        }
      }
    }
  }

  if (dem_menu == 1 && dem_tong == 1 && dem == 1) //Nếu chọn THÊM THẺ MỚI
  {
    a = EEPROM.read(0); //đọc ô nhớ 0 xem đã sử dụng bao nhiêu ô nhớ
    //Serial.print("a: "); Serial.println(a);

    if (a == 0) //Nếu chưa có thẻ PHỤ nào
    {
      int diachi_phu = 5; // 5,6,7,8
      if ( ! mfrc522.PICC_IsNewCardPresent())
      {
        return;
      }
      if ( ! mfrc522.PICC_ReadCardSerial())
      {
        return;
      }
      for (byte i = 0; i < 4; i++)  //Quét thẻ mới
      {
        UID[i] = mfrc522.uid.uidByte[i];
        id_moi[i] = UID[i];
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      if (id_moi[0] == EEPROM.read(1) && id_moi[1] == EEPROM.read(2) &&
          id_moi[2] == EEPROM.read(3) && id_moi[3] == EEPROM.read(4)) //THẺ mới nhập là MASTER
      {
        lcd.clear();
        menu_tong();
        dem_menu = 0;
      }

      else //THẺ MỚI KHÔNG PHẢI LÀ THẺ MASTER thi LƯU vào ô 5,6,7,8
      {
        for (byte i = 0; i < 4; i++)
        {
          EEPROM.write(diachi_phu, id_moi[i]);
          diachi_phu = diachi_phu + 1;
          a = diachi_phu;
        }
        EEPROM.write(0, a); //Sau khi lưu 1 thẻ mới vào thì cập nhật số ô nhớ đã sử dụng vào ô 0
        yes();

        lcd.setCursor(0, 1);
        lcd.print("=> DANG LUU...  ");
        delay(1000);
        analogWrite(dendung, 0);
        lcd.clear();
        chon_menu();
      }
    }

    else if ( a != 0) // Đã có 1 or nhiều thẻ phụ
    {
      if ( ! mfrc522.PICC_IsNewCardPresent())
      {
        return;
      }
      if ( ! mfrc522.PICC_ReadCardSerial())
      {
        return;
      }
      for (byte i = 0; i < 4; i++)  //Quét thẻ mới
      {
        UID[i] = mfrc522.uid.uidByte[i];
        id_moi[i] = UID[i];
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();

      ss_epprom(); //So sánh thẻ mới đưa vào với Eeprom

      if (demco == 1 && demchua == 0 && demmas == 0) //Nếu thẻ đã có
      {
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
        lcd.setCursor(0, 1);
        lcd.print("   THE DA CO!   ");
        delay(1000);
        lcd.clear();
        chon_menu();
        //in();
      }
      else if (demchua > 0 && demco == 0 && demmas == 0) //Nếu thẻ chưa có...THÌ LƯU THẺ ĐÓ VÀO EEPROM
      {
        tim_zero(); //Tìm vị trí 0 đầu tiên
        if (diachi_zero == a) //Nếu trong đoạn từ 4 đến số ô đã sử dụng không có ô nào có GT 0
          // a là ô cuối cùng lưu thẻ
        {
          for (int i = 0; i < 4; i++)
          {
            EEPROM.write(diachi_zero, id_moi[i]);
            diachi_zero = diachi_zero + 1;
            a = diachi_zero;
          }
          EEPROM.write(0, a); //Sau khi lưu 1 thẻ mới vào thì cập nhật số ô nhớ đã sử dụng vào ô 0
        }

        else if (diachi_zero == n) ////Nếu trong đoạn từ 4 đến số ô đã sử dụng CÓ ô = 0, thì gán ô đó vào n
        {
          for (int i = 0; i < 4; i++) //Lưu thẻ mới vào bắt đầu từ ô 0 đó
          {
            EEPROM.write(diachi_zero, id_moi[i]);
            diachi_zero = diachi_zero + 1;
            //a = diachi_zero;
          }
          diachi_zero = a;
        }
        tim_zero();
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
        yes();
        lcd.setCursor(0, 1);
        lcd.print("=> DANG LUU...  ");
        delay(1000);
        analogWrite(dendung, 0);
        lcd.clear();
        chon_menu();
        //in();
      }
      if (demmas == 1 && demco == 0 && demchua == 0) //THẺ MASTER thì quay về menu
      {
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
        lcd.clear();
        menu_tong();
        dem_menu = 0;
        //in();
      }
    }
  }

  else if (dem_menu == 1 && dem_tong == 2 && dem == 1) //Nếu chọn XÓA THẺ BẤT KỲ
  {
    a = EEPROM.read(0); //đọc ô nhớ 0 xem đã sử dụng bao nhiêu ô nhớ
    //Serial.print("a: "); Serial.println(a);

    if (a == 0) //Nếu chưa có thẻ PHỤ nào
    {
      lcd.setCursor(0, 1);
      lcd.print(" CHUA CO THE... ");
      delay(1000);
      lcd.clear();
      menu_tong();
      dem_menu = 0;
    }
    else //Nếu đã có 1 hoặc nhiều thẻ PHỤ
    {
      if ( ! mfrc522.PICC_IsNewCardPresent())
      {
        return;
      }
      if ( ! mfrc522.PICC_ReadCardSerial())
      {
        return;
      }
      for (byte i = 0; i < 4; i++)  //Quét thẻ mới
      {
        UID[i] = mfrc522.uid.uidByte[i];
        id_moi[i] = UID[i];
      }
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      ss_epprom();
      if (demco == 1 && demchua == 0 && demmas == 0) //thẻ có trong EEPROM
      {
        tim_zero();

        m = m - 4;
        for (int i = 0; i < 4; i++)
        {
          EEPROM.write(m, 0);
          m = m + 1;
        }
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0; n = 4;
        lcd.setCursor(0, 1);
        lcd.print(" DA XOA THE.... ");
        delay(1000);
        lcd.clear();
        chon_menu();
        tim_zero();
        //in();
      }
      else if (demchua > 0 && demco == 0 && demmas == 0) //Thẻ chưa có trong EEPROM
      {
  
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
        lcd.setCursor(0, 1);
        lcd.print(" THE CHUA CO... ");
        delay(1000);
        lcd.clear();
        chon_menu();
        //in();
      }
      if (demmas == 1 && demco == 0 && demchua == 0) //Là thẻ MASTER
      {
      
        o = 0; m = 5; moi = 0; demco = 0; demchua = 0; demmas = 0;
        lcd.clear();
        menu_tong();
        dem_menu = 0;
     
      }
    }
  }

  else if (dem_menu == 1 && dem_tong == 3 && dem == 1) //Chọn XÓA TẤT cả thẻ
  {
    a = EEPROM.read(0); //đọc ô nhớ 0 xem đã sử dụng bao nhiêu ô nhớ
    //Serial.print("a: "); Serial.println(a);
    if (a == 0) //Nếu chưa có thẻ PHỤ nào
    {
      lcd.setCursor(0, 1);
      lcd.print(" CHUA CO THE... ");
      dem_menu = 0;
      delay(2000);
      lcd.clear();
      menu_tong();
    }
    else if (a > 0) //Nếu đã có 1 hoặc nhiều thẻ PHỤ
    {
      lcd.setCursor(0, 1);
      lcd.print("  DANG XOA....  ");
      for (int diachi = 5; diachi < a; diachi++) //Bắt đầu từ ô 5 đến ô đang sử dụng
      {
        EEPROM.write(diachi, 0);
      }
      EEPROM.write(0, 0); //Sau đó cập nhật vào ô 0 là 0 vì đã xóa hết thẻ
      delay(3000);
      lcd.clear();
      menu_tong();
      dem_menu = 0;
    }
    diachi_zero = 0; n = 4; a = 0;
    tim_zero();
    //in();
  }

  if (gtlen != macdinh) //Nút lên
  {
    if (gtlen == 0)
    {
      if (dem == 1) //Lên ở menu
      {
        if (dem_tong <= 0)
          dem_tong = 3;
        else
          dem_tong --;
        menu_tong();
      }
      delay(100);
    }
    macdinh = gtlen;
  }

  if (gtxuong != macdinh) //Nút xuống
  {
    if (gtxuong == 0)
    {
      if (dem == 1) //Xuống ở menu
      {
        if (dem_tong >= 3)
          dem_tong = 0;
        else
          dem_tong ++;
        menu_tong();
      }
      delay(100);
    }
    macdinh = gtxuong;
  }

  if (gtmenu != macdinh) //Nút menu
  {
    if (gtmenu == 0)
    {
      if (dem == 1 && dem_tong != 0) //Chọn menu
      {
        chon_menu();
        dem_menu = 1;
      }
      else if (dem == 1 && dem_tong == 0) //Nhấn nút BACK để về màn hình
      {
        lcd.clear();
        manhinh();
        dem = 0;
      }
      delay(100);
    }
    macdinh = gtmenu;
  }

}
void Json(String khoa)
{
  if (millis() - last >= 500)
  {
    // Serial.print("uno gửi:");
    DataSend = "";
    DataSend = String(trangthailed);
    Serial.println(DataSend);
    uno.println(DataSend);
    last = millis();
  }
}
void ngat()
{
  //    digitalWrite(led, digitalRead(led)^1);
  if (uno.available() > 0)
  {
    char val = uno.read();
    Serial.print("ESP:");
    Serial.println(val);
    if (val == '1')
    {
      trangthailed = 1;
      digitalWrite(relay, HIGH);
      analogWrite(dendung, 255);
      Serial.println("LED on");
      last = millis();
    }
    else if (val == '0')
    {
      trangthailed = 0;
      digitalWrite(relay, LOW);
      analogWrite(dendung, 0);
      Serial.println("LED off");
    }
  }
  Json(String(trangthailed));
}
void button (void)
{
  digitalRead(nut);
  if (digitalRead(nut) == LOW)
  {
    delay(20);
    while (1)
    {
      if (digitalRead(nut) == HIGH)
      {
        control_led();
        delay(30);
        break;
      }  
    }
  }
}
void control_led(void)
{
  if (trangthailed == 0)
  {
    digitalWrite(relay, HIGH);
    analogWrite(dendung, 255);
    trangthailed = 1;
  }
  else if (trangthailed == 1)
  {
    digitalWrite(relay, LOW);
    analogWrite(dendung, 0);
    trangthailed = 0;
  }
}
void yes(void)
{
  analogWrite(dendung, 255);
  analogWrite(coi, 255);
  delay(200);
  analogWrite(coi, 0);
  delay(200);
  analogWrite(coi, 255);
  delay(200);
  analogWrite(coi, 0);

}
void no(void)
{
  analogWrite(densai, 255);
  analogWrite(coi, 255);
  delay(2000);
  analogWrite(coi, 0);
  delay(2000);
  analogWrite(densai, 0);
}
