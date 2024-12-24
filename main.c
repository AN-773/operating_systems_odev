/*
 * Basit bir Linux kabuk (shell) uygulaması
 * Bu program, temel komut satırı işlemlerini gerçekleştirebilen bir shell uygulamasıdır.
 * Özellikleri:
 * - Temel komutları çalıştırma
 * - Pipe (|) kullanarak komutları zincirleme
 * - Input/Output yönlendirme (< ve >)
 * - Arka planda çalıştırma (&)
 * - Dizin değiştirme (cd komutu)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

/* Sabit tanımlamalar */
#define MAX_COMMAND_LENGTH 1024    // Maksimum komut uzunluğu
#define MAX_ARGUMENTS 100          // Bir komut için maksimum argüman sayısı
#define MAX_BG_PROCESSES 100       // Maksimum arka plan işlem sayısı

/* Global değişkenler */
pid_t background_processes[MAX_BG_PROCESSES];  // Arka plan işlemlerinin PID'lerini tutan dizi
int bg_process_count = 0;                      // Mevcut arka plan işlem sayısı

/* Komut istemini ekrana yazdıran fonksiyon */
void print_prompt()
{
    // Komut istemini ekrana yazdır
    printf("> ");
    fflush(stdout);
}


/*
 * Ana fonksiyon
 * İşlevler:
 * - SIGCHLD sinyalini yakalar
 * - Sonsuz döngüde kullanıcı komutlarını bekler
 * - Komutları işlemek için gerekli fonksiyonları çağırır
 */
int main()
{

    char input[MAX_COMMAND_LENGTH];

    // Ana program döngüsü
    while (1)
    {
        // Komut istemini göster
        print_prompt();

        if (fgets(input, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            perror("Input reading failed");
            continue;
        }

    }

    return 0;
}
