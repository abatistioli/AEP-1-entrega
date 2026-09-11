/* =========================================================================
   PSE em Acao - Sistema de Planejamento e Acompanhamento de Acoes
   Tematica escolhida: Saude Mental e Inclusao (nivel COLETIVO)

   Disciplina: AEP - Engenharia de Software / Analise e Desenvolvimento
               de Sistemas - Unicesumar - 2 semestre - 2026
   Curso/Serie: ESOFT2S / ADSIS2S

   -------------------------------------------------------------------------
   OBSERVACAO SOBRE O ESTILO DO CODIGO:
   Todas as funcoes deste arquivo retornam "int" (nenhuma usa "void"),
   pois esse ainda nao e um tipo estudado em aula. Quando uma funcao nao
   precisa devolver um valor util, ela apenas "return 0;" ao final, do
   mesmo jeito que a funcao main() sempre devolve um inteiro.
   -------------------------------------------------------------------------
   IMPORTANTE - LIMITES ETICOS DO SISTEMA (conforme documento da AEP):
   Este programa NAO cadastra nome de aluno, diagnostico, laudo, prontuario
   ou qualquer dado clinico/individual. Ele trabalha apenas com ACOES
   COLETIVAS do Programa Saude na Escola (ex: rodas de conversa, capacitacao
   de professores, semanas de acolhimento), preservando a privacidade dos
   estudantes. O sistema NAO realiza diagnostico, triagem ou prescricao.
   ========================================================================= */

#include <stdio.h>
#include <string.h>

#define MAX_ACOES 50
#define TAM_TEXTO 60
#define TAM_DATA 11
#define TAM_SITUACAO 15

/* Situacao da acao (guardada como um numero inteiro) */
#define PLANEJADA 0
#define REALIZADA 1
#define CANCELADA 2

/* ---------------------------------------------------------------------
   Struct que representa uma acao coletiva do PSE.
   Nenhum campo identifica um aluno individualmente.
   --------------------------------------------------------------------- */
typedef struct {
    int codigo;
    char escola[TAM_TEXTO];
    char tema[TAM_TEXTO];
    char dataPrevista[TAM_DATA];
    char publicoAlvo[TAM_TEXTO];   /* ex: "Turmas do 2 ano - EM"      */
    char responsavel[TAM_TEXTO];   /* ex: "Coordenacao Pedagogica"    */
    int qtdPrevista;
    int qtdEfetiva;                /* so preenchido quando REALIZADA  */
    int situacao;                  /* PLANEJADA / REALIZADA / CANCELADA */
} Acao;

/* ---------------------- Prototipos das funcoes -------------------------
   Todas retornam int (sem uso de void), conforme explicado acima.
   ------------------------------------------------------------------- */
int exibirMenu();
int limparBufferEntrada();
int lerInteiro(const char *mensagem);
int lerTexto(const char *mensagem, char destino[], int tamanho);
int existeCodigo(Acao acoes[], int total, int codigo);
int obterSituacaoTexto(int situacao, char destino[]);

int cadastrarAcao(Acao acoes[], int *total);
int listarAcoes(Acao acoes[], int total);
int pesquisarAcoes(Acao acoes[], int total);
int atualizarSituacao(Acao acoes[], int total);
int gerarResumo(Acao acoes[], int total);

/* ========================================================================
   FUNCAO PRINCIPAL
   Controla o menu em loop ate o usuario optar por encerrar (opcao 0).
   ======================================================================== */
int main() {
    Acao acoes[MAX_ACOES];
    int total = 0;
    int opcao;

    printf("=====================================================\n");
    printf(" PSE EM ACAO - Saude Mental e Inclusao (dados ficticios)\n");
    printf("=====================================================\n");

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                cadastrarAcao(acoes, &total);
                break;
            case 2:
                listarAcoes(acoes, total);
                break;
            case 3:
                pesquisarAcoes(acoes, total);
                break;
            case 4:
                atualizarSituacao(acoes, total);
                break;
            case 5:
                gerarResumo(acoes, total);
                break;
            case 0:
                printf("\nEncerrando o sistema. Ate a proxima!\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida. Escolha uma opcao do menu.\n");
        }

    } while (opcao != 0);

    return 0;
}

/* ========================================================================
   exibirMenu
   Mostra as opcoes disponiveis ao usuario.
   ======================================================================== */
int exibirMenu() {
    printf("\n----------------- MENU PRINCIPAL -----------------\n");
    printf("1 - Cadastrar acao de saude mental/inclusao\n");
    printf("2 - Listar todas as acoes\n");
    printf("3 - Pesquisar acao (codigo, escola ou tema)\n");
    printf("4 - Atualizar situacao da acao\n");
    printf("5 - Gerar resumo geral\n");
    printf("0 - Sair\n");
    printf("----------------------------------------------------\n");
    return 0;
}

/* ========================================================================
   FUNCOES AUXILIARES DE ENTRADA (evitam repetir validacao em todo lugar)
   ======================================================================== */

/* Limpa o buffer do teclado apos leituras com scanf, evitando bugs no
   proximo fgets/scanf (residuo do ENTER). */
int limparBufferEntrada() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
    return 0;
}

/* Le um inteiro validando que a entrada e realmente numerica.
   Repete a pergunta ate o usuario digitar um numero valido. */
int lerInteiro(const char *mensagem) {
    int valor;
    int resultado;

    printf("%s", mensagem);
    resultado = scanf("%d", &valor);

    while (resultado != 1) {
        limparBufferEntrada();
        printf("[ERRO] Digite apenas numeros. %s", mensagem);
        resultado = scanf("%d", &valor);
    }
    limparBufferEntrada();
    return valor;
}

/* Le uma linha de texto (nao aceita campo vazio). */
int lerTexto(const char *mensagem, char destino[], int tamanho) {
    int valido = 0;

    do {
        printf("%s", mensagem);
        fgets(destino, tamanho, stdin);

        /* remove o '\n' final deixado pelo fgets */
        destino[strcspn(destino, "\n")] = '\0';

        if (strlen(destino) == 0) {
            printf("[ERRO] Este campo e obrigatorio e nao pode ficar vazio.\n");
        } else {
            valido = 1;
        }
    } while (valido == 0);

    return 0;
}

/* Verifica se um codigo ja esta cadastrado (evita duplicidade).
   Corresponde a funcao existeCodigo() referenciada no pseudocodigo
   e no fluxograma detalhado da 1a entrega. */
int existeCodigo(Acao acoes[], int total, int codigo) {
    int i;
    int encontrado = 0;

    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codigo) {
            encontrado = 1;
        }
    }
    return encontrado; /* 1 = existe, 0 = nao existe */
}

/* Preenche "destino" com o texto correspondente ao codigo de situacao.
   Em vez de retornar um texto diretamente, a funcao copia o resultado
   para o vetor de caracteres recebido por parametro (tecnica identica
   a usada em lerTexto), o que evita o uso de ponteiros de retorno. */
int obterSituacaoTexto(int situacao, char destino[]) {
    switch (situacao) {
        case PLANEJADA:
            strcpy(destino, "Planejada");
            break;
        case REALIZADA:
            strcpy(destino, "Realizada");
            break;
        case CANCELADA:
            strcpy(destino, "Cancelada");
            break;
        default:
            strcpy(destino, "Desconhecida");
    }
    return 0;
}

/* ========================================================================
   cadastrarAcao
   Requisito 1 e 6 do escopo: cadastro com validacao de codigo repetido,
   campos obrigatorios vazios e quantidade negativa.

   Esta funcao implementa, passo a passo, o pseudocodigo "CadastrarAcao"
   e o fluxograma detalhado (Figura 2) apresentados na 1a entrega da AEP.
   Os comentarios abaixo fazem a correspondencia direta com cada bloco
   do pseudocodigo em Portugol, para facilitar a conferencia entre os
   artefatos.
   ======================================================================== */
int cadastrarAcao(Acao acoes[], int *total) {
    Acao nova;

    printf("\n--- CADASTRAR NOVA ACAO (Saude Mental / Inclusao) ---\n");

    /* se (total_acoes >= LIMITE_MAXIMO) entao ... */
    if (*total >= MAX_ACOES) {
        printf("[ERRO] Limite de acoes atingido (maximo de %d acoes).\n", MAX_ACOES);
        return 0;
    }

    /* repita
           leia(codigo)
           se (existeCodigo(codigo)) entao escreva(...)
       ate (nao existeCodigo(codigo)) */
    do {
        nova.codigo = lerInteiro("Codigo da acao (numero unico): ");
        if (existeCodigo(acoes, *total, nova.codigo)) {
            printf("[ERRO] Codigo ja cadastrado. Escolha outro numero.\n");
        }
    } while (existeCodigo(acoes, *total, nova.codigo));

    /* leia(escola, tema, data_prevista, publico_alvo, responsavel) */
    lerTexto("Escola: ", nova.escola, TAM_TEXTO);
    lerTexto("Tema (ex: Saude Mental, Neurodivergencia, Inclusao): ", nova.tema, TAM_TEXTO);
    lerTexto("Data prevista (dd/mm/aaaa): ", nova.dataPrevista, TAM_DATA);
    lerTexto("Publico-alvo (ex: Turmas 2 ano EM - dado coletivo): ", nova.publicoAlvo, TAM_TEXTO);
    lerTexto("Responsavel (ex: Coordenacao Pedagogica): ", nova.responsavel, TAM_TEXTO);

    /* repita
           leia(quantidade_prevista)
           se (quantidade_prevista <= 0) entao escreva(...)
       ate (quantidade_prevista > 0) */
    do {
        nova.qtdPrevista = lerInteiro("Quantidade prevista de participantes: ");
        if (nova.qtdPrevista <= 0) {
            printf("[ERRO] Quantidade invalida. Informe um numero maior que zero.\n");
        }
    } while (nova.qtdPrevista <= 0);

    /* situacao <- "Planejada" ; quantidade_efetiva <- 0 */
    nova.situacao = PLANEJADA;
    nova.qtdEfetiva = 0;

    /* armazena a nova acao no vetor de acoes ; total_acoes <- total_acoes + 1 */
    acoes[*total] = nova;
    (*total)++;

    /* escreva("Acao cadastrada com sucesso") */
    printf("\n[OK] Acao cadastrada com sucesso! Total de acoes: %d\n", *total);
    return 0;
}

/* ========================================================================
   listarAcoes
   Requisito 2 do escopo: listar de forma organizada.
   ======================================================================== */
int listarAcoes(Acao acoes[], int total) {
    int i;
    char textoSituacao[TAM_SITUACAO];

    printf("\n--- LISTA DE ACOES CADASTRADAS ---\n");

    if (total == 0) {
        printf("Nenhuma acao cadastrada ate o momento.\n");
        return 0;
    }

    for (i = 0; i < total; i++) {
        obterSituacaoTexto(acoes[i].situacao, textoSituacao);

        printf("\nCodigo.........: %d\n", acoes[i].codigo);
        printf("Escola.........: %s\n", acoes[i].escola);
        printf("Tema...........: %s\n", acoes[i].tema);
        printf("Data prevista..: %s\n", acoes[i].dataPrevista);
        printf("Publico-alvo...: %s\n", acoes[i].publicoAlvo);
        printf("Responsavel....: %s\n", acoes[i].responsavel);
        printf("Qtd prevista...: %d\n", acoes[i].qtdPrevista);
        printf("Qtd efetiva....: %d\n", acoes[i].qtdEfetiva);
        printf("Situacao.......: %s\n", textoSituacao);
        printf("-------------------------------------------\n");
    }

    return 0;
}

/* ========================================================================
   pesquisarAcoes
   Requisito 3 do escopo: pesquisa por codigo, escola ou tema.
   ======================================================================== */
int pesquisarAcoes(Acao acoes[], int total) {
    int opcao;
    int encontrou = 0;
    int i;
    char textoSituacao[TAM_SITUACAO];

    if (total == 0) {
        printf("\nNenhuma acao cadastrada para pesquisar.\n");
        return 0;
    }

    printf("\n--- PESQUISAR ACAO ---\n");
    printf("1 - Por codigo\n");
    printf("2 - Por escola\n");
    printf("3 - Por tema\n");
    opcao = lerInteiro("Escolha o tipo de pesquisa: ");

    if (opcao == 1) {
        int codigoBusca = lerInteiro("Digite o codigo: ");
        for (i = 0; i < total; i++) {
            if (acoes[i].codigo == codigoBusca) {
                obterSituacaoTexto(acoes[i].situacao, textoSituacao);
                printf("\nAcao encontrada:\n");
                printf("Codigo: %d | Escola: %s | Tema: %s | Situacao: %s\n",
                       acoes[i].codigo, acoes[i].escola, acoes[i].tema, textoSituacao);
                encontrou = 1;
            }
        }
    } else if (opcao == 2) {
        char escolaBusca[TAM_TEXTO];
        lerTexto("Digite o nome (ou parte) da escola: ", escolaBusca, TAM_TEXTO);
        for (i = 0; i < total; i++) {
            if (strstr(acoes[i].escola, escolaBusca) != NULL) {
                obterSituacaoTexto(acoes[i].situacao, textoSituacao);
                printf("\nCodigo: %d | Escola: %s | Tema: %s | Situacao: %s\n",
                       acoes[i].codigo, acoes[i].escola, acoes[i].tema, textoSituacao);
                encontrou = 1;
            }
        }
    } else if (opcao == 3) {
        char temaBusca[TAM_TEXTO];
        lerTexto("Digite o tema (ou parte): ", temaBusca, TAM_TEXTO);
        for (i = 0; i < total; i++) {
            if (strstr(acoes[i].tema, temaBusca) != NULL) {
                obterSituacaoTexto(acoes[i].situacao, textoSituacao);
                printf("\nCodigo: %d | Escola: %s | Tema: %s | Situacao: %s\n",
                       acoes[i].codigo, acoes[i].escola, acoes[i].tema, textoSituacao);
                encontrou = 1;
            }
        }
    } else {
        printf("[ERRO] Opcao de pesquisa invalida.\n");
        return 0;
    }

    if (!encontrou) {
        printf("\nNenhum registro encontrado para essa pesquisa.\n");
    }

    return 0;
}

/* ========================================================================
   atualizarSituacao
   Requisito 4 do escopo: atualizar para planejada/realizada/cancelada,
   registrando a quantidade efetiva quando for "realizada".
   ======================================================================== */
int atualizarSituacao(Acao acoes[], int total) {
    int codigoBusca, i, indice = -1;
    int novaSituacao;
    char textoSituacao[TAM_SITUACAO];

    if (total == 0) {
        printf("\nNenhuma acao cadastrada para atualizar.\n");
        return 0;
    }

    printf("\n--- ATUALIZAR SITUACAO DE UMA ACAO ---\n");
    codigoBusca = lerInteiro("Digite o codigo da acao: ");

    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codigoBusca) {
            indice = i;
        }
    }

    if (indice == -1) {
        printf("[ERRO] Nenhuma acao encontrada com o codigo %d.\n", codigoBusca);
        return 0;
    }

    obterSituacaoTexto(acoes[indice].situacao, textoSituacao);
    printf("Acao atual: %s (situacao: %s)\n", acoes[indice].tema, textoSituacao);

    printf("Nova situacao:\n");
    printf("1 - Planejada\n");
    printf("2 - Realizada\n");
    printf("3 - Cancelada\n");
    novaSituacao = lerInteiro("Escolha: ");

    switch (novaSituacao) {
        case 1:
            acoes[indice].situacao = PLANEJADA;
            printf("[OK] Situacao atualizada para PLANEJADA.\n");
            break;
        case 2:
            acoes[indice].situacao = REALIZADA;
            do {
                acoes[indice].qtdEfetiva = lerInteiro("Quantidade efetiva de participantes: ");
                if (acoes[indice].qtdEfetiva < 0) {
                    printf("[ERRO] A quantidade nao pode ser negativa.\n");
                }
            } while (acoes[indice].qtdEfetiva < 0);
            printf("[OK] Situacao atualizada para REALIZADA.\n");
            break;
        case 3:
            acoes[indice].situacao = CANCELADA;
            acoes[indice].qtdEfetiva = 0;
            printf("[OK] Situacao atualizada para CANCELADA.\n");
            break;
        default:
            printf("[ERRO] Opcao invalida. Nenhuma alteracao foi feita.\n");
    }

    return 0;
}

/* ========================================================================
   gerarResumo
   Requisito 5 do escopo: resumo geral com quantidade de acoes por
   situacao, total de participantes e percentual de participacao.
   ======================================================================== */
int gerarResumo(Acao acoes[], int total) {
    int i;
    int qtdPlanejadas = 0, qtdRealizadas = 0, qtdCanceladas = 0;
    int totalPrevistoRealizadas = 0, totalEfetivo = 0;
    float percentual = 0.0f;

    printf("\n--- RESUMO GERAL DO PROGRAMA (Saude Mental / Inclusao) ---\n");

    if (total == 0) {
        printf("Nenhuma acao cadastrada ate o momento.\n");
        return 0;
    }

    for (i = 0; i < total; i++) {
        switch (acoes[i].situacao) {
            case PLANEJADA:
                qtdPlanejadas++;
                break;
            case REALIZADA:
                qtdRealizadas++;
                totalPrevistoRealizadas += acoes[i].qtdPrevista;
                totalEfetivo += acoes[i].qtdEfetiva;
                break;
            case CANCELADA:
                qtdCanceladas++;
                break;
        }
    }

    if (totalPrevistoRealizadas > 0) {
        percentual = (totalEfetivo * 100.0f) / totalPrevistoRealizadas;
    }

    printf("Total de acoes cadastradas.......: %d\n", total);
    printf("Acoes planejadas..................: %d\n", qtdPlanejadas);
    printf("Acoes realizadas..................: %d\n", qtdRealizadas);
    printf("Acoes canceladas..................: %d\n", qtdCanceladas);
    printf("Total de participantes (realizadas): %d\n", totalEfetivo);

    if (totalPrevistoRealizadas > 0) {
        printf("Percentual de participacao........: %.2f%%\n", percentual);
    } else {
        printf("Percentual de participacao........: sem acoes realizadas ainda\n");
    }

    return 0;
}
