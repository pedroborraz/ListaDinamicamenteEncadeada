/* Programa de teste da biblioteca: reproduz os traces da Aula 06. */
#include <stdio.h>
#include <string.h>
#include "ListaDinEncad.h"

static struct tarefa mk(int codigo, const char *descricao, int prioridade) {
    struct tarefa t;
    t.codigo = codigo;
    strncpy(t.descricao, descricao, sizeof(t.descricao) - 1);
    t.descricao[sizeof(t.descricao) - 1] = '\0';
    t.prioridade = prioridade;
    return t;
}

static void imprime(const char *rotulo, ListaTarefas *li) {
    int i, n = tamanho_lista(li);
    struct tarefa t;
    printf("%-22s tam=%d: inicio ->", rotulo, n);
    for (i = 1; i <= n; i++) {
        busca_tarefa_pos(li, i, &t);
        printf(" (%d, %s, %d) ->", t.codigo, t.descricao, t.prioridade);
    }
    printf(" NULL\n");
}

int main(void) {
    ListaTarefas *li = cria_lista();
    struct tarefa t;

    printf("vazia=%d cheia=%d tamanho=%d\n", lista_vazia(li), lista_cheia(li), tamanho_lista(li));

    insere_tarefa_final(li, mk(12, "Estudar", 3));
    insere_tarefa_final(li, mk(23, "Comprar leite", 1));
    insere_tarefa_final(li, mk(16, "Lavar roupa", 2));
    imprime("apos 3 insercoes", li);

    insere_tarefa_inicio(li, mk(30, "Revisar", 3));
    imprime("insere_inicio(30)", li);

    remove_tarefa_inicio(li);
    insere_tarefa_final(li, mk(40, "Dormir", 4));
    imprime("insere_final(40)", li);

    remove_tarefa_final(li);
    imprime("remove_final", li);

    remove_tarefa(li, 23);
    imprime("remove_tarefa(23)", li);
    printf("remove_tarefa(99)=%d\n", remove_tarefa(li, 99));

    libera_lista(li);

    li = cria_lista();
    insere_tarefa_ordenada(li, mk(23, "Comprar leite", 3));
    insere_tarefa_ordenada(li, mk(16, "Lavar roupa", 1));
    insere_tarefa_ordenada(li, mk(19, "Comprar", 2));
    insere_tarefa_ordenada(li, mk(12, "Estudar", 1));
    insere_tarefa_ordenada(li, mk(40, "Dormir", 4));
    imprime("ordenada por prioridade", li);

    if (busca_tarefa_pos(li, 3, &t))
        printf("busca_pos(3) -> codigo %d\n", t.codigo);
    if (busca_tarefa_cod(li, 19, &t))
        printf("busca_cod(19) -> %s\n", t.descricao);
    printf("busca_cod(99)=%d\n", busca_tarefa_cod(li, 99, &t));
    printf("conta_prioridade(1)=%d\n", conta_tarefas_prioridade(li, 1));
    if (tarefa_mais_urgente(li, &t))
        printf("mais_urgente -> codigo %d\n", t.codigo);
    if (busca_tarefa_desc(li, "Comprar", &t))
        printf("busca_desc(\"Comprar\") -> codigo %d\n", t.codigo);

    libera_lista(li);

    li = cria_lista();
    insere_tarefa_final(li, mk(1, "prioridade 1", 1));
    insere_tarefa_final(li, mk(2, "prioridade 2", 2));
    insere_tarefa_final(li, mk(3, "outra prioridade 1", 1));
    insere_tarefa_final_prioridade(li, mk(4, "ultima prioridade 1", 1));
    imprime("insere apos ultima prioridade", li);

    insere_tarefa_final_prioridade(li, mk(5, "prioridade inexistente", 3));
    imprime("prioridade inexistente", li);

    libera_lista(li);

    li = cria_lista();
    insere_tarefa_final(li, mk(1, "prioridade 1", 1));
    insere_tarefa_final(li, mk(2, "prioridade 2", 2));
    insere_tarefa_final(li, mk(3, "outra prioridade 1", 1));
    insere_tarefa_final(li, mk(4, "prioridade 3", 3));
    printf("remove_prioridade(1)=%d\n", remove_tarefas_prioridade(li, 1));
    imprime("apos remover prioridade", li);
    printf("remove_prioridade(9)=%d\n", remove_tarefas_prioridade(li, 9));

    printf("inverte_lista=%d\n", inverte_lista(li));
    imprime("lista invertida", li);

    printf("remove_pos(2)=%d\n", remove_tarefa_pos(li, 2));
    imprime("remove posicao 2", li);
    printf("remove_pos(9)=%d\n", remove_tarefa_pos(li, 9));

    libera_lista(li);

    ListaTarefas *dst = cria_lista();
    ListaTarefas *src = cria_lista();
    insere_tarefa_final(dst, mk(10, "destino", 1));
    insere_tarefa_final(src, mk(20, "origem 1", 2));
    insere_tarefa_final(src, mk(30, "origem 2", 3));
    printf("mescla_tarefas=%d\n", mescla_tarefas(dst, src));
    imprime("destino apos mescla", dst);
    imprime("origem apos mescla", src);

    libera_lista(dst);
    libera_lista(src);
    return 0;
}
