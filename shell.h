//
// Created by Abdalrahman Nasr, FUAD AISHAN, BERKAY CAN, Shahd Numa Yaldanı
//

#ifndef SHELL_H
#define SHELL_H

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
extern pid_t background_processes[MAX_BG_PROCESSES];
extern int bg_process_count;

/* Fonksiyon prototipleri */
void print_prompt();
void execute_command(char *command);
void execute_piped_commands(char *command);
void parse_and_execute(char *input);

#endif // SHELL_H
