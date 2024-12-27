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

#include "shell.h"

/* Arka planda çalışan süreçlerin PID'lerini tutan dizi */
pid_t background_processes[MAX_BG_PROCESSES];
int bg_process_count = 0;


/* Komut istemini ekrana yazdıran fonksiyon */
void print_prompt()
{
    // Komut istemini ekrana yazdır
    printf("> ");
    fflush(stdout);
}

/*
 * Tek bir komutu çalıştıran fonksiyon
 * Parametreler:
 * - command: Çalıştırılacak komut dizisi
 * İşlevler:
 * - Komutu argümanlara ayırır
 * - Input/Output yönlendirmelerini işler
 * - Arka plan çalıştırma kontrolü yapar
 * - Özel komutları (cd, quit) işler
 */
void execute_command(char *command)
{
    // Komut argümanlarını ve yönlendirme değişkenlerini tanımla
    char *args[MAX_ARGUMENTS];
    int arg_count = 0;
    char *input_file = NULL, *output_file = NULL;
    int background = 0;

    // Komutu boşluklara göre parçala ve argümanları işle
    char *token = strtok(command, " ");
    while (token != NULL && arg_count < MAX_ARGUMENTS - 1)
    {
        // Input yönlendirmesi (<) kontrolü
        if (strcmp(token, "<") == 0)
        {
            token = strtok(NULL, " ");
            if (token)
                input_file = token;
        }
            // Output yönlendirmesi (>) kontrolü
        else if (strcmp(token, ">") == 0)
        {
            token = strtok(NULL, " ");
            if (token)
                output_file = token;
        }
            // Arka plan işlemi (&) kontrolü
        else if (strcmp(token, "&") == 0)
        {
            background = 1;
        }
            // Normal argüman ise diziye ekle
        else
        {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    // Boş komut kontrolü
    if (arg_count == 0)
        return;

    // Çıkış komutu kontrolü
    if (strcmp(args[0], "quit") == 0)
    {
        printf("Exiting shell...\n");
        exit(0);
    }

    // cd (dizin değiştirme) komutu kontrolü
    if (strcmp(args[0], "cd") == 0)
    {
        if (arg_count < 2)
        {
            // Argüman verilmemişse ev dizinine git
            char *home = getenv("HOME");
            if (home != NULL && chdir(home) != 0)
            {
                perror("cd");
            }
        }
        else if (chdir(args[1]) != 0)
        {
            perror("cd");
        }
        return;
    }

    // Yeni bir süreç oluştur
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return;
    }

    if (pid == 0)
    {
        // Çocuk süreç işlemleri

        // Input yönlendirmesi varsa dosyayı aç ve stdin'e yönlendir
        if (input_file)
        {
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0)
            {
                perror("Input file error");
                exit(EXIT_FAILURE);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        // Output yönlendirmesi varsa dosyayı aç ve stdout'a yönlendir
        if (output_file)
        {
            int fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0)
            {
                perror("Output file error");
                exit(EXIT_FAILURE);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        // Komutu çalıştır
        if (execvp(args[0], args) < 0)
        {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Ana süreç işlemleri
        if (background)
        {
            // Arka plan işlemi ise PID'i kaydet ve devam et
            printf("[PID %d] Running in background\n", pid);
            if (bg_process_count < MAX_BG_PROCESSES)
            {
                background_processes[bg_process_count++] = pid;
            }
        }
        else
        {
            // Ön plan işlemi ise çocuk sürecin bitmesini bekle
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

/*
 * Pipe ile bağlanmış komutları çalıştıran fonksiyon
 * Parametreler:
 * - command: Pipe (|) ile ayrılmış komutlar dizisi
 * İşlevler:
 * - Komutları pipe karakterine göre ayırır
 * - Her komut için ayrı bir süreç oluşturur
 * - Süreçler arası iletişimi pipe ile sağlar
 */
void execute_piped_commands(char *command)
{
    // Komutları pipe karakterine göre ayır
    char *commands[MAX_ARGUMENTS];
    int command_count = 0;
    char *token = strtok(command, "|");
    while (token != NULL && command_count < MAX_ARGUMENTS - 1)
    {
        commands[command_count++] = token;
        token = strtok(NULL, "|");
    }
    commands[command_count] = NULL;

    // Her komut çifti için pipe oluştur
    int pipes[MAX_ARGUMENTS][2];
    for (int i = 0; i < command_count - 1; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("Pipe creation failed");
            return;
        }
    }

    // Her komut için yeni bir süreç oluştur
    for (int i = 0; i < command_count; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            return;
        }

        if (pid == 0)
        {
            // İlk komut değilse, önceki pipe'ın çıkışını stdin'e bağla
            if (i > 0)
            {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            // Son komut değilse, sonraki pipe'ın girişini stdout'a bağla
            if (i < command_count - 1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Tüm pipe dosya tanımlayıcılarını kapat
            for (int j = 0; j < command_count - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Komutu çalıştır
            execute_command(commands[i]);
            exit(0);
        }
    }

    // Ana süreçte tüm pipe'ları kapat
    for (int i = 0; i < command_count - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Tüm çocuk süreçlerin bitmesini bekle
    for (int i = 0; i < command_count; i++)
    {
        wait(NULL);
    }
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
            execute_piped_commands(commands[i]);
        }
        else
        {
            execute_command(commands[i]);
        }
    }
}

/*
 * Arka plan işlemlerinin sonlanmasını yakalayan sinyal işleyici
 * Parametreler:
 * - sig: Sinyal numarası (SIGCHLD için)
 * İşlevler:
 * - Sonlanan arka plan işlemlerini temizler
 * - İşlem durumunu kontrol eder ve raporlar
 */
void handle_child_exit(int sig)
{
    int status;
    pid_t pid;

    // Sonlanan tüm çocuk süreçleri kontrol et
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        // Check if this was a background process
        int is_background = 0;
        for (int i = 0; i < bg_process_count; i++)
        {
            if (background_processes[i] == pid)
            {
                is_background = 1;
                // Remove this PID from our tracking array
                for (int j = i; j < bg_process_count - 1; j++)
                {
                    background_processes[j] = background_processes[j + 1];
                }
                bg_process_count--;
                break;
            }
        }

        if (is_background && WIFEXITED(status))
        {
            printf("\n[%d] Retval : %d\n", pid, WEXITSTATUS(status));
            fflush(stdout);
            print_prompt();
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
    // SIGCHLD sinyali için yapılandırma
    struct sigaction sa;
    sa.sa_handler = handle_child_exit;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
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

        // Satır sonu karakterini kaldır
        input[strcspn(input, "\n")] = '\0';

        // Komutu işle ve çalıştır
        parse_and_execute(input);
    }

    return 0;
}
