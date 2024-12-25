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
 * Kullanıcı girdisini işleyen ve uygun şekilde çalıştıran fonksiyon
 * Parametreler:
 * - input: Kullanıcıdan alınan komut satırı
 * İşlevler:
 * - Pipe içeren ve içermeyen komutları ayırt eder
 * - Uygun çalıştırma fonksiyonunu çağırır
 */
void parse_and_execute(char *input)
{
    char *commands[MAX_ARGUMENTS];
    int command_count = 0;

    // Split input by ';'
    char *token = strtok(input, ";");
    while (token != NULL && command_count < MAX_ARGUMENTS - 1)
    {
        commands[command_count++] = token;
        if (strstr(token, "<") != NULL)
        {
            // This will fix error showing after prompt > {Error}
            char sleep[] = "sleep 0.2";
            commands[command_count++] = sleep;
        }
        token = strtok(NULL, ";");
    }
    commands[command_count] = NULL;

    for (int i = 0; i < command_count; i++)
    {
        if (strchr(commands[i], '|') != NULL)
        {
            // TODO - Pipe işlemini gerçekleştir
//            execute_piped_commands(commands[i]);
        }
        else
        {
            // TODO - Pipe içermeyen komutları çalıştır
//            execute_command(commands[i]);
        }
    }
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
