# Linux Syslog Okuyucu ve Bağlı Liste Uygulaması

Bu proje, Linux işletim sistemindeki sistem günlüklerini (`/var/log/syslog`) okuyarak, bu verileri bellekte dinamik olarak **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısıyla tutan ve analiz eden bir C uygulamasıdır.



## 🚀 Projenin Amacı
Sistemde oluşan olayları, hataları ve uyarıları standart bir formatta kaydeden Syslog mekanizmasını anlamak ve bu yapılandırılmamış metin verilerini, bellek yönetimi (pointers, `malloc`, `free`) kurallarına uygun olarak dinamik bir veri yapısına aktarmaktır.

## 🛠️ Temel Özellikler
* **Dinamik Bellek Yönetimi:** Dosya boyutu önceden bilinmediği için her log satırı `malloc` kullanılarak bellekte anlık olarak oluşturulan düğümlere (Node) atanır. İşlem bitiminde `free` ile bellek sızıntısı (memory leak) önlenir.
* **Kronolojik Okuma (`next` pointer):** Veriler eklendiği sırayla, baştan sona doğru listelenir.
* **Geriye Dönük Analiz (`prev` pointer):** Çift yönlü bağlı listenin gücü kullanılarak, listenin sonundan (`tail`) başa doğru okuma yapılır. Bu özellik, sistem çökmelerinde "hatadan hemen önceki" olayları analiz etmek için kritik bir mantıktır.
* **Simülasyon Modu (Mock Data):** Program Linux ortamında `sudo` yetkisi olmadan çalıştırılırsa veya `/var/log/syslog` dosyasına erişemezse çökmek yerine, test amacıyla sanal loglar üreterek çalışmaya devam eder.

## 💻 Veri Yapısı Mimarisi
Projede kullanılan Çift Yönlü Bağlı Liste düğüm (struct) yapısı aşağıdaki gibidir:

```c
typedef struct Node {
    int log_id;                         // Düğümün benzersiz kimliği
    char log_level[20];                 // Log seviyesi (INFO, WARN, ERROR)
    char log_message[MAX_LINE_LENGTH];  // Orijinal log metni
    struct Node* prev;                  // Önceki düğümü işaret eden pointer (Ok operatörü)
    struct Node* next;                  // Sonraki düğümü işaret eden pointer
} Node;
