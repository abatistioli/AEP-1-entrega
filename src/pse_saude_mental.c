/* ==========================================================================
   PSE em Acao - Sistema de Planejamento e Acompanhamento de Acoes
   Tema: Saude Mental e Inclusao (informacoes COLETIVAS e dados ficticios)

   AEP - 2o semestre de 2026 - Analise e Desenvolvimento de Sistemas
   Unicesumar - turma ADSIS2B - entrega final
   
   Limites eticos: o programa nao registra nome, diagnostico, laudo,
   prontuario ou qualquer dado individual de estudantes e nao faz
   diagnostico, triagem ou recomendacao de tratamento.

   ========================================================================== */

#include <stdio.h>
#include <string.h>

#define MAX_ACOES 50
#define TAM_TEXTO 60
#define TAM_DATA 11
#define TAM_LINHA 256
#define TAM_SITUACAO 15

#define PLANEJADA 0
#define REALIZADA 1
#define CANCELADA 2

typedef struct {
    int codigo;
    char escola[TAM_TEXTO];
    char tema[TAM_TEXTO];
    char dataPrevista[TAM_DATA];
    char publicoAlvo[TAM_TEXTO];   /* sempre coletivo, ex: "Turmas do 2o ano" */
    char responsavel[TAM_TEXTO];   /* ex: "Coordenacao Pedagogica" */
    int qtdPrevista;
    int qtdEfetiva;                /* so e usada quando a acao foi REALIZADA */
    int situacao;
} Acao;

int exibirMenu();
int limparBufferEntrada();
int lerInteiro(const char *mensagem);
int lerTexto(const char *mensagem, char destino[], int tamanho);
int lerData(const char *mensagem, char destino[]);
int dataValida(const char *data);
int paraMinusculo(char texto[]);
int contemTexto(const char *texto, const char *busca);
int obterSituacaoTexto(int situacao, char destino[]);
int buscarIndice(Acao acoes[], int total, int codigo);
int existeCodigo(Acao acoes[], int total, int codigo);
int exibirAcao(Acao acao);
int adicionarAcaoDemo(Acao acoes[], int *total, int codigo, const char *escola,
                      const char *tema, const char *data, const char *publico,
                      const char *responsavel, int prevista, int situacao,
                      int efetiva);

int cadastrarAcao(Acao acoes[], int *total);
int listarAcoes(Acao acoes[], int total);
int pesquisarAcoes(Acao acoes[], int total);
int atualizarSituacao(Acao acoes[], int total);
int gerarResumo(Acao acoes[], int total, const char *filtroEscola);
int resumoPorEscola(Acao acoes[], int total);
int carregarDadosDemonstracao(Acao acoes[], int *total);

/* ========================================================================
   main: menu controlado por do...while ate o usuario escolher 0 (Sair)
   ======================================================================== */
int main() {
    Acao acoes[MAX_ACOES];
    int total = 0;
    int opcao;

    printf("=====================================================\n");
    printf(" PSE EM ACAO - Saude Mental e Inclusao \n");
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
                gerarResumo(acoes, total, "");
                break;
            case 6:
                resumoPorEscola(acoes, total);
                break;
            case 7:
                carregarDadosDemonstracao(acoes, &total);
                break;
            case 0:
                printf("\nEncerrando o sistema. Ate a proxima!\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida. Escolha uma opcao do menu.\n");
        }
    } while (opcao != 0);

    printf("Pressione ENTER para fechar...");
    limparBufferEntrada();
    return 0;
}

int exibirMenu() {
    printf("\n----------------- MENU PRINCIPAL -----------------\n");
    printf("1 - Cadastrar acao\n");
    printf("2 - Listar todas as acoes\n");
    printf("3 - Pesquisar acao (codigo, escola ou tema)\n");
    printf("4 - Atualizar situacao e registrar participantes\n");
    printf("5 - Resumo geral\n");
    printf("6 - Resumo por escola\n");
    printf("7 - Carregar dados ficticios de demonstracao\n");
    printf("0 - Sair\n");
    printf("----------------------------------------------------\n");
    return 0;
}

/* ========================================================================
   Entrada de dados com validacao
   ======================================================================== */

/* Descarta o que sobrou na linha digitada (ate o ENTER). */
int limparBufferEntrada() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
    return 0;
}

/* Repete a pergunta ate o usuario digitar somente um numero inteiro. */
int lerInteiro(const char *mensagem) {
    int valor = 0;
    int c;
    int resultado;
    int valido = 0;

    do {
        printf("%s", mensagem);
        resultado = scanf("%d", &valor);
        if (resultado == EOF) {
            return 0;
        }
        if (resultado == 1) {
            c = getchar();
            while (c == ' ') {
                c = getchar();
            }
            if (c == '\n' || c == EOF) {
                valido = 1;
            } else {
                limparBufferEntrada();
            }
        } else {
            limparBufferEntrada();
        }
        if (valido == 0) {
            printf("[ERRO] Digite apenas numeros inteiros.\n");
        }
    } while (valido == 0);

    return valor;
}

/* Le um texto obrigatorio. Recusa campo vazio e texto maior que o campo
   (o excedente e descartado para nao "vazar" para a proxima leitura). */
int lerTexto(const char *mensagem, char destino[], int tamanho) {
    char linha[TAM_LINHA];
    int i, tamanhoLido;
    int temConteudo;
    int valido = 0;

    do {
        printf("%s", mensagem);
        linha[0] = '\0';
        fgets(linha, TAM_LINHA, stdin);
        if (strchr(linha, '\n') == NULL) {
            limparBufferEntrada();
        }
        linha[strcspn(linha, "\n")] = '\0';

        tamanhoLido = strlen(linha);
        temConteudo = 0;
        for (i = 0; i < tamanhoLido; i++) {
            if (linha[i] != ' ') {
                temConteudo = 1;
            }
        }

        if (temConteudo == 0) {
            printf("[ERRO] Este campo e obrigatorio e nao pode ficar vazio.\n");
        } else if (tamanhoLido >= tamanho) {
            printf("[ERRO] Texto muito longo. Use no maximo %d caracteres.\n", tamanho - 1);
        } else {
            strcpy(destino, linha);
            valido = 1;
        }
    } while (valido == 0);

    return 0;
}

/* Aceita somente datas no formato dd/mm/aaaa (dia 01-31, mes 01-12). */
int dataValida(const char *data) {
    int i, dia, mes;

    if (strlen(data) != 10) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (data[i] != '/') {
                return 0;
            }
        } else if (data[i] < '0' || data[i] > '9') {
            return 0;
        }
    }
    dia = (data[0] - '0') * 10 + (data[1] - '0');
    mes = (data[3] - '0') * 10 + (data[4] - '0');
    if (dia < 1 || dia > 31 || mes < 1 || mes > 12) {
        return 0;
    }
    return 1;
}

int lerData(const char *mensagem, char destino[]) {
    int valida = 0;

    do {
        lerTexto(mensagem, destino, TAM_DATA);
        if (dataValida(destino) == 1) {
            valida = 1;
        } else {
            printf("[ERRO] Data invalida. Use o formato dd/mm/aaaa (ex: 15/09/2026).\n");
        }
    } while (valida == 0);

    return 0;
}

/* ========================================================================
   Funcoes auxiliares de busca e texto
   ======================================================================== */

int paraMinusculo(char texto[]) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        if (texto[i] >= 'A' && texto[i] <= 'Z') {
            texto[i] = texto[i] + 32;
        }
    }
    return 0;
}

/* Retorna 1 se "busca" aparece dentro de "texto", sem diferenciar
   maiusculas de minusculas. */
int contemTexto(const char *texto, const char *busca) {
    char textoMin[TAM_TEXTO];
    char buscaMin[TAM_TEXTO];

    strcpy(textoMin, texto);
    strcpy(buscaMin, busca);
    paraMinusculo(textoMin);
    paraMinusculo(buscaMin);

    if (strstr(textoMin, buscaMin) != NULL) {
        return 1;
    }
    return 0;
}

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

/* Retorna a posicao da acao no vetor ou -1 se o codigo nao existe. */
int buscarIndice(Acao acoes[], int total, int codigo) {
    int i;
    int indice = -1;

    for (i = 0; i < total; i++) {
        if (acoes[i].codigo == codigo) {
            indice = i;
        }
    }
    return indice;
}

int existeCodigo(Acao acoes[], int total, int codigo) {
    if (buscarIndice(acoes, total, codigo) != -1) {
        return 1;
    }
    return 0;
}

int exibirAcao(Acao acao) {
    char textoSituacao[TAM_SITUACAO];

    obterSituacaoTexto(acao.situacao, textoSituacao);
    printf("\nCodigo.........: %d\n", acao.codigo);
    printf("Escola.........: %s\n", acao.escola);
    printf("Tema...........: %s\n", acao.tema);
    printf("Data prevista..: %s\n", acao.dataPrevista);
    printf("Publico-alvo...: %s\n", acao.publicoAlvo);
    printf("Responsavel....: %s\n", acao.responsavel);
    printf("Qtd prevista...: %d\n", acao.qtdPrevista);
    printf("Qtd efetiva....: %d\n", acao.qtdEfetiva);
    printf("Situacao.......: %s\n", textoSituacao);
    printf("-------------------------------------------\n");
    return 0;
}

/* ========================================================================
   Funcionalidades do menu
   ======================================================================== */

int cadastrarAcao(Acao acoes[], int *total) {
    Acao nova;

    printf("\n--- CADASTRAR NOVA ACAO ---\n");

    if (*total >= MAX_ACOES) {
        printf("[ERRO] Limite de acoes atingido (maximo de %d acoes).\n", MAX_ACOES);
        return 0;
    }

    do {
        nova.codigo = lerInteiro("Codigo da acao (numero inteiro maior que zero): ");
        if (nova.codigo <= 0) {
            printf("[ERRO] Codigo invalido. Use um numero maior que zero.\n");
        } else if (existeCodigo(acoes, *total, nova.codigo) == 1) {
            printf("[ERRO] Codigo ja cadastrado. Escolha outro numero.\n");
        }
    } while (nova.codigo <= 0 || existeCodigo(acoes, *total, nova.codigo) == 1);

    lerTexto("Escola: ", nova.escola, TAM_TEXTO);
    lerTexto("Tema (ex: Saude Mental - Roda de conversa): ", nova.tema, TAM_TEXTO);
    lerData("Data prevista (dd/mm/aaaa): ", nova.dataPrevista);
    lerTexto("Publico-alvo (coletivo, ex: Turmas do 2o ano): ", nova.publicoAlvo, TAM_TEXTO);
    lerTexto("Responsavel (ex: Coordenacao Pedagogica): ", nova.responsavel, TAM_TEXTO);

    do {
        nova.qtdPrevista = lerInteiro("Quantidade prevista de participantes: ");
        if (nova.qtdPrevista <= 0) {
            printf("[ERRO] Quantidade invalida. Informe um numero maior que zero.\n");
        }
    } while (nova.qtdPrevista <= 0);

    nova.situacao = PLANEJADA;
    nova.qtdEfetiva = 0;

    acoes[*total] = nova;
    (*total)++;

    printf("\n[OK] Acao cadastrada com sucesso! Total de acoes: %d\n", *total);
    return 0;
}

int listarAcoes(Acao acoes[], int total) {
    int i;

    printf("\n--- LISTA DE ACOES CADASTRADAS ---\n");

    if (total == 0) {
        printf("Nenhuma acao cadastrada ate o momento.\n");
        return 0;
    }

    for (i = 0; i < total; i++) {
        exibirAcao(acoes[i]);
    }
    printf("Total de acoes listadas: %d\n", total);
    return 0;
}

int pesquisarAcoes(Acao acoes[], int total) {
    int opcao, i, indice, codigoBusca;
    int encontrados = 0;
    char termo[TAM_TEXTO];

    if (total == 0) {
        printf("\nNenhuma acao cadastrada para pesquisar.\n");
        return 0;
    }

    printf("\n--- PESQUISAR ACAO ---\n");
    printf("1 - Por codigo\n");
    printf("2 - Por escola\n");
    printf("3 - Por tema\n");
    opcao = lerInteiro("Escolha o tipo de pesquisa: ");

    switch (opcao) {
        case 1:
            codigoBusca = lerInteiro("Digite o codigo: ");
            indice = buscarIndice(acoes, total, codigoBusca);
            if (indice != -1) {
                exibirAcao(acoes[indice]);
                encontrados = 1;
            }
            break;
        case 2:
            lerTexto("Digite o nome (ou parte) da escola: ", termo, TAM_TEXTO);
            for (i = 0; i < total; i++) {
                if (contemTexto(acoes[i].escola, termo) == 1) {
                    exibirAcao(acoes[i]);
                    encontrados++;
                }
            }
            break;
        case 3:
            lerTexto("Digite o tema (ou parte): ", termo, TAM_TEXTO);
            for (i = 0; i < total; i++) {
                if (contemTexto(acoes[i].tema, termo) == 1) {
                    exibirAcao(acoes[i]);
                    encontrados++;
                }
            }
            break;
        default:
            printf("[ERRO] Opcao de pesquisa invalida.\n");
            return 0;
    }

    if (encontrados == 0) {
        printf("\nNenhum registro encontrado para essa pesquisa.\n");
    } else {
        printf("\n%d registro(s) encontrado(s).\n", encontrados);
    }
    return 0;
}

int atualizarSituacao(Acao acoes[], int total) {
    int codigoBusca, indice, novaSituacao;
    char textoSituacao[TAM_SITUACAO];

    if (total == 0) {
        printf("\nNenhuma acao cadastrada para atualizar.\n");
        return 0;
    }

    printf("\n--- ATUALIZAR SITUACAO DA ACAO ---\n");
    codigoBusca = lerInteiro("Digite o codigo da acao: ");
    indice = buscarIndice(acoes, total, codigoBusca);

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
            acoes[indice].qtdEfetiva = 0;
            printf("[OK] Situacao atualizada para PLANEJADA.\n");
            break;
        case 2:
            do {
                acoes[indice].qtdEfetiva = lerInteiro("Quantidade efetiva de participantes: ");
                if (acoes[indice].qtdEfetiva < 0) {
                    printf("[ERRO] A quantidade nao pode ser negativa.\n");
                }
            } while (acoes[indice].qtdEfetiva < 0);
            acoes[indice].situacao = REALIZADA;
            printf("[OK] Situacao atualizada para REALIZADA.\n");
            if (acoes[indice].qtdEfetiva > acoes[indice].qtdPrevista) {
                printf("[AVISO] Participacao acima do previsto (%d previstos).\n",
                       acoes[indice].qtdPrevista);
            }
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

/* Resumo geral (filtroEscola vazio) ou resumo de uma escola.
   O percentual de participacao considera apenas as acoes REALIZADAS:
   participantes efetivos / participantes previstos dessas acoes. */
int gerarResumo(Acao acoes[], int total, const char *filtroEscola) {
    int i, considerar;
    int consideradas = 0;
    int qtdPlanejadas = 0, qtdRealizadas = 0, qtdCanceladas = 0;
    int totalPrevisto = 0, totalEfetivo = 0;
    float percentual = 0.0f;

    if (strlen(filtroEscola) > 0) {
        printf("\n--- RESUMO DA ESCOLA: %s ---\n", filtroEscola);
    } else {
        printf("\n--- RESUMO GERAL DO PROGRAMA ---\n");
    }

    if (total == 0) {
        printf("Nenhuma acao cadastrada ate o momento.\n");
        return 0;
    }

    for (i = 0; i < total; i++) {
        considerar = 1;
        if (strlen(filtroEscola) > 0) {
            considerar = contemTexto(acoes[i].escola, filtroEscola);
        }
        if (considerar == 1) {
            consideradas++;
            switch (acoes[i].situacao) {
                case PLANEJADA:
                    qtdPlanejadas++;
                    break;
                case REALIZADA:
                    qtdRealizadas++;
                    totalPrevisto += acoes[i].qtdPrevista;
                    totalEfetivo += acoes[i].qtdEfetiva;
                    break;
                case CANCELADA:
                    qtdCanceladas++;
                    break;
            }
        }
    }

    if (consideradas == 0) {
        printf("Nenhuma acao encontrada para esta escola.\n");
        return 0;
    }

    if (totalPrevisto > 0) {
        percentual = (totalEfetivo * 100.0f) / totalPrevisto;
    }

    printf("Acoes consideradas..................: %d\n", consideradas);
    printf("Acoes planejadas....................: %d\n", qtdPlanejadas);
    printf("Acoes realizadas....................: %d\n", qtdRealizadas);
    printf("Acoes canceladas....................: %d\n", qtdCanceladas);
    printf("Participantes previstos (realizadas): %d\n", totalPrevisto);
    printf("Participantes efetivos (realizadas).: %d\n", totalEfetivo);
    if (totalPrevisto > 0) {
        printf("Percentual de participacao..........: %.2f%%\n", percentual);
    } else {
        printf("Percentual de participacao..........: sem acoes realizadas ainda\n");
    }
    return 0;
}

int resumoPorEscola(Acao acoes[], int total) {
    char escola[TAM_TEXTO];

    if (total == 0) {
        printf("\n--- RESUMO POR ESCOLA ---\n");
        printf("Nenhuma acao cadastrada ate o momento.\n");
        return 0;
    }

    lerTexto("\nDigite o nome (ou parte) da escola: ", escola, TAM_TEXTO);
    gerarResumo(acoes, total, escola);
    return 0;
}

/* ========================================================================
   Dados ficticios para testes e para a apresentacao
   ======================================================================== */

/* Retorna 1 se adicionou; 0 se ignorou (codigo repetido ou limite atingido). */
int adicionarAcaoDemo(Acao acoes[], int *total, int codigo, const char *escola,
                      const char *tema, const char *data, const char *publico,
                      const char *responsavel, int prevista, int situacao,
                      int efetiva) {
    if (*total >= MAX_ACOES || existeCodigo(acoes, *total, codigo) == 1) {
        return 0;
    }
    acoes[*total].codigo = codigo;
    strcpy(acoes[*total].escola, escola);
    strcpy(acoes[*total].tema, tema);
    strcpy(acoes[*total].dataPrevista, data);
    strcpy(acoes[*total].publicoAlvo, publico);
    strcpy(acoes[*total].responsavel, responsavel);
    acoes[*total].qtdPrevista = prevista;
    acoes[*total].situacao = situacao;
    acoes[*total].qtdEfetiva = efetiva;
    (*total)++;
    return 1;
}

int carregarDadosDemonstracao(Acao acoes[], int *total) {
    int adicionadas = 0;
    int ignoradas = 0;
    int resultado;

    resultado = adicionarAcaoDemo(acoes, total, 101, "Colegio Estadual Modelo Norte",
        "Saude Mental - Roda de conversa", "15/09/2026", "Turmas do 1o ano - Ensino Medio",
        "Coordenacao Pedagogica", 40, REALIZADA, 36);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 102, "Colegio Estadual Modelo Norte",
        "Neurodiversidade - Formacao de professores", "22/09/2026", "Corpo docente",
        "Direcao Escolar", 25, REALIZADA, 25);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 103, "Colegio Estadual Modelo Centro",
        "Cultura de Paz - Semana de acolhimento", "06/10/2026", "Todas as turmas do Ensino Medio",
        "Coordenacao Pedagogica", 120, REALIZADA, 90);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 104, "Colegio Estadual Modelo Centro",
        "Saude Mental - Oficina sobre ansiedade", "10/11/2026", "Turmas do 2o ano - Ensino Medio",
        "Equipe Pedagogica", 35, PLANEJADA, 0);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 105, "Colegio Estadual Modelo Sul",
        "Inclusao - Acessibilidade e adaptacoes em sala", "17/11/2026", "Corpo docente e equipe de apoio",
        "Direcao Escolar", 30, PLANEJADA, 0);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 106, "Colegio Estadual Modelo Sul",
        "Alimentacao saudavel - Feira de saberes", "24/11/2026", "Turmas do 3o ano - Ensino Medio",
        "Coordenacao Pedagogica", 60, CANCELADA, 0);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    resultado = adicionarAcaoDemo(acoes, total, 107, "Colegio Estadual Modelo Sul",
        "Saude Mental - Roda de conversa", "01/12/2026", "Turmas do 1o ano - Ensino Medio",
        "Equipe Pedagogica", 45, PLANEJADA, 0);
    adicionadas += resultado;
    ignoradas += 1 - resultado;

    printf("\n[OK] Dados de demonstracao: %d acao(oes) carregada(s), %d ignorada(s) ", adicionadas, ignoradas);
    printf("(codigo repetido ou limite atingido).\n");
    return 0;
}
