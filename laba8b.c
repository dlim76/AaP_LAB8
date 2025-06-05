#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char *word;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct DblLinkedList {
    Node *head;
    Node *tail;
    int size;
} DblLinkedList;

Node* createNode(const char *word) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }

    newNode->word = strdup(word);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void init_list(DblLinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void insert_end(DblLinkedList *list, const char *word) {
    Node *newNode = createNode(word);

    if (list->tail == NULL) {
        list->head = list->tail = newNode;
    } else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }

    list->size++;
}


void free_list(DblLinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void print_list(DblLinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        printf("%s", current->word);
        if (current->next != NULL) {
            printf(" ");
        }
        current = current->next;
    }
    printf("\n");
}

void split_words(DblLinkedList *list) {
    Node *current = list->head;

    while (current != NULL) {
        char *word = current->word;
        char *hyphen = strchr(word, '-');

        if (hyphen != NULL) {
            *hyphen = '\0';
            char *part1 = word;
            char *part2 = hyphen + 1;

            Node *newNode1 = createNode(part1);
            Node *newNode2 = createNode(part2);

            newNode1->prev = current->prev;
            newNode2->next = current->next;
            newNode1->next = newNode2;
            newNode2->prev = newNode1;

            if (current->prev != NULL) {
                current->prev->next = newNode1;
            } else {
                list->head = newNode1;
            }

            if (current->next != NULL) {
                current->next->prev = newNode2;
            } else {
                list->tail = newNode2;
            }

            free(current->word);
            free(current);

            current = newNode2->next;
            list->size++;
        } else {
            current = current->next;
        }
    }
}

void input_string(DblLinkedList *list) {
    char buffer[1024];
    int pos = 0;
    char c;
    int ch;
    printf("Введите строку (введите '.' для завершения ввода): ");

    while ((c = getchar()) != '.') {
        if (pos < sizeof(buffer) - 1) {
            buffer[pos++] = c;
        }
    }
    buffer[pos] = '\0';
    while ((ch = getchar()) != '\n' && ch != EOF);

    char *token = strtok(buffer, " ");
    while (token != NULL) {
        insert_end(list, token);
        token = strtok(NULL, " ");
    }
}

int main() {
    DblLinkedList list;
    init_list(&list);
    input_string(&list);

    printf("\nИсходный список слов:\n");
    print_list(&list);

    split_words(&list);

    printf("\nСписок после разделения слов с дефисами:\n");
    print_list(&list);

    free_list(&list);
    return 0;
}