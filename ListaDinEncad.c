/* ============================================================
   ListaDinEncad.c
   Estrutura de Dados I - UFPB - Aula 06
   Implementacao da lista dinamica encadeada.

   Tudo o que deve ficar oculto do usuario da biblioteca esta
   aqui: a definicao de struct elemento e o corpo das funcoes.

   Diferencas em relacao ao codigo do livro:
   - o ponteiro ant e inicializado com NULL, o que evita o aviso
     -Wmaybe-uninitialized quando se compila com otimizacao;
   - as consultas devolvem -1 para lista invalida, conforme a
     convencao do repositorio.
   ============================================================ */

#include <stdlib.h>
#include <string.h>
#include "ListaDinEncad.h"

/* Cada elemento guarda os dados e o endereco do proximo elemento.
   O ultimo elemento aponta para NULL. */
struct elemento {
    struct tarefa dados;
    struct elemento *prox;
};
typedef struct elemento Elem;

/* ------------------------------------------------------------
   Criacao e destruicao
   ------------------------------------------------------------ */

ListaTarefas* cria_lista(void) {
    ListaTarefas* li = (ListaTarefas*) malloc(sizeof(ListaTarefas));
    if (li != NULL)
        *li = NULL;               /* lista vazia: o inicio aponta para NULL */
    return li;
}

void libera_lista(ListaTarefas* li) {
    if (li != NULL) {
        Elem* no;
        while ((*li) != NULL) {
            no = *li;
            *li = (*li)->prox;    /* avanca ANTES de liberar */
            free(no);
        }
        free(li);                 /* libera o bloco do inicio */
    }
}

/* ------------------------------------------------------------
   Informacoes de estado
   ------------------------------------------------------------ */
int tamanho_lista(ListaTarefas* li) {
    if (li == NULL)
        return -1;
    int cont = 0;
    Elem* no = *li;
    while (no != NULL) {          /* nao ha campo qtd: e preciso percorrer */
        cont++;
        no = no->prox;
    }
    return cont;
}

int lista_cheia(ListaTarefas* li) {
    if (li == NULL)
        return -1;
    return 0;                     /* so falta memoria quando o malloc falha */
}

int lista_vazia(ListaTarefas* li) {
    if (li == NULL)
        return -1;
    if (*li == NULL)
        return 1;
    return 0;
}

/* ------------------------------------------------------------
   Insercao
   ------------------------------------------------------------ */
int insere_tarefa_inicio(ListaTarefas* li, struct tarefa t) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;     /* falta de memoria: lista cheia */
    no->dados = t;
    no->prox = (*li);             /* 1o: liga o novo no ao antigo primeiro */
    *li = no;                     /* 2o: so entao altera o inicio */
    return 1;
}

int insere_tarefa_final(ListaTarefas* li, struct tarefa t) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = t;
    no->prox = NULL;              /* o novo no sera o ultimo */
    if ((*li) == NULL) {          /* lista vazia: insere no inicio */
        *li = no;
    } else {
        Elem* aux = *li;
        while (aux->prox != NULL) /* para NO ultimo, e nao depois dele */
            aux = aux->prox;
        aux->prox = no;
    }
    return 1;
}

int insere_tarefa_ordenada(ListaTarefas* li, struct tarefa t) {
    if (li == NULL) return 0;
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) return 0;
    no->dados = t;
    if ((*li) == NULL) {          /* lista vazia: insere no inicio */
        no->prox = NULL;
        *li = no;
        return 1;
    } else {
        Elem *ant = NULL, *atual = *li;
        /* procura o primeiro elemento com prioridade maior ou igual */
        while (atual != NULL &&
               atual->dados.prioridade < t.prioridade) {
            ant = atual;
            atual = atual->prox;
        }
        if (atual == *li) {       /* insere no inicio */
            no->prox = (*li);
            *li = no;
        } else {                  /* insere entre ant e atual */
            no->prox = atual;
            ant->prox = no;
        }
        return 1;
    }
}

/* ------------------------------------------------------------
   Remocao
   ------------------------------------------------------------ */

int remove_tarefa_inicio(ListaTarefas* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL)            /* lista vazia */
        return 0;
    Elem *no = *li;
    *li = no->prox;               /* religa antes do free */
    free(no);
    return 1;
}

int remove_tarefa_final(ListaTarefas* li) {
    if (li == NULL) return 0;
    if ((*li) == NULL)            /* lista vazia */
        return 0;
    Elem *ant = NULL, *no = *li;
    while (no->prox != NULL) {
        ant = no;
        no = no->prox;
    }
    if (no == (*li))              /* unico elemento: a lista fica vazia */
        *li = no->prox;
    else
        ant->prox = no->prox;     /* o penultimo passa a apontar para NULL */
    free(no);
    return 1;
}

int remove_tarefa(ListaTarefas* li, int codigo) {
    if (li == NULL) return 0;
    if ((*li) == NULL)            /* lista vazia */
        return 0;
    Elem *ant = NULL, *no = *li;
    while (no != NULL && no->dados.codigo != codigo) {
        ant = no;
        no = no->prox;
    }
    if (no == NULL)               /* elemento nao encontrado */
        return 0;
    if (no == *li)                /* remove o primeiro */
        *li = no->prox;
    else
        ant->prox = no->prox;     /* contorna o no removido */
    free(no);
    return 1;
}

/* ------------------------------------------------------------
   Busca
   ------------------------------------------------------------ */

int busca_tarefa_pos(ListaTarefas* li, int pos, struct tarefa *t) {
    if (li == NULL || pos <= 0)
        return 0;
    Elem *no = *li;
    int i = 1;
    while (no != NULL && i < pos) { /* nao ha indice: percorre ate a posicao */
        no = no->prox;
        i++;
    }
    if (no == NULL)               /* posicao maior que o tamanho */
        return 0;
    else {
        *t = no->dados;
        return 1;
    }
}

int busca_tarefa_cod(ListaTarefas* li, int codigo, struct tarefa *t) {
    if (li == NULL)
        return 0;
    Elem *no = *li;
    while (no != NULL && no->dados.codigo != codigo)
        no = no->prox;
    if (no == NULL)               /* elemento nao encontrado */
        return 0;
    else {
        *t = no->dados;
        return 1;
    }
}

int conta_tarefas_prioridade(ListaTarefas* li, int prioridade) {
    if (li == NULL)
        return -1; // questao pediu para retornar -1 se a lista for NULL
    int count = 0;
    Elem *no = *li;
    while (no != NULL) {
        if (no->dados.prioridade == prioridade)
            count++;
        no = no->prox;
    }
    return count;
}

int tarefa_mais_urgente(ListaTarefas* li, struct tarefa *t) {
    if (li == NULL || *li == NULL)
        return 0; // Lista inválida ou vazia
    Elem *no = *li;
    struct tarefa mais_urgente = no->dados;
    while (no != NULL) {
        if (no->dados.prioridade < mais_urgente.prioridade) { // Busca pelo de menor valor de prioridade
            mais_urgente = no->dados;
        }
        no = no->prox;
    }

    *t = mais_urgente;
    return 1; // Sucesso
}

int busca_tarefa_desc(ListaTarefas* li, char *texto, struct tarefa *t) {
    if (li == NULL || *li == NULL || texto == NULL || t == NULL) // verifica se a lista esta criada, se ela é vazia, se o texto é vazio, se a struct é vazia
        return 0;

    Elem *no = *li;
    while (no != NULL) {
        if (strstr(no->dados.descricao, texto) != NULL) { // strstr retorna o ponteiro para a primeira posição da substring localizada
            *t = no->dados;
            return 1;
        }
        no = no->prox;
    }

    return 0; // Não localizou a string
}

int insere_tarefa_final_prioridade(ListaTarefas* li, struct tarefa t) {
    if (li == NULL) return 0; // lista invalida
    Elem* novo = (Elem*) malloc(sizeof(Elem)); // cria o no que armazenara a nova tarefa
    if (novo == NULL) return 0;  // falha ao alocar memoria para o no
    novo->dados = t; // armazena os dados recebidos de t
    novo->prox = NULL; // inicialmente o no nao aponta para nenhum outro elemento

    Elem* aux = *li; // ponteiro usado para pecorrer cada no comecando pelo primeiro
    Elem* ultima_prioridade = NULL; // ultimo no encontrado com a mesma prioridade de t
    Elem* ultimo = NULL; // ultimo no da lista usado se a prioridade de t nao existir

    while (aux != NULL) { // continua enquanto houver um no para analisar
        ultimo = aux; // atualiza o ultimo no visitado ao final, sera o ultimo da lista

        if (aux->dados.prioridade == t.prioridade) {
            ultima_prioridade = aux; // substitui o registro para manter a ultima ocorrencia
        }

        aux = aux->prox; // avanca para o proximo no
    }

    if (ultima_prioridade != NULL) {
        novo->prox = ultima_prioridade->prox; // liga o novo no ao elemento que vinha depois
        ultima_prioridade->prox = novo; // coloca o novo no logo apos a ultima prioridade igual
    } else if (ultimo == NULL) {
        *li = novo; // a lista estava vazia entao insere no inicio
    } else {
        ultimo->prox = novo; // nenhuma prioridade igual insere ao final da lista
    }

    return 1; // sucesso
}

int remove_tarefas_prioridade(ListaTarefas* li, int prioridade) {
    if (li == NULL) return -1;
    Elem *ant = NULL, *no = *li;
    int removidas = 0; //quantidade de nos removidos

    while (no != NULL) {
        if (no->dados.prioridade == prioridade) {
            Elem *removido = no; // guarda o no 
            no = no->prox; // avanca o no

            if (ant == NULL)
                *li = no; // remove o primeiro no da lista
            else
                ant->prox = no; // contorna o no removido
            free(removido);
            removidas++;
        } else {
            ant = no;
            no = no->prox;
        }
    }
    return removidas;
}

int inverte_lista(ListaTarefas* li) {
    if (li == NULL) return 0;
    Elem *ant = NULL, *no = *li;
    while (no != NULL) {
        Elem *prox = no->prox;
        no->prox = ant; // inverte pra onde o no atual aponta
        ant = no;
        no = prox;
    }
    *li = ant; // atualiza o inicio da lista
    return 1;
}

int remove_tarefa_pos(ListaTarefas* li, int pos) {
    if (li == NULL || *li == NULL || pos <= 0) return 0;
    Elem *ant = NULL, *no = *li;
    int i = 1;
    while (no != NULL && i < pos) {
        ant = no; //guarda o no
        no = no->prox; // avanca 
        i++; // guarda a posicao
    }

    if (no == NULL) return 0; // posicao nao encontrada
    if (ant == NULL)
        *li = no->prox; //remove o primeiro no
    else
        ant->prox = no->prox; // contorna o no removido
    free(no); // libera o no retirado
    return 1;
}

int mescla_tarefas(ListaTarefas* dst, ListaTarefas* src) {
    if (dst == NULL || src == NULL) return -1;
    if (dst == src || *src == NULL) return 0; // nada para mesclar

    Elem *no = *src; // varre o de fonte
    int transferidas = 0; // quantidade de nos transferidos
    while (no != NULL) {
        transferidas++;
        if (no->prox == NULL) break; //encontrou o ultimo no
        no = no->prox;
    }

    if (*dst == NULL)
        *dst = *src; // destino vazio src vira o inicio
    else {
        Elem *ultimo = *dst;
        while (ultimo->prox != NULL)
            ultimo = ultimo->prox;
        ultimo->prox = *src; //liga dst ao primeiro no de src
    }

    *src = NULL; // src fica vazio mas continua valido
    return transferidas;
}
