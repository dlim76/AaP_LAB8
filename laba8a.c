#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Node{
    int deg, coeff;
    struct Node *next;
}Node;

typedef struct linked_list{
    Node *head;
    Node *tail;
    int size;
} linked_list;

void init_list(linked_list *list){
    list->head = list->tail = NULL;
    list->size = 0;
}

Node* createNode(int deg, int coeff) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    newNode->deg = deg;
    newNode->coeff = coeff;
    newNode->next = NULL;
    return newNode;
}

void insert_end(linked_list *list, int coeff, int deg){
    if (coeff == 0){
        printf("Коэффициент равен нулю");
        return;
    }

    Node *newNode = createNode(deg, coeff);
    if (list->tail == NULL){
        list->tail = list->head = newNode;
    }
    else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}


void insert_after(linked_list *list, int coeff, int deg) {
   if (coeff == 0){
       return;
   }

   Node *current = list->head;
   Node *prev = NULL;

   while (current != NULL && current->deg > deg){
       prev = current;
       current = current->next;
   }

   if (current != NULL && current->deg == deg){
       current->coeff += coeff;

       if (current->coeff == 0){
            if (prev == NULL) {
                list->head = current->next;
            }
            else {
                prev->next = current->next;
            }
           if (current == list->tail) {
               list->tail = prev;
           }

           free(current);
           list->size--;
       }
       return;
   }

   Node *newNode = createNode(deg, coeff);
   newNode->next = current;
   if (prev == NULL) {
       list->head = newNode;
   }
   if (current == NULL){
       list->tail = newNode;
   }
   else{
       prev->next = newNode;
   }

   list->size++;
}

void free_list(linked_list *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void print_list(linked_list *list) {
    Node *current = list->head;
    bool first_node = true;

    while (current != NULL) {
        //обработка знака для первого и следующих членов
        if (!first_node) {
            printf(current->coeff > 0 ? " + " : " - ");
        } else {
            first_node = false;
            if (current->coeff < 0) printf("-");
        }

        //обработка коэффициентов
        int abs_coeff = abs(current->coeff);
        if (abs_coeff != 1 || current->deg == 0) {
            printf("%d", abs_coeff);
        }

        //обработка степени
        if (current->deg > 0) {
            printf("x");
            if (current->deg > 1) printf("^%d", current->deg);
        }

        current = current->next;
    }
    printf("\n");
}


void input_polynomial(linked_list *list) {
    int max_degree;
    printf("Введите максимальную степень многочлена (n): ");
    scanf("%d", &max_degree);
    getchar();

    for (int deg = max_degree; deg >= 0; deg--) {
        int coeff;
        if (deg > 1) {
            printf("Введите коэффициент при x^%d: ", deg);
        }
        else if (deg == 1) {
            printf("Введите коэффициент при x: ");
        }
        else {
            printf("Введите коэффициент при x^0 (константе): ");
        }

        scanf("%d", &coeff);
        getchar();

        if (coeff != 0) {
            insert_end(list, coeff, deg);
        }
    }
}



int main() {
    linked_list poly;
    init_list(&poly);
    int choice, coeff, deg;

    while (1)
    {
        printf("\nМеню:\n");
        printf("1. Создать новый многочлен\n");
        printf("2. Добавить член в многочлен\n");
        printf("3. Вывести многочлен\n");
        printf("4. Выход\n");
        printf("Выберите опцию (1-4): ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                input_polynomial(&poly);
                printf("Многочлен создан: ");
                print_list(&poly);
                break;

            case 2:
                if (poly.size == 0)
                {
                    printf("Многочлен не создан! Сначала создайте многочлен.\n");
                    break;
                }
                printf("Введите коэффициент и степень (например, '-5 2' для -5x^2): ");
                scanf("%d%d", &coeff, &deg);
                getchar();
                if (deg < 0)
                {
                    printf("Степень не может быть отрицательной!\n");
                    break;
                }
                printf("Многочлен до добавления: ");
                print_list(&poly);
                insert_after(&poly, coeff, deg);
                printf("Многочлен после добавления: ");
                print_list(&poly);
                break;

            case 3: //
                if (poly.size == 0) {
                    printf("Многочлен не создан!\n");
                } else {
                    printf("Текущий многочлен: ");
                    print_list(&poly);
                }
                break;

            case 4:
                free_list(&poly);
                printf("Программа завершена.\n");
                return 0;

            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
    }

    return 0;
}
