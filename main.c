#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Kullanici yapisi
typedef struct Kullanici {
    int id;
    struct Kullanici** arkadaslari;
    int arkadas_sayisi;
    int arkadas_kapasitesi;
} Kullanici;

// Agac dugumu yapisi
typedef struct AgacDugumu {
    Kullanici* kullanici;
    struct AgacDugumu** cocuklari;
    int cocuk_sayisi;
    int cocuk_kapasitesi;
} AgacDugumu;

// Ikili arama agaci dugumu yapisi (Kirmizi-Siyah yerine)
typedef struct BSTDugumu {
    Kullanici* kullanici;
    struct BSTDugumu *sol;
    struct BSTDugumu *sag;
} BSTDugumu;

#define MAX_SATIR_UZUNLUK 100
#define BASLANGIC_ARKADAS_KAPASITESI 5
#define BASLANGIC_COCUK_KAPASITESI 5
#define MAX_KULLANICI 100 // Tahmini maksimum kullanici sayisi

// Ikili arama agaci dugumu olusturma
BSTDugumu* yeni_bst_dugumu(Kullanici* kullanici) {
    BSTDugumu* dugum = (BSTDugumu*)malloc(sizeof(BSTDugumu));
    if (dugum) {
        dugum->kullanici = kullanici;
        dugum->sol = dugum->sag = NULL;
    }
    return dugum;
}

// Kullaniciyi bulma fonksiyonu
Kullanici* kullanici_bul(Kullanici* kullanicilar[], int kullanici_sayisi, int id) {
    int i;
    for (i = 0; i < kullanici_sayisi; i++) {
        if (kullanicilar[i] != NULL && kullanicilar[i]->id == id) {
            return kullanicilar[i];
        }
    }
    return NULL;
}

// Kullanicinin arkadas listesine arkadas ekleme (dinamik boyutlandirma)
bool kullanici_arkadas_ekle(Kullanici* kullanici, Kullanici* arkadas) {
    if (kullanici->arkadas_sayisi >= kullanici->arkadas_kapasitesi) {
        kullanici->arkadas_kapasitesi *= 2;
        Kullanici** yeni_arkadaslar = (Kullanici**)realloc(kullanici->arkadaslari, sizeof(Kullanici*) * kullanici->arkadas_kapasitesi);
        if (!yeni_arkadaslar) return false;
        kullanici->arkadaslari = yeni_arkadaslar;
    }
    kullanici->arkadaslari[kullanici->arkadas_sayisi++] = arkadas;
    return true;
}

int veriseti_oku(const char* dosya_adi, Kullanici* kullanicilar[]) {
    FILE* dosya = fopen(dosya_adi, "r");
    if (!dosya) {
        perror("Dosya acma hatasi");
        return -1;
    }

    char satir[MAX_SATIR_UZUNLUK];
    int kullanici_sayisi = 0;
    int satir_sayisi = 0;

    // Once kullanicilari oku
    while (fgets(satir, MAX_SATIR_UZUNLUK, dosya)) {
        satir_sayisi++;
        if (strncmp(satir, "USER", 4) == 0) {
            int id = atoi(satir + 5);
            if (id <= 0) {
                fprintf(stderr, "Hata: Gecersiz kullanici ID'si satir %d: %s", satir_sayisi, satir);
                fclose(dosya);
                int i;
                for (i = 0; i < kullanici_sayisi; i++) {
                    free(kullanicilar[i]->arkadaslari);
                    free(kullanicilar[i]);
                }
                return -1;
            }
            Kullanici* yeni_kullanici = (Kullanici*)malloc(sizeof(Kullanici));
            if (!yeni_kullanici) {
                fclose(dosya);
                perror("Bellek ayirma hatasi");
                int i;
                for (i = 0; i < kullanici_sayisi; i++) {
                    free(kullanicilar[i]->arkadaslari);
                    free(kullanicilar[i]);
                }
                return -1;
            }
            yeni_kullanici->id = id;
            yeni_kullanici->arkadaslari = (Kullanici**)malloc(sizeof(Kullanici*) * BASLANGIC_ARKADAS_KAPASITESI);
            if (!yeni_kullanici->arkadaslari) {
                free(yeni_kullanici);
                fclose(dosya);
                perror("Bellek ayirma hatasi");
                int i;
                for (i = 0; i < kullanici_sayisi; i++) {
                    free(kullanicilar[i]->arkadaslari);
                    free(kullanicilar[i]);
                }
                return -1;
            }
            yeni_kullanici->arkadas_sayisi = 0;
            yeni_kullanici->arkadas_kapasitesi = BASLANGIC_ARKADAS_KAPASITESI;
            kullanicilar[kullanici_sayisi++] = yeni_kullanici;
            if (kullanici_sayisi >= MAX_KULLANICI) break;
        }
    }
    rewind(dosya); // Dosyanin basina geri don

    satir_sayisi = 0;
    // Sonra arkadaslik iliskilerini oku
    while (fgets(satir, MAX_SATIR_UZUNLUK, dosya)) {
        satir_sayisi++;
        if (strncmp(satir, "FRIEND", 6) == 0) {
            int id1 = atoi(satir + 7);
            char* bosluk = strchr(satir + 7, ' ');
            if (bosluk) {
                int id2 = atoi(bosluk + 1);
                Kullanici* k1 = kullanici_bul(kullanicilar, kullanici_sayisi, id1);
                Kullanici* k2 = kullanici_bul(kullanicilar, kullanici_sayisi, id2);
                if (k1 && k2) {
                    kullanici_arkadas_ekle(k1, k2);
                    kullanici_arkadas_ekle(k2, k1);
                }
            }
        }
    }

    fclose(dosya);
    return kullanici_sayisi;
}

AgacDugumu* yeni_agac_dugumu(Kullanici* kullanici) {
    AgacDugumu* dugum = (AgacDugumu*)malloc(sizeof(AgacDugumu));
    if (!dugum) return NULL;
    dugum->kullanici = kullanici;
    dugum->cocuklari = (AgacDugumu**)malloc(sizeof(AgacDugumu*) * BASLANGIC_COCUK_KAPASITESI);
    if (!dugum->cocuklari) {
        free(dugum);
        return NULL;
    }
    dugum->cocuk_sayisi = 0;
    dugum->cocuk_kapasitesi = BASLANGIC_COCUK_KAPASITESI;
    return dugum;
}

bool agac_dugumu_cocuk_ekle(AgacDugumu* dugum, AgacDugumu* cocuk) {
    if (dugum->cocuk_sayisi >= dugum->cocuk_kapasitesi) {
        dugum->cocuk_kapasitesi *= 2;
        AgacDugumu** yeni_cocuklar = (AgacDugumu**)realloc(dugum->cocuklari, sizeof(AgacDugumu*) * dugum->cocuk_kapasitesi);
        if (!yeni_cocuklar) return false;
        dugum->cocuklari = yeni_cocuklar;
    }
    dugum->cocuklari[dugum->cocuk_sayisi++] = cocuk;
    return true;
}

// Genişlik öncelikli arkadaş oluşturma (her kullanıcı sadece bir kere ziyaret edilir)
AgacDugumu* iliski_agaci_olustur(Kullanici* kullanicilar[], int kullanici_sayisi, int baslangic_id) {
    Kullanici* baslangic_kullanici = kullanici_bul(kullanicilar, kullanici_sayisi, baslangic_id);
    if (!baslangic_kullanici) return NULL;

    AgacDugumu* kok_dugum = yeni_agac_dugumu(baslangic_kullanici);
    if (!kok_dugum) return NULL;

    AgacDugumu** kuyruk = (AgacDugumu**)malloc(sizeof(AgacDugumu*) * MAX_KULLANICI);
    if (!kuyruk) {
        free(kok_dugum->cocuklari);
        free(kok_dugum);
        return NULL;
    }
    int on = 0, arka = 0;
    kuyruk[arka++] = kok_dugum;

    bool* ziyaret_edilen = (bool*)calloc(MAX_KULLANICI + 1, sizeof(bool));
    if (!ziyaret_edilen) {
        free(kok_dugum->cocuklari);
        free(kok_dugum);
        free(kuyruk);
        return NULL;
    }
    ziyaret_edilen[baslangic_id] = true;

    while (on < arka) {
        AgacDugumu* mevcut_dugum = kuyruk[on++];
        Kullanici* mevcut_kullanici = mevcut_dugum->kullanici;

        int i;
        for (i = 0; i < mevcut_kullanici->arkadas_sayisi; i++) {
            Kullanici* arkadas = mevcut_kullanici->arkadaslari[i];
            if (!ziyaret_edilen[arkadas->id]) {
                ziyaret_edilen[arkadas->id] = true;
                AgacDugumu* yeni_dugum = yeni_agac_dugumu(arkadas);
                agac_dugumu_cocuk_ekle(mevcut_dugum, yeni_dugum);
                kuyruk[arka++] = yeni_dugum;
            }
        }
    }

    free(ziyaret_edilen);
    free(kuyruk);
    return kok_dugum;
}

void agaci_yazdir(AgacDugumu* dugum, int seviye) {
    if (dugum) {
        int i;
        for (i = 0; i < seviye; i++) printf("  ");
        printf("%d\n", dugum->kullanici->id);
        for (i = 0; i < dugum->cocuk_sayisi; i++) {
            agaci_yazdir(dugum->cocuklari[i], seviye + 1);
        }
    }
}

void belirli_mesafedeki_arkadaslari_bul_dfs_recursive(Kullanici* kullanicilar[], int kullanici_sayisi, int baslangic_id, int hedef_mesafe, int mevcut_mesafe, bool ziyaret_edilen[], int* sonuclar, int* sonuc_sayisi) {
    if (mevcut_mesafe == hedef_mesafe) {
        sonuclar[(*sonuc_sayisi)++] = baslangic_id;
        return;
    }

    Kullanici* mevcut_kullanici = kullanici_bul(kullanicilar, kullanici_sayisi, baslangic_id);
    if (mevcut_kullanici) {
        int i;
        for (i = 0; i < mevcut_kullanici->arkadas_sayisi; i++) {
            int arkadas_id = mevcut_kullanici->arkadaslari[i]->id;
            if (!ziyaret_edilen[arkadas_id]) {
                ziyaret_edilen[arkadas_id] = true;
                belirli_mesafedeki_arkadaslari_bul_dfs_recursive(kullanicilar, kullanici_sayisi, arkadas_id, hedef_mesafe, mevcut_mesafe + 1, ziyaret_edilen, sonuclar, sonuc_sayisi);
                ziyaret_edilen[arkadas_id] = false;
            }
        }
    }
}

void belirli_mesafedeki_arkadaslari_bul_dfs(Kullanici* kullanicilar[], int kullanici_sayisi, int baslangic_id, int mesafe) {
    bool* ziyaret_edilen = (bool*)calloc(MAX_KULLANICI + 1, sizeof(bool));
    int* sonuclar = (int*)malloc(sizeof(int) * MAX_KULLANICI);
    int sonuc_sayisi = 0;

    if (ziyaret_edilen && sonuclar) {
        ziyaret_edilen[baslangic_id] = true;
        belirli_mesafedeki_arkadaslari_bul_dfs_recursive(kullanicilar, kullanici_sayisi, baslangic_id, mesafe, 0, ziyaret_edilen, sonuclar, &sonuc_sayisi);
        printf("USER %d'nin %d mesafedeki arkadaslari: ", baslangic_id, mesafe);
        int i;
        for (i = 0; i < sonuc_sayisi; i++) {
            printf("%d ", sonuclar[i]);
        }
        printf("\n");
        free(sonuclar);
        free(ziyaret_edilen);
    } else {
        perror("Bellek ayirma hatasi");
    }
}

void ortak_arkadaslari_bul(Kullanici* kullanicilar[], int kullanici_sayisi, int id1, int id2) {
    Kullanici* k1 = kullanici_bul(kullanicilar, kullanici_sayisi, id1);
    Kullanici* k2 = kullanici_bul(kullanicilar, kullanici_sayisi, id2);
    if (k1 && k2) {
        printf("USER %d ve USER %d'nin ortak arkadaslari: ", id1, id2);
        int ortak_sayisi = 0;
        int i, j;
        for (i = 0; i < k1->arkadas_sayisi; i++) {
            for (j = 0; j < k2->arkadas_sayisi; j++) {
                if (k1->arkadaslari[i]->id == k2->arkadaslari[j]->id) {
                    printf("%d ", k1->arkadaslari[i]->id);
                    ortak_sayisi++;
                    break;
                }
            }
        }
        printf("\n");
        if (ortak_sayisi == 0) {
            printf("USER %d ve USER %d'nin ortak arkadasi bulunmamaktadir.\n", id1, id2);
        } else {
            printf("Toplam %d ortak arkadas bulundu.\n", ortak_sayisi);
        }
    } else {
        printf("Belirtilen kullanicilar bulunamadi.\n");
    }
}

void potansiyel_topluluklari_bul(Kullanici* kullanicilar[], int kullanici_sayisi, int ortak_esik) {
    printf("Potansiyel Topluluklar (Ortak Arkadas Esigi: %d):\n", ortak_esik);
    int bulundu = 0;
    int i, j, k, l;
    for (i = 0; i < kullanici_sayisi; i++) {
        for (j = i + 1; j < kullanici_sayisi; j++) {
            int ortak_sayisi = 0;
            for (k = 0; k < kullanicilar[i]->arkadas_sayisi; k++) {
                for (l = 0; l < kullanicilar[j]->arkadas_sayisi; l++) {
                    if (kullanicilar[i]->arkadaslari[k]->id == kullanicilar[j]->arkadaslari[l]->id) {
                        ortak_sayisi++;
                        break;
                    }
                }
            }
            if (ortak_sayisi >= ortak_esik) {
                printf("{%d, %d} (%d ortak arkadas)\n", kullanicilar[i]->id, kullanicilar[j]->id, ortak_sayisi);
                bulundu = 1;
            }
        }
    }
    if (!bulundu) {
        printf("Belirtilen esikte potansiyel topluluk bulunmamaktadir.\n");
    }
}

int etki_alani_hesapla(Kullanici* kullanicilar[], int kullanici_sayisi, int id) {
    Kullanici* kullanici = kullanici_bul(kullanicilar, kullanici_sayisi, id);
    if (kullanici) {
        return kullanici->arkadas_sayisi;
    }
    return 0;
}

// Ikili arama agacina kullanici ekleme
BSTDugumu* bst_ekle(BSTDugumu* kok, Kullanici* kullanici) {
    if (kok == NULL) {
        return yeni_bst_dugumu(kullanici);
    }

    if (kullanici->id < kok->kullanici->id) {
        kok->sol = bst_ekle(kok->sol, kullanici);
    } else if (kullanici->id > kok->kullanici->id) {
        kok->sag = bst_ekle(kok->sag, kullanici);
    }
    return kok;
}

// Ikili arama agacini olusturma
BSTDugumu* bst_olustur(Kullanici* kullanicilar[], int kullanici_sayisi) {
    BSTDugumu* kok = NULL;
    int i;
    for (i = 0; i < kullanici_sayisi; i++) {
        kok = bst_ekle(kok, kullanicilar[i]);
    }
    return kok;
}

// Ikili arama agacinda kullanici arama
Kullanici* bst_ara(BSTDugumu* kok, int id) {
    if (kok == NULL || kok->kullanici->id == id) {
        return (kok == NULL) ? NULL : kok->kullanici;
    }

    if (id < kok->kullanici->id) {
        return bst_ara(kok->sol, id);
    } else {
        return bst_ara(kok->sag, id);
    }
}

// Ikili arama agacini serbest birakma (bellek sizintilarini onlemek icin)
void bst_serbest_birak(BSTDugumu* dugum) {
    if (dugum) {
        bst_serbest_birak(dugum->sol);
        bst_serbest_birak(dugum->sag);
        free(dugum);
    }
}

// Kullanici listesini serbest birakma
void kullanicilari_serbest_birak(Kullanici* kullanicilar[], int kullanici_sayisi) {
    int i;
    for (i = 0; i < kullanici_sayisi; i++) {
        if (kullanicilar[i]) {
            free(kullanicilar[i]->arkadaslari);
            free(kullanicilar[i]);
        }
    }
}

// Iliski agacini serbest birakma
void iliski_agacini_serbest_birak(AgacDugumu* dugum) {
    if (dugum) {
        int i;
        for (i = 0; i < dugum->cocuk_sayisi; i++) {
            iliski_agacini_serbest_birak(dugum->cocuklari[i]);
        }
        free(dugum->cocuklari);
        free(dugum);
    }
}

// Tum kullanicilar icin etki alanlarini hesaplayan fonksiyon
void tum_etki_alanlarini_hesapla(Kullanici* kullanicilar[], int kullanici_sayisi) {
    printf("\n----- TUM KULLANICILARIN ETKI ALANLARI -----\n");
    int i = 0;
    while (i < kullanici_sayisi) {
        int etki_id = kullanicilar[i]->id;
        int etki = etki_alani_hesapla(kullanicilar, kullanici_sayisi, etki_id);
        printf("USER %d'un etki alani (dogrudan baglanti sayisi): %d\n", etki_id, etki);
        i++;
    }
    printf("----- TUM KULLANICILARIN ETKI ALANLARI SONU -----\n");
}

// Tum kullanicilar icin belirli mesafedeki arkadaslari bulan fonksiyon
void tum_mesafedeki_arkadaslari_bul(Kullanici* kullanicilar[], int kullanici_sayisi, int mesafe) {
    printf("\n----- TUM KULLANICILAR ICIN %d MESAFEDEKILER -----\n", mesafe);
    int i = 0;
    while (i < kullanici_sayisi) {
        int baslangic_id_dfs = kullanicilar[i]->id;
        printf("----- USER %d icin %d mesafedekiler -----\n", baslangic_id_dfs, mesafe);
        belirli_mesafedeki_arkadaslari_bul_dfs(kullanicilar, kullanici_sayisi, baslangic_id_dfs, mesafe);
        i++;
    }
    printf("----- TUM KULLANICILAR ICIN %d MESAFEDEKILER SONU -----\n", mesafe);
}

// Tum kullanicilari ikili arama agacina ekleyen ve arayan fonksiyon
void bst_islemleri(Kullanici* kullanicilar[], int kullanici_sayisi) {
    printf("\n----- IKILI ARAMA AGACI ISLEMLERI -----\n");
    BSTDugumu* kullanici_bst_kok = bst_olustur(kullanicilar, kullanici_sayisi);
    int i = 0;
    while (i < kullanici_sayisi) {
        int aranan_id = kullanicilar[i]->id;
        Kullanici* bulunan_kullanici = bst_ara(kullanici_bst_kok, aranan_id);
        printf("USER %d Ikili Arama Agacinda: %s\n", aranan_id, bulunan_kullanici ? "BULUNDU" : "BULUNAMADI");
        i++;
    }
    bst_serbest_birak(kullanici_bst_kok);
    printf("----- IKILI ARAMA AGACI ISLEMLERI SONU -----\n");
}

int main() {
    Kullanici* kullanicilar[MAX_KULLANICI] = {NULL};
    int kullanici_sayisi = veriseti_oku("veriseti.txt", kullanicilar);

    if (kullanici_sayisi > 0) {
        printf("Veri seti okundu. Toplam %d kullanici.\n\n", kullanici_sayisi);

        printf("----- TUM KULLANICILAR ICIN ANALIZLER -----\n");

        // TUM KULLANICILAR ICIN ILISKI AGACI OLUSTURMA VE YAZDIRMA
        printf("\n----- ILISKI AGACLARI -----\n");
        int i = 0;
        while (i < kullanici_sayisi) {
            int baslangic_id_agac = kullanicilar[i]->id;
            AgacDugumu* kok_agac = iliski_agaci_olustur(kullanicilar, kullanici_sayisi, baslangic_id_agac);
            printf("----- USER %d Iliski Agaci -----\n", baslangic_id_agac);
            if (kok_agac) {
                agaci_yazdir(kok_agac, 0);
                iliski_agacini_serbest_birak(kok_agac);
            } else {
                printf("USER %d icin iliski agaci olusturulamadi.\n", baslangic_id_agac);
            }
            i++;
        }
        printf("----- ILISKI AGACLARI SONU -----\n");

        // TUM KULLANICILAR ICIN BELIRLI MESAFEDEKILERI BULMA (mesafe=1 ve 2 icin)
        tum_mesafedeki_arkadaslari_bul(kullanicilar, kullanici_sayisi, 1);
        tum_mesafedeki_arkadaslari_bul(kullanicilar, kullanici_sayisi, 2);

        // TUM KULLANICI CIFTLERI ICIN ORTAK ARKADAS BULMA
        printf("\n----- ORTAK ARKADASLAR -----\n");
        i = 0;
        while (i < kullanici_sayisi) {
            int j = i + 1;
            while (j < kullanici_sayisi) {
                printf("----- USER %d ve USER %d Ortak Arkadaslari -----\n", kullanicilar[i]->id, kullanicilar[j]->id);
                ortak_arkadaslari_bul(kullanicilar, kullanici_sayisi, kullanicilar[i]->id, kullanicilar[j]->id);
                j++;
            }
            i++;
        }
        printf("----- ORTAK ARKADASLAR SONU -----\n");

        // POTANSIYEL TOPLULUKLARI BULMA (ortak esik = 1) - Bir kere yapilir, tum ciftler icin gecerli
        printf("\n----- POTANSIYEL TOPLULUKLAR (Esik 1) -----\n");
        potansiyel_topluluklari_bul(kullanicilar, kullanici_sayisi, 1);
        printf("----- POTANSIYEL TOPLULUKLAR SONU -----\n");

        // TUM KULLANICILAR ICIN ETKI ALANI HESAPLAMA
        tum_etki_alanlarini_hesapla(kullanicilar, kullanici_sayisi);

        // IKILI ARAMA AGACI ISLEMLERI
        bst_islemleri(kullanicilar, kullanici_sayisi);

        kullanicilari_serbest_birak(kullanicilar, kullanici_sayisi);
        printf("----- TUM KULLANICILAR ICIN ANALIZLER SONU -----\n");

    } else {
        printf("Veri seti okuma hatasi.\n");
    }
    return 0;
}
