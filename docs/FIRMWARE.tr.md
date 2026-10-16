# Firmware yapısı

[English](FIRMWARE.md)

Koda katkı yapacak ya da üstünde değişiklik yapacaklar için.

## Dosyalar

Ana akış UYANIK.ino içinde. Pinler, web sunucusu, HTTP uçları, setup ve loop, bir de uzak kontrol görevi.

Geri kalanlar birer iş üstleniyor. net.h be net.cpp Wi-Fi durum makinesini ve kurulum portalını yönetiyor. rtdb.h ve rtdb.cpp Firebase Realtime Database’in REST istemcisi. settings.h ve settings.cpp yapılandırmayı tutuyor ve kullanıcıdan gelen değerleri doğruluyor. identity.h ve identity.cpp cihaza özgü parolaları üretiyor ve fabrika sıfırlamayı yapıyor. press.h ve press.cpp güç ve reset düğmesine basma işini yapıyor. json.h JSON string kaçışı için, gts_root.h de Google’ın kök sertifikalarını içeriyor.

Yerel panelin kaynağı panel.html, tarayıcıda açıp önizleyebilirsin. Cihaza gömülen hâli panel_html.h, bu dosya build_html.py ile panel.html’den üretiliyor. Elle düzenlenmemeli.

Klasör adı .ino ile aynı olmak zorunda.

## RTDB Düğümleri

Her şey /<cihazAdı>/ altında duruyor.

cmd komut alanı. Panel buraya power_short, power_long ya da reset yazıyor, cihaz okuyup none yapıyor.

Geri kalanları cihaz yazıyor, paneller okuyor. state bilgisayarın durumu, on ya da off. ip dış IP, localIp yerel IP. lastSeen cihazın en son ne zaman haber verdiği, sunucu zaman damgası olarak. pcName de bilgisayarın uzak panelde görünen adı.

Komut şöyle ilerliyor: panel cmd’ye yazıyor, cihaz okuyor, önce alanı none yapıyor, sonra komutu uyguluyor, panel de alanın temizlendiğini görünce onaylıyor.

Buradaki sıra kritik. Tersi olsaydı, yani cihaz önce uygulayıp sonra silseydi, silme başarısız olduğunda komut veritabanında kalır ve bir sonraki yoklamada tekrar çalışırdı. Kullanıcı bir kez “zorla kapat” der, cihaz iki kez yapardı. Bir komutu kaybetmek bundan daha ucuz.

## HTTP uçları (yerel panel)

Yerel panelin konuştuğu uçlar bunlar.

/ panelin kendisini, /api/status durumu JSON olarak veriyor. Komutlar /api/cmd’ye POST ile gidiyor. /api/settings GET ile yapılandırmayı okuyor, POST ile kaydediyor.

Wi-Fi tarafında /api/wifi/scan GET ile son tarama sonucunu veriyor, POST ile yeni tarama başlatıyor. /api/wifi/connect bir ağa bağlanıyor, /api/wifi/forget kayıtlı ağı siliyor.

Oturum için /api/login parolayı alıp bir jeton dönüyor. /api/logout jetonu geçersiz kılıyor, bu açık olan tüm oturumları düşürüyor. /api/panelpw panel parolasını değiştiriyor.

Son olarak /api/factory fabrika sıfırlaması yapıyor, /api/restart cihazı yeniden başlatıyor.

Panelin kendisi hariç hepsi apiOk() kontrolünden geçiyor, ayrıntısı güvenlik notlarında.

## Zamanlamalar

Düğmeye basma süreleri: normal açma kapama için PRESS_SHORT 300 ms, zorla kapatma için PRESS_LONG 6 saniye, reset için PRESS_RESET yine 300 ms.

Cihaz komutları POLL_INTERVAL ile 2 saniyede bir yokluyor. Bilgisayarın durumuna STATE_INTERVAL ile 5 saniyede bir bakıyor. lastSeen alanını STAMP_INTERVAL ile 30 saniyede bir yazıyor. Dış IP’yi de IP_INTERVAL ile 5 dakikada bir soruyor.

## Dikkat edilecekler

Basma işi esp_timer geri çağrımında, döngüde değil. Kısa basış 300 ms, ATX’te zorla kapatma eşiği ise 4 saniye. Bırakma işi bloklayan bir çağrının arkasında beklerse “aç” komutu çalışan makineyi sertçe kapatabilir. Fabrika sıfırlama pininin örneklenmesi de aynı sebeple zamanlayıcıda.

esp_timer geri çağrımlarında Serial.printf kullanılmamalı. UART tamponu doluyken bloklar ve tüm zamanlayıcı geri çağrımlarını geciktirir. Yapılacak şey bir bayrak set etmek, yazdırmayı loop() içinde yapmak.

Fabrika sıfırlama pini 23 ms’de bir okunuyor ve 217 kesintisiz LOW istiyor, yani yaklaşık 5 saniye. 23 ms’nin seçilme sebebi şebeke paraziti: 50 Hz’in periyodu 20 ms, 60 Hz’inki 16.7 ms, 23 ms ikisiyle de ortak bölen paylaşmıyor. Böylece art arda gelen okumalar parazitin farklı anlarına denk geliyor. Tek bir HIGH okuma sayacı sıfırlıyor.

millis() 49 günde taşar. Bu yüzden zaman karşılaştırmaları farkın işaretiyle yapılıyor: (int32_t)(millis() - t) >= 0.

Token yenileme ucu alan adlarını snake_case döndürüyor (id_token), giriş ucu ise camelCase (idToken). Bu ikisi karıştırılırsa token hiç yenilenmez ve cihaz tam 60. dakikada Firebase bağlantısını kaybeder.

Üç kök sertifika gömülü. Google her uç için farklı bir sertifika zinciri kullanıyor: firebasedatabase.app için RSA, identitytoolkit.googleapis.com için ECC. Yalnızca biri gömülü olursa kimlik doğrulama sertifika hatasıyla düşer.

panel.html ile panel_html.h elle eşitlenmeye çalışılmasın. Bir kez denendi ve kaydı: kaynak güncellendi ama cihaz eski sürümü göstermeye devam etti. Derlemeden önce python build_html.py --kontrol çalıştır.