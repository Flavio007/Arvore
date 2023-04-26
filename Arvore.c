#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Node
{
    struct Node *l;
    struct Node *r;
    struct Node *up;
    long Cpf;
    char Nome[50];
    char Data[15];
    long Telefone;
    short Peso;
};

int insert(long cpf, char nome[], char data[], long telefone, struct Node **root)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->l = NULL;
    node->r = NULL;
    node->up = NULL;
    node->Cpf = cpf;
    strcpy(node->Nome, nome);
    strcpy(node->Data, data);
    node->Telefone = telefone;
    node->Peso = 0;

    if (root == NULL)
    {
        root = node;
        return 1;
    }

    struct Node *current = root;
    struct Node *parent = NULL;

    while (current != NULL)
    {
        parent = current;

        if (cpf < current->Cpf)
        {
            current = current->l;
        }
        else if (cpf > current->Cpf)
        {
            current = current->r;
        }
        else
        {
            return 0;
        }
    }
    node->up = parent;
    if (cpf < parent->Cpf)
    {
        parent->l = node;
    }
    else
    {
        parent->r = node;
    }

    return 1;
}

int remove_p(long cpf, struct Node **root)
{
    struct Node *current = *root;

    while (current != NULL && current->Cpf != cpf)
    {
        if (cpf < current->Cpf)
        {
            current = current->l;
        }
        else
        {
            current = current->r;
        }
    }

    if (current == NULL)
    {
        return 0;
    }

    struct Node *parent = current->up;

    if (current->l == NULL && current->r == NULL)
    {
        if (parent == NULL)
        {
            *root = NULL;
        }
        else if (parent->l == current)
        {
            parent->l = NULL;
        }
        else
        {
            parent->r = NULL;
        }
        free(current);
        return 1;
    }

    if (current->l == NULL || current->r == NULL)
    {
        struct Node *child;
        if (current->l == NULL)
        {
            child = current->r;
        }
        else
        {
            child = current->l;
        }

        if (parent == NULL)
        {
            *root = child;
        }
        else if (parent->l == current)
        {
            parent->l = child;
        }
        else
        {
            parent->r = child;
        }
        child->up = parent;
        free(current);
        return 1;
    }

    struct Node *min_right = current->r;
    while (min_right->l != NULL)
    {
        min_right = min_right->l;
    }

    struct Node *min_right_parent = min_right->up;

    if (min_right_parent == current)
    {
        min_right->l = current->l;
        current->l->up = min_right;
        min_right->up = parent;

        if (parent == NULL)
        {
            *root = min_right;
        }
        else if (parent->l == current)
        {
            parent->l = min_right;
        }
        else
        {
            parent->r = min_right;
        }

        free(current);
        return 1;
    }

    struct Node *min_right_child = min_right->r;
    min_right_parent->l = min_right_child;
    if (min_right_child != NULL)
    {
        min_right_child->up = min_right_parent;
    }

    min_right->l = current->l;
    min_right->l->up = min_right;
    min_right->r = current->r;
    min_right->r->up = min_right;
    min_right->up = parent;

    if (parent == NULL)
    {
        *root = min_right;
    }
    else if (parent->l == current)
    {
        parent->l = min_right;
    }
    else
    {
        parent->r = min_right;
    }

    free(current);
    return 1;
}

int altura(struct Node *n)
{
    if (n == NULL)
    {
        return 0;
    }
    else
    {
        int lh = altura(n->l);
        int rh = altura(n->r);
        if (lh > rh)
        {
            return lh + 1;
        }
        else
        {
            return rh + 1;
        }
    }
}

void rotacao_r(struct Node **root)
{
    struct Node *temp = (*root)->l;
    (*root)->l = temp->r;
    if ((*root)->l != NULL)
    {
        (*root)->l->up = (*root);
    }
    temp->up = (*root)->up;
    if ((*root)->up == NULL)
    {
        (*root) = temp;
    }
    else if ((*root) == (*root)->up->l)
    {
        (*root)->up->l = temp;
    }
    else
    {
        (*root)->up->r = temp;
    }
    temp->r = (*root);
    (*root)->up = temp;
}

void rotacao_l(struct Node **root)
{
    struct Node *temp = (*root)->r;
    (*root)->r = temp->l;
    if ((*root)->r != NULL)
    {
        (*root)->r->up = (*root);
    }
    temp->up = (*root)->up;
    if ((*root)->up == NULL)
    {
        (*root) = temp;
    }
    else if ((*root) == (*root)->up->l)
    {
        (*root)->up->l = temp;
    }
    else
    {
        (*root)->up->r = temp;
    }
    temp->l = (*root);
    (*root)->up = temp;
}

void balancear(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    balancear(&((*root)->l));
    balancear(&((*root)->r));
    int lh = altura((*root)->l);
    int rh = altura((*root)->r);
    if (abs(lh - rh) > 1)
    {
        if (lh > rh)
        {
            if (altura((*root)->l->l) >= altura((*root)->l->r))
            {
                rotacao_r(root);
            }
            else
            {
                rotacao_l(&((*root)->l));
                rotacao_r(root);
            }
        }
        else
        {
            if (altura((*root)->r->r) >= altura((*root)->r->l))
            {
                rotacao_l(root);
            }
            else
            {
                rotacao_r(&((*root)->r));
                rotacao_l(root);
            }
        }
    }
}

int main()
{
    struct Node *root = NULL;
    long cpf, telefone;
    char nome[50], data[15];
    int continua = 1, option = 0;

    while (continua)
    {
        printf("Digite a opçao que voce deseja: \n0 = Inserir Consulta\n1 = Remover Consulta\n");
        scanf("%i", &option);
        switch (option)
        {
        case 0:
            printf("Digite o CPF: ");
            scanf("%ld", &cpf);
            printf("Digite o nome: ");
            scanf("%s", nome);
            printf("Digite a data: ");
            scanf("%s", data);
            printf("Digite o telefone: ");
            scanf("%ld", &telefone);

            if (root == NULL)
            {
                root = (struct Node *)malloc(sizeof(struct Node));
                root->up = NULL;
                root->Cpf = cpf;
                strcpy(root->Nome, nome);
                strcpy(root->Data, data);
                root->Telefone = telefone;
                root->Peso = 0;
            }
            else
            {
                if (insert(cpf, nome, data, telefone, root)) balancear(root);
            }
            break;
        case 1:
            printf("Digite o CPF: ");
            scanf("%ld", &cpf);
            if (remove_p(cpf, root)) balancear(root);
            break;

        default:
            break;
        }
        printf("Deseja continuar? (1 para sim, 0 para não): ");
        scanf("%d", &continua);
    }

    return 0;
}