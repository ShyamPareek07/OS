#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

// Handle alarm signal
void handle_alarm(int sig) {
    printf("Alarm went off: %d\n", sig);
}

int main() {
    int input, fd = -1;
    char name[255];
    char buffer[255];
    int pipefd[2];
    int pid;

    while (1) {
        printf("\nSelect an option:\n");
        printf("1. Pipe Communication\n2. FIFO Communication\n3. File Operations\n4. Info System Calls\n5. Process Operations\n6. Exit\n");
        scanf("%d", &input);

        switch (input) {
        case 1: {
            // Pipe Communication
            char *msg = "Hello from parent process\n";
            if (pipe(pipefd) == -1) {
                perror("Error creating pipe");
                exit(1);
            }

            pid = fork();
            if (pid == 0) {
                // Child reads from the pipe
                read(pipefd[0], buffer, 255);
                printf("From child process (PID: %d): %s\n", getpid(), buffer);
            } else {
                // Parent writes to the pipe
                write(pipefd[1], msg, strlen(msg));
                wait(NULL);
                close(pipefd[0]);
                close(pipefd[1]);
            }
            break;
        }
        case 2: {
            // FIFO Communication
            char *myfifo = "myfifo";
            mkfifo(myfifo, 0666);
            char write_buf[255], read_buf[255], option[1];
            
            while (1) {
                printf("1. Send a message\n2. Display unread messages\n");
                while (getchar() != '\n');
                scanf("%c", option);
                switch (option[0]) {
                case '1':
                    fd = open(myfifo, O_WRONLY, 0666);
                    if (fd == -1) {
                        perror("Error opening FIFO for writing");
                        exit(1);
                    }
                    printf("Enter a message to send: ");
                    scanf("%s", write_buf);
                    write(fd, write_buf, 255);
                    close(fd);
                    break;
                case '2':
                    fd = open(myfifo, O_RDONLY, 0666);
                    read(fd, read_buf, 255);
                    printf("Got message: %s\n", read_buf);
                    close(fd);
                    break;
                default:
                    printf("Invalid option\n");
                    break;
                }
            }
            break;
        }
        case 3: {
            // File Operations
            printf("1. Open\n2. Read\n3. Write\n4. Close\n5. Link\n6. Unlink\n7. Stat\n");
            scanf("%d", &input);
            switch (input) {
            case 1:
                // open
                printf("Enter file name: ");
                scanf("%s", name);
                fd = open(name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                if (fd == -1) {
                    perror("Error opening file");
                    exit(1);
                }
                printf("File opened successfully: %s\n", name);
                break;
            case 2:
                // read
                printf("Enter file name: ");
                scanf("%s", name);
                int file = open(name, O_RDONLY, S_IRUSR);
                if (file == -1) {
                    perror("Error opening file for reading");
                    break;
                }
                ssize_t bytes_read = read(file, buffer, sizeof(buffer) - 1);
                if (bytes_read == -1) {
                    perror("Error reading file");
                    break;
                }
                buffer[bytes_read] = '\0';
                printf("Bytes read: %ld\nContent: %s\n", bytes_read, buffer);
                close(file);
                break;
            case 3:
                // write
                if (fd == -1) {
                    printf("No file open\n");
                    break;
                }
                char *txt = "I am Linus Torvalds, creator of Linux\n";
                write(fd, txt, strlen(txt));
                printf("Done writing\n");
                break;
            case 4:
                // close
                if (fd == -1) {
                    printf("No file open\n");
                    break;
                }
                close(fd);
                fd = -1;
                printf("File closed successfully\n");
                break;
            case 5:
                // link
                printf("Enter path of link to create: ");
                char link_path[255];
                scanf("%s", link_path);
                if (link("test.txt", link_path) == -1) {
                    perror("Error creating link");
                    exit(1);
                }
                printf("Link created successfully\n");
                break;
            case 6:
                // unlink
                printf("Enter path of file to unlink: ");
                char unlink_path[255];
                scanf("%s", unlink_path);
                if (unlink(unlink_path) == -1) {
                    perror("Error unlinking file");
                    exit(1);
                }
                printf("File unlinked successfully\n");
                break;
            case 7:
                // stat
                if (fd == -1) {
                    printf("No file open\n");
                    break;
                }
                struct stat stat_buff;
                if (stat(name, &stat_buff) == -1) {
                    perror("Error getting file stats");
                    break;
                }
                printf("Size: %ld\nInode: %ld\n", stat_buff.st_size, stat_buff.st_ino);
                break;
            default:
                printf("Invalid input\n");
            }
            break;
        }
        case 4: {
            // Info System Calls (sleep and alarm)
            printf("1. Sleep\n2. Alarm\n");
            scanf("%d", &input);
            switch (input) {
            case 1:
                printf("Enter number of seconds to sleep: ");
                scanf("%d", &input);
                //sleep(input);
                for (int i = 0; i < 100000000; i++);
                break;
            case 2:
                printf("Enter number of seconds to set alarm: ");
                scanf("%d", &input);
                signal(SIGALRM, handle_alarm);
                alarm(input);
                while (1);  // Keep the program running to catch alarm signal
                break;
            default:
                printf("Invalid input\n");
                break;
            }
            break;
        }
        case 5: {
            // Process Operations (fork, getpid, exec, etc.)
            printf("1. Create a child process\n2. Get PID\n3. Get Parent PID\n4. Exec\n5. Wait\n6. Exit\n");
            scanf("%d", &input);
            switch (input) {
            case 1:
                pid = fork();
                if (pid == 0) {
                    exit(1);
                } else {
                    printf("Child with PID %d created\n", pid);
                }
                break;
            case 2:
                printf("PID: %d\n", getpid());
                break;
            case 3:
                printf("Parent PID: %d\n", getppid());
                break;
            case 4:
                {
                    char cmd[255];
                    char path[255];
                    char arg[33];
                    printf("Enter path: ");
                    scanf("%s", path);
                    printf("Enter argument: ");
                    scanf("%s", arg);
                    execl(path, "", arg, NULL);
                }
                break;
            case 5:
                pid = fork();
                if (pid) {
                    int status;
                    wait(&status);
                    if (WIFEXITED(status)) {
                        printf("Child exited with code %d\n", status);
                    }
                } else {
                    printf("Child process starting\n");
                    for (int i = 0; i < 100000000; i++);
                    printf("Child process done\n");
                    exit(0);
                }
                break;
            case 6:
                exit(0);
                break;
            default:
                printf("Invalid input\n");
                break;
            }
            break;
        }
        case 6:
            // Exit
            exit(0);
        default:
            printf("Invalid input\n");
            break;
        }
    }
    return 0;
}
