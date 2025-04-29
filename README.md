# Sosyal Ağ Uygulaması (C ile Geliştirildi)

Bu proje, C programlama dilinde geliştirilmiş basit bir sosyal ağ uygulamasıdır. Kullanıcıları, arkadaşlık ilişkilerini ve bu ilişkiler üzerinden çeşitli etkileşimleri modellemeyi hedefler. Veri setinden kullanıcıları ve arkadaşlıklarını okuyabilir, ilişki ağaçları oluşturabilir, belirli mesafedeki arkadaşları bulabilir, ortak arkadaşları tespit edebilir, potansiyel toplulukları belirleyebilir ve kullanıcıların etki alanlarını hesaplayabilir. Ayrıca, kullanıcı verilerini hızlı bir şekilde aramak için bir ikili arama ağacı (Binary Search Tree - BST) uygulaması da içerir.

## İçindekiler

- [Projeye Genel Bakış](#projeye-genel-bakış)
- [Özellikler](#özellikler)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [Veri Seti Formatı](#veri-seti-formatı)
- [Fonksiyonlar ve Algoritmalar](#fonksiyonlar-ve-algoritmalar)
- [Katkıda Bulunma](#katkıda-bulunma)
- [Lisans](#lisans)
- [İletişim](#iletişim)

## Projeye Genel Bakış

Bu proje, sosyal ağlardaki temel etkileşimleri modellemek ve analiz etmek için bir başlangıç noktası sunar. Amacı, farklı veri yapıları (diziler, ağaçlar) ve algoritmaların (genişlik öncelikli arama, derinlik öncelikli arama) sosyal ağ uygulamalarındaki kullanımını göstermektir. Kırmızı-Siyah ağacı yerine basit bir ikili arama ağacı kullanılarak, kullanıcıların hızlı bir şekilde bulunması sağlanır.

## Özellikler

- **Veri Okuma:** Bir metin dosyasından kullanıcıları ve arkadaşlık ilişkilerini okuyabilme.
- **Kullanıcı Yönetimi:** Kullanıcıları dinamik olarak saklama ve yönetebilme.
- **Arkadaşlık İlişkileri:** Kullanıcılar arasında dinamik olarak arkadaşlık ilişkileri kurabilme.
- **İlişki Ağacı Oluşturma:** Belirli bir kullanıcıdan başlayarak arkadaşlık ilişkilerini genişlik öncelikli arama (BFS) algoritması ile ağaç yapısında görselleştirebilme.
- **Belirli Mesafedeki Arkadaşları Bulma:** Belirtilen bir kullanıcıya belirli bir mesafede (doğrudan arkadaşları, arkadaşının arkadaşları vb.) bulunan kullanıcıları derinlik öncelikli arama (DFS) algoritması ile bulabilme.
- **Ortak Arkadaşları Bulma:** İki kullanıcının ortak arkadaşlarını tespit edebilme.
- **Potansiyel Toplulukları Belirleme:** Belirli bir eşik değerine göre potansiyel arkadaş gruplarını (ortak arkadaş sayısı belirli bir sayının üzerinde olan kullanıcı çiftleri) belirleyebilme.
- **Etki Alanı Hesaplama:** Bir kullanıcının doğrudan bağlantı sayısını (arkadaş sayısını) hesaplayarak etki alanını belirleyebilme.
- **İkili Arama Ağacı (BST):** Kullanıcıları ID'lerine göre saklamak ve hızlı bir şekilde aramak için basit bir ikili arama ağacı yapısı ve işlemleri.
- **Dinamik Bellek Yönetimi:** Kullanıcıların ve arkadaş listelerinin boyutları arttıkça dinamik olarak bellek ayırabilme.

## Kurulum

Bu projeyi çalıştırmak için bir C derleyicisine (örneğin GCC) ihtiyacınız olacaktır.

1.  **Depoyu klonlayın:**
    ```bash
    git clone [Bu deponun GitHub URL'si]
    cd [depo adı]
    ```

2.  **Kodu derleyin:**
    ```bash
    gcc sosyal_medya_analiz.c -o sosyal_ag_uygulamasi
    ```
    **Not:** Çıktı dosyasının adı `sosyal_ag_uygulamasi` olarak değiştirilmiştir.

## Kullanım

1.  **Veri Seti Hazırlama:** Projenin doğru çalışabilmesi için `veriseti.txt` adında bir metin dosyası oluşturmanız ve aşağıda belirtilen formatta kullanıcı ve arkadaşlık bilgilerini eklemeniz gerekmektedir.

2.  **Programı Çalıştırma:**
    ```bash
    ./sosyal_ag_uygulamasi
    ```

    Program, `veriseti.txt` dosyasını okuyacak, analizleri gerçekleştirecek ve sonuçları konsola yazdıracaktır.

## Veri Seti Formatı

`veriseti.txt` dosyası aşağıdaki formatta olmalıdır:

- **Kullanıcı Ekleme:** Her kullanıcı "USER" kelimesiyle başlamalı ve ardından kullanıcının benzersiz ID'si gelmelidir.
- **Arkadaşlık Ekleme:** Her arkadaşlık ilişkisi "FRIEND" kelimesiyle başlamalı ve ardından birbirine arkadaş olan iki kullanıcının ID'leri bir boşlukla ayrılmış şekilde yazılmalıdır.

  **Önemli:** Arkadaşlık ilişkileri çift yönlü olarak kabul edilir. Yani "FRIEND 1 2" satırı, hem Kullanıcı 1'in Kullanıcı 2'nin arkadaşı olduğunu hem de Kullanıcı 2'nin Kullanıcı 1'in arkadaşı olduğunu belirtir.

## Fonksiyonlar ve Algoritmalar

Projede kullanılan temel veri yapıları ve algoritmalar şunlardır:

- **`Kullanici` Yapısı:** Kullanıcı bilgilerini (ID, arkadaş listesi, arkadaş sayısı ve kapasitesi) tutar. Arkadaş listesi dinamik bir dizi olarak yönetilir.
- **`AgacDugumu` Yapısı:** İlişki ağacını oluşturmak için kullanılır. Her düğüm bir kullanıcıyı ve çocuklarını (arkadaşlarını) temsil eder. Çocuklar da dinamik bir dizi olarak yönetilir.
- **`BSTDugumu` Yapısı:** İkili arama ağacının düğümlerini temsil eder. Her düğüm bir kullanıcıyı ve sol/sağ alt ağaçlarını içerir.
- **`kullanici_bul`:** Verilen ID'ye sahip kullanıcıyı bir kullanıcı dizisi içinde arar.
- **`kullanici_arkadas_ekle`:** Bir kullanıcının arkadaş listesine dinamik olarak bir arkadaş ekler.
- **`veriseti_oku`:** Belirtilen dosyadan kullanıcıları ve arkadaşlık ilişkilerini okur ve `Kullanici` dizisini doldurur.
- **`yeni_agac_dugumu` ve `agac_dugumu_cocuk_ekle`:** İlişki ağacı düğümlerini oluşturur ve çocuk ekler.
- **`iliski_agaci_olustur`:** Genişlik öncelikli arama (BFS) kullanarak belirli bir kullanıcıdan başlayarak ilişki ağacını oluşturur. Ziyaret edilen kullanıcıları takip etmek için bir `ziyaret_edilen` dizisi kullanılır.
- **`agaci_yazdir`:** Oluşturulan ilişki ağacını konsola seviyeli bir şekilde yazdırır.
- **`belirli_mesafedeki_arkadaslari_bul_dfs_recursive` ve `belirli_mesafedeki_arkadaslari_bul_dfs`:** Derinlik öncelikli arama (DFS) kullanarak belirli bir mesafedeki arkadaşları bulur.
- **`ortak_arkadaslari_bul`:** İki kullanıcının ortak arkadaşlarını bulur.
- **`potansiyel_topluluklari_bul`:** Belirli bir ortak arkadaş eşiğine göre potansiyel toplulukları (kullanıcı çiftlerini) belirler.
- **`etki_alani_hesapla`:** Bir kullanıcının doğrudan arkadaş sayısını hesaplar.
- **`yeni_bst_dugumu`, `bst_ekle`, `bst_olustur`, `bst_ara`, `bst_serbest_birak`:** İkili arama ağacı (BST) işlemleri için kullanılan fonksiyonlardır. Kullanıcıları ID'lerine göre ekler, oluşturur, arar ve ağacın belleğini serbest bırakır.
- **`kullanicilari_serbest_birak` ve `iliski_agacini_serbest_birak`:** Bellek sızıntılarını önlemek için ayrılan bellek alanlarını serbest bırakır.
- **`tum_etki_alanlarini_hesapla`, `tum_mesafedeki_arkadaslari_bul`, `bst_islemleri`:** Tüm kullanıcılar için belirli analizleri toplu olarak gerçekleştiren yardımcı fonksiyonlardır.

## Katkıda Bulunma

Bu proje, sosyal ağ modellemesine giriş niteliğinde basit bir örnektir. Geliştirmeler ve katkılar memnuniyetle karşılanır. Katkıda bulunmak için lütfen aşağıdaki adımları izleyin:

1.  Projeyi fork edin.
2.  Kendi branch'inizi oluşturun (`git checkout -b feature/harika-ozellik`).
3.  Değişikliklerinizi commit edin (`git commit -am 'Yeni bir özellik eklendi'`).
4.  Branch'inizi push edin (`git push origin feature/harika-ozellik`).
5.  Pull Request oluşturun.

## Lisans

Bu proje altında lisanslanmıştır. Lisans detayları için lütfen `LICENSE` dosyasına bakın.

## İletişim

[Adınız Soyadınız] - [E-posta adresiniz] - [GitHub profilinizin URL'si]
