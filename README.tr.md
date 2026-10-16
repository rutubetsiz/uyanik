# UYANIK

[English](README.md)

Uzaktaki bir bilgisayarı açıp kapatmak için yapılmış bir cihaz. İnternet erişiminiz varsa, dünyanın neresinde olursanız olun fark etmez.

## Niye böyle bir şey yaptım?

Bir yaz projesi olarak başladı. Boşta duran bilgisayarımı sunucu yapmıştım ama sürekli açık bırakmak istemiyordum. Üniversite de başlıyordu, uzaktan kapatıp açabilmem lazımdı. Uzaktaki bir makineyi kapatmak kolay ama açmak zor. Wake-on-LAN yetmiyor, Bir de CGNAT varsa... Hepsine çare olarak evde sürekli açık bir cihaz gerekiyordu, yaptım... UYANIK, nerede olursam olayım web paneline girip sanki evde kasanın düğmesine fiziksel olarak basıyormuşum gibi sunucumu açıp kapatmamı sağlıyor.

## Nasıl çalışıyor

Cihaz anakartın ön panel header'ına (JFP1) bağlanıyor. Komut geldiğinde optokuplör çalışıyor, anakart bunu düğmeye basılmış gibi görüyor. Aynı header'daki güç ledi hattını okuyarak da bilgisayarın açık mı kapalı mı olduğunu anlıyor.

Header'ı cihaz kullandığı için kasanın ön paneli devre dışı kalıyor; kasanın güç düğmesini, reset düğmesini ve ledini bağlayacak yer kalmıyor. Bu yüzden cihazın üstünde kendi güç ve reset düğmeleri var, bir de bilgisayarın açık olup olmadığını gösteren bir led. Yani kasa düğmesini kaybetmiyorsun, yeri değişiyor, artık cihazın üstünde.

Komutlar Firebase Realtime Database üzerinden geçiyor. Panelden bir komut gönderdiğinde veritabanındaki alana yazılıyor, cihaz o alanı yokluyor ve gördüğünde uyguluyor. Durum bilgisi de ters yönde aynı yoldan geliyor.

Firebase kullanılmasının sebebi; cihaz ve telefon farklı ağlarda ve genelde CGNAT arkasında. Doğrudan bağlantı kurulamıyor, bir buluşma noktası gerekiyor. Port yönlendirme, DDNS ve statik IP gerekmiyor.

Uzak panelin yanında bir de yerel panel var; o cihazın kendi flash'ında duruyor ve internet olmadan da çalışıyor. Aynı ağdaysan doğrudan cihaza bağlanıp komut gönderebilirsin. Cihaz hiçbir ağa bağlanamadığında ise kendi hotspot'unu açıyor ve aynı panel kurulum ekranı olarak karşına çıkıyor.

Cihaz bilgisayar kapalıyken de çalışmak zorunda, sonuçta kapalı makineyi o açacak. Ben beslemeyi anakartın USB header’ından, JUSB2'den alıyorum. Bilgisayar kapalıyken o header’a gelen güç aslında güç kaynağının standby hattı, fiş prizde olduğu sürece hep canlı. Anakart onu header’a varsayılan olarak vermiyor, o yüzden BIOS’ta iki ayarı değiştirmek gerekiyor. Ayrıntısı aşağıda Besleme kısmında.

## Donanım

### Pinler

GPIO25, güç düğmesinin optokuplörünü; GPIO26, reset düğmesinin. GPIO27 ise güç ledinin optokuplöründen gelen sinyali okuyor.

GPIO4 fabrika sıfırlama için ayrıldı. Kartta buton yok, sıfırlamak için bu pini GND'ye değdirip beş saniye kesintisiz tutun.

GPIO32, versiyon 2'de eklendi. Bilgisayarın açık olduğunu göstermek için LED çıkışı veriyor.

### Malzemeler

- ESP32-WROOM-32D
- 3x PC817 DIP-4 optokuplör
- 3x 470 ohm direnç
- 2x 3.5x6mm tact switch
- LED
- Sürgülü On/Off Switch 3 Pin

JFP1'e bağlanmak için bulabilirseniz dişi uçlu ön panel kabloları yeterli. Veya dişi-dişi jumper'ları modifiye edip kullanabilirsiniz.

### Neden Optokuplör?

Elimden geldiğince tek bir anakart için değil, farklı üreticilerin anakartlarında da çalışması için uğraştım. Optokuplör zorunlu değildi; butonlar transistörle, Power LED de dirençlerle doğrudan okunabilirdi. Optokuplörü seçmemin sebebi, anakartın iç devresi hakkında varsayım yapmayı gerektirmemesi. Butonların pull-up voltajı ya da LED'in hangi taraftan sürüldüğü karttan karta değişebiliyor, optokuplör ise bunların hepsinde aynı şekilde çalışıyor. Maliyet farkı da çok az olduğu için kartlar arasındaki farklardan etkilenmeyen bu çözümü seçtim.

### Besleme

Cihazın bilgisayar kapalıyken de ayakta olması gerekiyor. Farklı yöntemler uygulanabilir.

Bir tanesi; Anakarttan, JUSB portları üzerinden. Ben böyle yaptım. Ama bir şartı var, anakart varsayılan ayarlarla kapalıyken USB header'lara standby gücü vermiyor. BIOS ayarı değiştirmek gerekiyor. Birincisi EuP 2013 ya da ErP Ready (kartına göre adı değişiyor), MSI'da Advanced altındaki Power Management Setup menüsünde. Bunu Disabled yapıyorsun. İkincisi Resume By USB Device, yine Advanced altında ama Wake Up Event Setup menüsünde. Bunu Enabled yapıyorsun. Header'ın VCC ve GND pinleri arasını ölç, 5 V civarı görmelisin. Göremiyorsan diğer header'ı dene, bazı anakartlar standby'ı sadece belirli portlara veriyor. Dezavantajı, Elektrik gittiğinde veya anakarttan güç kesildiğinde bilgisayar el ile çalıştırılmadan besleme alınamıyor.

Bir diğeri ise doğrudan güç kaynağından, 24 pinli konnektörün 9. pininden. Mor kablo. BIOS'tan tamamen bağımsız çünkü gücü anakart dağıtmadan önceki noktadan alıyorsun; hiçbir ayar onu kapatamaz. Karşılığında araya bir breakout kartı ya da hırsız klemens gibi bir şey kullanmanız gerekiyor. BIOS'u kurcalamak istemiyorsan ya da anakartın standby'ı header'lara hiç vermiyorsa bu yol kesin çalışır.

Bir başka yöntem de direkt bir pil ile veya adaptör ile cihazı beslemek, mevcut durumda buna uygun değil modifikasyon yapmanız gerekir.

### PCB

hardware/ içinde KiCad projesi ve PDF çıktıları var.

Ben PCB'yi kendim bastım. Kendim bastığım için evde baskı devreye uygun şekilde hazırladım. Kendiniz basabilirsiniz ya da KiCad dosyasını açıp kendine göre değiştirip bir baskıya da gönderebilirsiniz. Hiçbiri zorunlu değil, devre breadboard'da da çalışır.

### Kutu

case/ içinde model var.

3D baskı alabilirsiniz. Bütün parçalar bir dosya içinde, ayırmanız gerekebilir.

## Kurulum

### 1. Firmware

Arduino IDE ile derleniyor. Klasör adının .ino dosyasıyla aynı olması zorunlu, o yüzden software/UYANIK/ klasörünü olduğu gibi aç.

panel.html dosyasını düzenlersen cihazın içine gömülen kopyayı yeniden üretmen gerekiyor:

```bash
python build_html.py
```

İkisinin aynı olup olmadığını kontrol etmek için:

```bash
python build_html.py --kontrol
```

### 2. İlk açılış

Cihaz ilk açıldığında seri porta üç değer basıyor: hotspot ağının adı, panel parolası ve OTA parolası.

```
SSID : UYANIK-XXXX
Password : ...
OTA : ...
```

Bunları bir yere kaydet. Fabrika sıfırlamada değişmiyorlar. Panel parolasını sonradan değiştirirsen buradaki değer geçersiz oluyor, ama fabrika sıfırlama onu geri getiriyor.

### 3. Wi-Fi

Kayıtlı ağ yoksa cihaz kendi hotspot'unu açıyor. Telefonla bağlandığında uyanik.local ile panele girin, açılmazsa 192.168.4.1 adresini kullanın.

Ağa bağlandıktan sonra cihaza uyanik.local adresinden ya da panelde yazan yerel IP'den ulaşıyorsun. Hotspot 25 saniye daha açık kalıyor, yeni adresi görebilesin diye.

### 4. Firebase

Her kullanıcı kendi Firebase projesini açıyor.

Firebase konsolunda yeni bir proje aç ve içinde bir Realtime Database oluştur. Authentication bölümünden Email/Password sağlayıcısını aç ve cihaz için bir hesap oluştur.

Son olarak veritabanı kurallarını bu hesabın UID'sine bağla:

```json
{
  "rules": {
      ".read":  "auth != null && auth.uid === 'UID'",
      ".write": "auth != null && auth.uid === 'UID'"
  }
}
```

Bunlar bittiğinde cihazın panelindeki Ayarlar sekmesine şunları giriyorsun: Realtime Database adresi, Web API Key, oluşturduğun hesabın e-postası ve parolası, bir de cihaz adı.

### 5. Uzak panel

software/web/public/index.html tek dosyalık statik bir sayfa, iki şekilde kullanılabiliyor.

Firebase Hosting'e atacaksan software/web/ klasöründe:

```bash
firebase use --add
```

```bash
firebase deploy --only hosting
```

Statik bir dosya olduğu için kendi sunucunda da barındırabilirsin.

Doğrudan file:// ile de kullanabilirsiniz.

Panel ilk açıldığında veritabanı adresini, Web API Key'i ve cihaz adını soruyor. Bunlar yalnızca tarayıcıda saklanıyor, hiçbir yere gönderilmiyor.

## Fabrika sıfırlama

GPIO 4'ü GND'ye değdir ve beş saniye kesintisiz tut. Tek bir kopma sayacı sıfırlıyor.

Sıfırlama Wi-Fi ağını ve parolasını, Firebase ayarlarını, cihaz ve hotspot adını, bir de sonradan değiştirdiysen panel parolasını siliyor. İlk açılışta üretilen panel ve OTA parolalarına ise dokunmuyor.

## Daha fazlası

- [Sorun giderme](docs/TROUBLESHOOTING.tr.md)
- [Firmware yapısı](docs/FIRMWARE.tr.md)

## Lisans

GPL-3.0. Ayrıntı için [LICENSE](LICENSE).
