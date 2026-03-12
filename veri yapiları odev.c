#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 512

// Cift Yonlu Bagli Liste Dugumu
typedef struct Node {
    int log_id;
    char log_level[20];
    char log_message[MAX_LINE_LENGTH];
    struct Node* prev;
    struct Node* next;
} Node;

Node* head = NULL;
Node* tail = NULL;
int global_id_counter = 1;

// Listeye log ekleme fonksiyonu
void addLog(const char* message) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->log_id = global_id_counter++;
    //ponter ve struct arasý köprü
    // Mesaj icerigine gore log seviyesini belirleme 
    if (strstr(message, "error") || strstr(message, "fail") || strstr(message, "fatal")) {
        strcpy(newNode->log_level, "[ ERROR ]");
    } else if (strstr(message, "warn") || strstr(message, "unauthorized")) {
        strcpy(newNode->log_level, "[ WARN  ]");
    } else {
        strcpy(newNode->log_level, "[ INFO  ]");
    }

    strncpy(newNode->log_message, message, MAX_LINE_LENGTH);
    newNode->log_message[strcspn(newNode->log_message, "\n")] = 0; // Sondaki alt satiri temizle
    newNode->next = NULL;
    
    if (head == NULL) {
        newNode->prev = NULL;
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

// Loglari bastan sona yazdirma
void printLogsForward() {
    Node* current = head;
    printf("\n====================================================================\n");
    printf("   SISTEM GUNLUKLERI (SYSLOG) - ILERI YONLU (KRONOLOJIK) OKUMA\n");
    printf("====================================================================\n");
    
    while (current != NULL) {
        printf("[Dugum ID: %03d] %s %s\n", current->log_id, current->log_level, current->log_message);
        printf("--------------------------------------------------------------------\n");
        current = current->next;
    }
}

// Loglari sondan basa yazdirma (Ters Kronolojik)
void printLogsReverse() {
    Node* current = tail;
    int count = 0;
    printf("\n====================================================================\n");
    printf("     SON EKLENEN KAYITLAR - GERI YONLU (PREV) BAGLANTI ILE\n");
    printf("====================================================================\n");
    
    // Sadece son 5 kaydi geriye dogru okuyarak cift yonlu listenin gucunu gosterdim
    while (current != NULL && count < 5) {
        printf("[Dugum ID: %03d] %s %s\n", current->log_id, current->log_level, current->log_message);
        current = current->prev;
        count++;
    }
    printf("--------------------------------------------------------------------\n");
}

// Dosya okunamadiginda video icin sanal veri ureten fonksiyon
void generateMockLogs() {
    printf("\n[!] UYARI: Gercek /var/log/syslog dosyasi okunamadi (Yetki eksikligi olabilir).\n");
    printf("[!] Video sunumu icin otomatik sanal loglar uretiliyor...\n\n");
    
    addLog("Mar 13 10:00:01 kernel: [    0.000000] Linux version 5.15.0-generic");
    addLog("Mar 13 10:00:02 systemd[1]: Starting system logging service...");
    addLog("Mar 13 10:00:05 NetworkManager[850]: <info>  [1647890000] interface (eth0) ready");
    addLog("Mar 13 10:05:12 sshd[1024]: Failed password for root from 192.168.1.10 port 22");
    addLog("Mar 13 10:05:15 sshd[1024]: error: maximum authentication attempts exceeded");
    addLog("Mar 13 10:10:00 cron[900]: (root) CMD ( /usr/lib/sysstat/sa1 1 1)");
    addLog("Mar 13 10:15:22 kernel: [   45.123456] usb 1-1: new high-speed USB device connected");
    addLog("Mar 13 10:20:01 sudo: mert : TTY=pts/0 ; PWD=/home/mert ; USER=root ; COMMAND=/bin/su");
    addLog("Mar 13 10:25:33 apache2[2048]: [warn] module rewrite_module is already loaded");
    addLog("Mar 13 10:30:00 mysql[3012]: ready for connections. Version: '8.0.28'");
}

int main() {
    printf("====================================================================\n");
    printf(" Ogrenci    : Mert Oruc (1250505077)\n");
    printf(" Proje      : Syslog Verilerinin Cift Yonlu Bagli Liste Ile Analizi\n");
    printf("====================================================================\n");

    FILE *file = fopen("/var/log/syslog", "r");
    
    if (file == NULL) {
        // Dosya yoksa veya okuma yetkisi yoksa sanal loglari yukle
        generateMockLogs();
    } else {
        char line[MAX_LINE_LENGTH];
        int line_count = 0;
        // Gercek dosyadan 50 satir oku
        while (fgets(line, sizeof(line), file) != NULL && line_count < 50) {
            addLog(line);
            line_count++;
        }
        fclose(file);
    }

   
    printLogsForward();
    
   
    printLogsReverse();

    // Bellegi temizleme
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
