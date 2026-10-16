# Sorun Giderme

[English](TROUBLESHOOTING.md)

**No such file or directory**

Sketch klasörü eksik. Arduino yalnızca .ino ile aynı klasördeki dosyaları derler; bir üst klasördekileri ne görür ne derler. software/UYANIK/ klasörünün tamamı bir arada olmalı ve klasör adı UYANIK.ino ile aynı olmalı.

**Panelde yaptığın değişiklik cihazda görünmüyor**

panel.html düzenlendi ama panel_html.h yeniden üretilmedi. Cihaz gömülü kopyayı servis ediyor, kaynağı değil.

```bash
python build_html.py --kontrol
```

Bayat diyorsa "python build_html.py" çalıştır, sonra yeniden derle.

**Bilgisayar açıkken çalışıyor, kapanınca ölüyor**

USB header'a standby gücü gelmiyor. İki BIOS ayarı birden gerekiyor (EuP 2013 = Disabled ve Resume By USB Device = Enabled). Bir zamanlar
çalışıp sonra durduysa ayarlar sıfırlanmıştır. CMOS pili bitmiş ya da BIOS güncellenmiş olabilir.

Ölçerek doğrula. Bilgisayar kapalı, fiş takılı, PSU anahtarı açık, JUSB'nin VCC–GND pinleri arasında ~5 V.

Kalıcı çözüm isteniyorsa 24-pin pin 9 (mor kablo) BIOS'tan bağımsızdır.

**Cihaz rastgele yeniden başlıyor**

Wi-Fi gönderimi sırasındaki ani çekim besleme gerilimini düşürüyor.

**Açılışta ilk bağlantı denemesi düşüyor**

Normal. Seri portta "Connection failed" görürsün, hemen ardından ikinci deneme bağlanır. Modem ilk isteğini reddediyor, ESP32 ile ev modemlerinde yaygın.

Her denemede düşüyorsa parola gerçekten yanlış olabilir, ama tek bir başarısız denemeye bakıp karar verme.

**uyanik.local çözülmüyor**

mDNS her ağda ve her istemcide çalışmıyor. Panelin gösterdiği yerel IP'yi kullan. Modemden DHCP rezervasyonu yaparsan adres sabit kalır.

**Kurulum portalı açılmıyor**

Hotspot'a bağlandıktan sonra sayfa kendiliğinden açılmazsa 192.168.4.1 adresine git.

**Yetki reddedildi**

Realtime Database kuralları giriş yaptığın hesaba izin vermiyor. Kurallardaki UID ile hesabın UID'si aynı mı, ve kuraldaki düğüm adı cihaz adıyla eşleşiyor mu ikisine de bak.

**Fabrika sıfırlama çalışmıyor**

GPIO 4 ile GND arasındaki kısa devrenin 5 saniye kesintisiz sürmesi gerekiyor. Tek bir kopma sayacı sıfırlar. Dahili pull-up kullanılıyor, harici direnç gerekmiyor.
