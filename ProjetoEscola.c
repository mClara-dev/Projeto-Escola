#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define ALUNO 1
#define PROFESSOR 2

#define MAX_PESSOAS 200
#define MAX_DISCI 100
#define MAX_ALUNOS_DISCI 50 // Capacidade máxima de alunos por disciplina
#define VAGAS_LIMITE 40     

#define NOME_SIZE 100
#define CPF_SIZE 15
#define MATRICULA_INICIAL 1000

// Códigos de retorno
#define LISTA_CHEIA 1
#define CAD_SUCESSO 1
#define MATRICULA_INVALIDA -1
#define ATUALIZACAO_SUCESSO 2
#define EXCLUSAO_SUCESSO 3
#define MATRICULA_INEXISTENTE -3
#define CODIGO_INVALIDO -4
#define DISCIPLINA_INEXISTENTE -5
#define CAD_DISCIPLINA_SUCESSO 5
#define EXCLUSAO_DISCIPLINA_SUCESSO 6
#define ATUALIZACAO_DISCIPLINA_SUCESSO 7

// --- IMPLEMENTAÇÃO AUXILIAR DE FUNÇÃO NÃO-PADRÃO (para strcasestr) ---
// Retorna a primeira ocorrência (case-insensitive)
char *strcasestr_custom(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (; *haystack; ++haystack) {
        if (toupper((unsigned char)*haystack) == toupper((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n && (toupper((unsigned char)*h) == toupper((unsigned char)*n)); ++h, ++n);
            if (!*n) return (char *)haystack;
        }
    }
    return NULL;
}

// ESTRUTURAS (STRUCTS)
typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int matricula;
    char nome[NOME_SIZE];
    char sexo;
    Data dataNascimento;
    char cpf[CPF_SIZE];
    int ativo; // 1= ativo, 0 = excluido (lógica)
    int tipo;  // 1 = aluno, 2 = professor
} Matriculado;

typedef struct {
    char nome[100];
    char codigo[10]; 
    int semestre;
    char professor[NOME_SIZE]; 
    int alunosMatriculados[MAX_ALUNOS_DISCI];
    int qtdAlunos;
    int ativo;
} Disciplina;

// VARIÁVEIS GLOBAIS 
Matriculado vetor[MAX_PESSOAS];
Disciplina disciplinas[MAX_DISCI];
int proximaMatricula = MATRICULA_INICIAL;
int qtdMatriculados = 0;
int qtdDisciplinas = 0;

// PROTOTIPOS
// Menus
int menuGeral();
int menuListagens();
int menuMatriculados(const char *c); 
int menuDisciplina();
int menuOrdenacao();
int menuListagensPessoas();

// CRUD e Auxiliares de Pessoa
int cadastrarPessoa(Matriculado lista[], int *qtd, int tipo);
int excluirPessoa(Matriculado vetorLocal[], int *qtdPessoas, int tipo);
int atualizarPessoa(Matriculado vetorLocal[], int qtdPessoas, int tipo);
int buscarPessoa(Matriculado vetorLocal[], int qtdPessoas, int matricula);
int gerarMatricula();

// CRUD e Auxiliares de Disciplina
int cadastrarDisciplina(Disciplina listaDisciplina[], int *qtdDisciplina);
void listarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina);
int atualizarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina);
int excluirDisciplina(Disciplina listaDisciplina[], int *qtdDisciplina);

int inserirAlunoDisciplina(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, char codigo[], int matAluno);
int excluirAlunoDisciplina(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, char codigo[], int matAluno);

// Validacao
int ehBissexto(int ano);
int validar_data_struct(Data d);
int validar_nome(char nome[]);
int validar_sexo(char sexo);
int validar_CPF(const char cpf[]);
int validar_codigoDisciplina(const char codigo[]);
int ler_opcao(int min, int max);

// Listagens (Itens 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18)
void listarPessoas(Matriculado vetorLocal[], int qtdPessoas, int tipo);
void listarDisciplinaCompleta(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, Matriculado vetorLocal[], int qtdPessoas);
void listarPessoasPorSexo(Matriculado vetorLocal[], int qtdPessoas, int tipo, char sexo);
void ordenarPessoasPorNome(Matriculado vetorLocal[], int qtdPessoas, int tipo);
void ordenarPessoasPorDataNascimento(Matriculado vetorLocal[], int qtdPessoas, int tipo);
void listarAniversariantesDoMes(Matriculado vetorLocal[], int qtdPessoas);
void buscarPessoasPorString(Matriculado vetorLocal[], int qtdPessoas, char busca[]);
int listarAlunosMenos3Disciplinas();
void listarDisciplinasVagasExcedidas();
int contarDisciplinasMatriculadas(int matricula);

// IMPLEMENTAÇÕES DOS MENUS 

int menuGeral() {
    int opcao;
    printf("\n--- MENU GERAL ---\n");
    printf("0 - Sair\n");
    printf("1 - Aluno (Cadastro/CRUD)\n");
    printf("2 - Professor (Cadastro/CRUD)\n");
    printf("3 - Disciplina (Cadastro/Matrícula)\n");
    printf("4 - Listagens e Relatórios\n");
    printf("Escolha uma opção: ");
    return ler_opcao(0, 4);
}

// CORREÇÃO: Usando const char *c para evitar warning de overflow de string literal
int menuMatriculados(const char *c) {
    int opcao;
    printf("\n--- MENU %s ---\n", c);
    printf("0 - Voltar\n");
    printf("1 - Cadastrar %s\n", c);
    printf("2 - Listar %s(s)\n", c);
    printf("3 - Atualizar %s\n", c);
    printf("4 - Excluir %s\n", c);
    printf("Escolha uma opção: ");
    return ler_opcao(0, 4);
}

int menuDisciplina() {
    int opcao;
    printf("\n--- MENU DISCIPLINA ---\n");
    printf("0 - Voltar\n");
    printf("1 - Cadastrar Disciplina\n");
    printf("2 - Listar Disciplinas (Básico)\n");
    printf("3 - Excluir Disciplina\n");
    printf("4 - Inserir aluno em disciplina\n");
    printf("5 - Remover aluno de disciplina\n");
    printf("6 - Atualizar Disciplina\n"); 
    printf("7 - Listar Disciplinas (Completo)\n"); 
    printf("Escolha uma opção: ");
    return ler_opcao(0, 7);
}

int menuListagens() {
    int opcao;
    printf("\n--- MENU LISTAGENS ---\n");
    printf("0 - Voltar\n");
    printf("1 - Aniversariantes do mês (Item 15)\n");
    printf("2 - Disciplinas acima de %d alunos (Item 18)\n", VAGAS_LIMITE);
    printf("3 - Alunos em menos de 3 disciplinas (Item 17)\n");
    printf("4 - Listagens de Pessoas (Por Sexo/Busca)\n"); // Submenu
    printf("5 - Ordenação de Pessoas (Por Nome/Data)\n"); // Submenu
    printf("Escolha uma opção: ");
    return ler_opcao(0, 5);
}

int menuListagensPessoas() {
    int opcao;
    printf("\n--- LISTAGENS DE PESSOAS ---\n");
    printf("0 - Voltar\n");
    printf("1 - Listar por Sexo (Alunos)\n"); // Item 9
    printf("2 - Listar por Sexo (Professores)\n"); // Item 12
    printf("3 - Buscar por Trecho no Nome (Item 16)\n");
    printf("Escolha uma opção: ");
    return ler_opcao(0, 3);
}

int menuOrdenacao() {
    int opcao;
    printf("\n--- ORDENAÇÃO DE PESSOAS ---\n");
    printf("0 - Voltar\n");
    printf("1 - Alunos por Nome (Item 10)\n");
    printf("2 - Alunos por Data de Nascimento (Item 11)\n");
    printf("3 - Professores por Nome (Item 13)\n");
    printf("4 - Professores por Data de Nascimento (Item 14)\n");
    printf("Escolha uma opção: ");
    return ler_opcao(0, 4);
}

// IMPLEMENTACOES DE UTILIDADES E VALIDACOES
int ehBissexto(int ano){
    if((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) return 1;
    else return 0;
}

int validar_data_struct(Data d){
    if(d.ano < 1900 || d.ano > 2100) return 0;
    if(d.mes < 1 || d.mes > 12) return 0;
    int diasPorMes[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(d.mes == 2 && ehBissexto(d.ano)) diasPorMes[2] = 29;
    if(d.dia < 1 || d.dia > diasPorMes[d.mes]) return 0;
    return 1;
}

int validar_nome(char nome[]){
    if(strlen(nome) < 3) return 0;
    for(int i = 0; nome[i] != '\0'; i++){
        if(!isalpha((unsigned char)nome[i]) && nome[i] != ' ') return 0;
    }
    return 1;
}

int validar_sexo(char sexo){
    return (sexo == 'M' || sexo == 'F' || sexo == 'm' || sexo == 'f');
}

int validar_codigoDisciplina(const char codigo[]){
    if (codigo == NULL || strlen(codigo) < 3 || strlen(codigo) > 9) return 0;
    for (size_t i = 0; i < strlen(codigo); i++){
        if (!isalnum((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

int gerarMatricula() {
    int matriculaGerada = proximaMatricula;
    proximaMatricula++;
    return matriculaGerada;
}

int buscarPessoa(Matriculado vetorLocal[], int qtdPessoas, int matricula) {
    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].matricula == matricula) return i;
    }
    return -1;
}

int ler_opcao(int min, int max) {
    int opcao;
    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    if (opcao < min || opcao > max) return -1;
    return opcao;
}

int validar_CPF(const char *cpf_raw) {
    if (cpf_raw == NULL) return 0;
    int tmp[20];
    int t = 0;
    for (size_t i = 0; cpf_raw[i] != '\0'; i++) {
        if (isdigit((unsigned char)cpf_raw[i])) {
            if (t < 20) tmp[t] = cpf_raw[i] - '0';
            t++;
        }
    }
    if (t != 11) return 0;
    int d[11];
    for (int i = 0; i < 11; i++) d[i] = tmp[i];
    int allsame = 1;
    for (int i = 1; i < 11; i++) {
        if (d[i] != d[0]) { allsame = 0; break; }
    }
    if (allsame) return 0;
    int soma = 0;
    for (int i = 0, peso = 10; i < 9; i++, peso--) soma += d[i] * peso;
    int resto = soma % 11;
    int dv1 = (resto < 2) ? 0 : 11 - resto;
    soma = 0;
    for (int i = 0, peso = 11; i < 10; i++, peso--) soma += d[i] * peso;
    resto = soma % 11;
    int dv2 = (resto < 2) ? 0 : 11 - resto;
    return (dv1 == d[9] && dv2 == d[10]) ? 1 : 0;
}


//  IMPLEMENTAcoES DE CRUD PESSOA 

int cadastrarPessoa(Matriculado vetor[], int *qtd, int tipo) {
    if (*qtd >= MAX_PESSOAS) return LISTA_CHEIA;
    Matriculado novaPessoa;
    char temp_nome[NOME_SIZE];
    char temp_cpf[CPF_SIZE];
    char temp_data_str[20];
    char temp_sexo;
    int data_valida = 0;
    novaPessoa.matricula = gerarMatricula();
    novaPessoa.tipo = tipo;
    novaPessoa.ativo = 1;

    printf("\n--- Cadastro de %s ---\n", tipo == ALUNO ? "Aluno" : "Professor");
    printf("Matrícula gerada: %d\n", novaPessoa.matricula);
    while (getchar() != '\n' && !feof(stdin));

    do {
        printf("Insira o nome: ");
        scanf(" %[^\n]", temp_nome);
        if (!validar_nome(temp_nome)) printf("Nome inválido.\n");
    } while (!validar_nome(temp_nome));
    strcpy(novaPessoa.nome, temp_nome);

    do {
        printf("Insira o sexo (M/F): ");
        scanf(" %c", &temp_sexo);
        if (!validar_sexo(temp_sexo)) printf("Sexo inválido.\n");
    } while (!validar_sexo(temp_sexo));
    novaPessoa.sexo = toupper((unsigned char)temp_sexo);

    do {
        printf("Insira a data de nascimento (dd/mm/aaaa): ");
        scanf(" %19s", temp_data_str);
        int d, m, a;
        if (sscanf(temp_data_str, "%d/%d/%d", &d, &m, &a) != 3) {
            printf("Formato inválido. Use dd/mm/aaaa\n");
            data_valida = 0;
        } else {
            Data dd = {d, m, a};
            data_valida = validar_data_struct(dd);
            if (!data_valida) printf("Data inválida.\n");
            else novaPessoa.dataNascimento = dd;
        }
    } while (!data_valida);

    do {
        printf("Insira o CPF (XXX.XXX.XXX-XX ou 11 dígitos): ");
        scanf(" %14s", temp_cpf);
        if (!validar_CPF(temp_cpf)) printf("CPF inválido.\n");
    } while (!validar_CPF(temp_cpf));
    strcpy(novaPessoa.cpf, temp_cpf);

    vetor[*qtd] = novaPessoa;
    (*qtd)++;
    printf("%s cadastrado com sucesso! Matrícula: %d\n", tipo == ALUNO ? "Aluno" : "Professor", novaPessoa.matricula);
    return CAD_SUCESSO;
}

int excluirPessoa(Matriculado vetorLocal[], int *qtdPessoas, int tipo){
    int matricula_busca;
    printf("Exclusão de %s\n", tipo == ALUNO ? "Aluno" : "Professor");
    printf("Digite a matrícula a excluir: ");
    if (scanf("%d", &matricula_busca) != 1) { while (getchar() != '\n'); printf("Entrada inválida.\n"); return -1; }

    int indice = buscarPessoa(vetorLocal, *qtdPessoas, matricula_busca);
    if (indice == -1 || vetorLocal[indice].ativo == 0 || vetorLocal[indice].tipo != tipo) {
        printf("Matrícula não encontrada, inativa ou tipo incorreto.\n");
        return MATRICULA_INEXISTENTE;
    }

    // Exclusao logica
    vetorLocal[indice].ativo = 0;
    printf("%s com matrícula %d excluído (logicamente) com sucesso.\n", tipo == ALUNO ? "Aluno" : "Professor", matricula_busca);
    return EXCLUSAO_SUCESSO;
}

int atualizarPessoa(Matriculado vetorLocal[], int qtdPessoas, int tipo){
    int matricula_busca;
    int indice;
    int opcao;

    printf("Atualização de cadastro de %s\n", tipo == ALUNO ? "Aluno" : "Professor");
    printf("Digite a matrícula do %s a ser atualizado: ", tipo == ALUNO ? "Aluno" : "Professor");
    if(scanf("%d", &matricula_busca) != 1){ while(getchar() != '\n'); printf("Entrada inválida.\n"); return -1; }

    indice = buscarPessoa(vetorLocal, qtdPessoas, matricula_busca);
    if (indice == -1 || vetorLocal[indice].tipo != tipo || vetorLocal[indice].ativo == 0) {
        printf("Erro: Matrícula não encontrada, inativa ou tipo incorreto.\n");
        return MATRICULA_INEXISTENTE;
    }

    Matriculado *p = &vetorLocal[indice];
    printf("\n%s encontrado: %s\n", tipo == ALUNO ? "Aluno" : "Professor", p->nome);

    do {
        printf("\nQual campo deseja atualizar?\n");
        printf("1. Nome: %s\n", p->nome);
        printf("2. Sexo: %c\n", p->sexo);
        printf("3. Data de Nascimento: %02d/%02d/%d\n", p->dataNascimento.dia, p->dataNascimento.mes, p->dataNascimento.ano);
        printf("4. CPF: %s\n", p->cpf);
        printf("0. Finalizar Atualização\n");
        printf("Opção: ");

        opcao = ler_opcao(0, 4);
        if (opcao == -1) { printf("Opção inválida.\n"); continue; }

        char temp_str[120];
        Data temp_data;
        int valida;

        switch (opcao) {
            case 1: do { printf("Novo Nome: "); scanf(" %[^\n]", temp_str); valida = validar_nome(temp_str); if (!valida) printf("Nome inválido.\n"); } while (!valida);
                strcpy(p->nome, temp_str); printf("Nome atualizado.\n"); break;
            case 2: do { printf("Novo Sexo (M/F): "); scanf(" %c", &temp_str[0]); valida = validar_sexo(temp_str[0]); if (!valida) printf("Sexo inválido.\n"); } while (!valida);
                p->sexo = toupper((unsigned char)temp_str[0]); printf("Sexo atualizado.\n"); break;
            case 3: do { printf("Nova Data (DD/MM/AAAA): "); scanf(" %19s", temp_str);
                    int campos = sscanf(temp_str, "%d/%d/%d", &temp_data.dia, &temp_data.mes, &temp_data.ano);
                    if (campos != 3 || !(valida = validar_data_struct(temp_data))) printf("Data inválida.\n"); else p->dataNascimento = temp_data;
                } while (!valida); printf("Data atualizada.\n"); break;
            case 4: do { printf("Novo CPF: "); scanf(" %14s", temp_str); valida = validar_CPF(temp_str); if (!valida) printf("CPF inválido.\n"); } while (!valida);
                strcpy(p->cpf, temp_str); printf("CPF atualizado.\n"); break;
            case 0: return ATUALIZACAO_SUCESSO;
        }
    } while (1);
}

// IMPLEMENTACOES DE CRUD DISCIPLINA 

int cadastrarDisciplina(Disciplina listaDisciplina[], int *qtdDisciplina) {
    if (*qtdDisciplina >= MAX_DISCI) return LISTA_CHEIA;
    char codigo[10];
    char nomeDisc[100];
    int semestre;
    char professor[NOME_SIZE];

    printf("Digite o código da disciplina (ex: INFO201): ");
    scanf(" %9s", codigo);
    if (!validar_codigoDisciplina(codigo)) { printf("Código inválido.\n"); return CODIGO_INVALIDO; }

    for (int i = 0; i < *qtdDisciplina; i++) { // Verifica duplicidade
        if (strcmp(listaDisciplina[i].codigo, codigo) == 0 && listaDisciplina[i].ativo == 1) {
            printf("Erro: Disciplina já cadastrada com este código.\n");
            return CODIGO_INVALIDO;
        }
    }

    printf("Digite o nome da disciplina: ");
    scanf(" %[^\n]", nomeDisc);
    printf("Digite o semestre (1, 2, 3...): ");
    if (scanf("%d", &semestre) != 1 || semestre <= 0) { while (getchar() != '\n'); printf("Semestre inválido.\n"); return CODIGO_INVALIDO; }
    printf("Digite o nome do professor: ");
    scanf(" %[^\n]", professor);

    Disciplina *d = &listaDisciplina[*qtdDisciplina];
    strcpy(d->codigo, codigo);
    strcpy(d->nome, nomeDisc);
    d->semestre = semestre;
    strcpy(d->professor, professor);
    d->qtdAlunos = 0;
    d->ativo = 1;
    for (int k = 0; k < MAX_ALUNOS_DISCI; k++) d->alunosMatriculados[k] = -1;

    (*qtdDisciplina)++;
    return CAD_DISCIPLINA_SUCESSO;
}

void listarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina) {
    printf("\n--- Lista de Disciplinas (Ativas) ---\n");
    int achou = 0;
    for (int i = 0; i < qtdDisciplina; i++) {
        if (listaDisciplina[i].ativo == 1) {
            printf("Código: %s | Nome: %s | Semestre: %d | Professor: %s | Alunos: %d\n",
                   listaDisciplina[i].codigo, listaDisciplina[i].nome,
                   listaDisciplina[i].semestre, listaDisciplina[i].professor,
                   listaDisciplina[i].qtdAlunos);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhuma disciplina ativa encontrada.\n");
}

int atualizarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina) {
    char codigo[10];
    printf("Digite o código da disciplina a ser atualizada: ");
    scanf(" %9s", codigo);

    int indice = -1;
    for (int i = 0; i < qtdDisciplina; i++) {
        if (strcmp(listaDisciplina[i].codigo, codigo) == 0 && listaDisciplina[i].ativo == 1) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Disciplina não encontrada ou inativa.\n");
        return DISCIPLINA_INEXISTENTE;
    }

    Disciplina *d = &listaDisciplina[indice];
    printf("\nDisciplina encontrada: %s\n", d->nome);

    int opcao;
    do {
        printf("\nQual campo deseja atualizar?\n");
        printf("1. Nome: %s\n", d->nome);
        printf("2. Semestre: %d\n", d->semestre);
        printf("3. Professor: %s\n", d->professor);
        printf("0. Finalizar Atualização\n");
        printf("Opção: ");
        opcao = ler_opcao(0, 3);
        if (opcao == -1) { printf("Opção inválida.\n"); continue; }

        char temp_str[NOME_SIZE];
        int temp_int;

        switch (opcao) {
            case 1:
                printf("Novo Nome: ");
                scanf(" %[^\n]", temp_str);
                strcpy(d->nome, temp_str);
                printf("Nome atualizado.\n");
                break;
            case 2:
                printf("Novo Semestre: ");
                if (scanf("%d", &temp_int) == 1 && temp_int > 0) {
                    d->semestre = temp_int;
                    printf("Semestre atualizado.\n");
                } else {
                    while (getchar() != '\n');
                    printf("Valor inválido.\n");
                }
                break;
            case 3:
                printf("Novo Professor: ");
                scanf(" %[^\n]", temp_str);
                strcpy(d->professor, temp_str);
                printf("Professor atualizado.\n");
                break;
            case 0:
                printf("Atualização finalizada.\n");
                return ATUALIZACAO_DISCIPLINA_SUCESSO;
        }
    } while (1);
}

int excluirDisciplina(Disciplina listaDisciplina[], int *qtdDisciplina) {
    char codigo[10];
    printf("Digite o código da disciplina a excluir (exclusão lógica): ");
    scanf(" %9s", codigo);

    for (int i = 0; i < *qtdDisciplina; i++) {
        if (strcmp(listaDisciplina[i].codigo, codigo) == 0 && listaDisciplina[i].ativo == 1) {
            listaDisciplina[i].ativo = 0; 
            // Nao rearranja a lista nem decrementa qtdDisciplinas para manter a ordem e os indices
            printf("Disciplina %s excluída (logicamente) com sucesso.\n", codigo);
            return EXCLUSAO_DISCIPLINA_SUCESSO;
        }
    }
    return DISCIPLINA_INEXISTENTE;
}

// IMPLEMENTACOES DE MATRICULA EM DISCIPLINA 

int inserirAlunoDisciplina(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, char codigo[], int matAluno) {
    int idxAluno = buscarPessoa(vetor, qtdMatriculados, matAluno);
    if (idxAluno == -1 || vetor[idxAluno].ativo == 0 || vetor[idxAluno].tipo != ALUNO) {
        return MATRICULA_INEXISTENTE; // Não é um aluno ativo
    }

    for (int i = 0; i < qtdDisciplinasLocal; i++) {
        if (disciplinasLocal[i].ativo == 1 && strcmp(disciplinasLocal[i].codigo, codigo) == 0) {
            if (disciplinasLocal[i].qtdAlunos >= MAX_ALUNOS_DISCI) return LISTA_CHEIA;
            for (int j = 0; j < disciplinasLocal[i].qtdAlunos; j++) {
                if (disciplinasLocal[i].alunosMatriculados[j] == matAluno) return 0; // já matriculado
            }
            disciplinasLocal[i].alunosMatriculados[disciplinasLocal[i].qtdAlunos++] = matAluno;
            return CAD_SUCESSO;
        }
    }
    return DISCIPLINA_INEXISTENTE;
}

int excluirAlunoDisciplina(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, char codigo[], int matAluno) {
    for (int i = 0; i < qtdDisciplinasLocal; i++) {
        if (disciplinasLocal[i].ativo == 1 && strcmp(disciplinasLocal[i].codigo, codigo) == 0) {
            int found = -1;
            for (int j = 0; j < disciplinasLocal[i].qtdAlunos; j++) {
                if (disciplinasLocal[i].alunosMatriculados[j] == matAluno) { found = j; break; }
            }
            if (found == -1) return MATRICULA_INEXISTENTE;

            // Remove o aluno (shift left)
            for (int k = found; k < disciplinasLocal[i].qtdAlunos - 1; k++) {
                disciplinasLocal[i].alunosMatriculados[k] = disciplinasLocal[i].alunosMatriculados[k+1];
            }
            disciplinasLocal[i].qtdAlunos--;
            disciplinasLocal[i].alunosMatriculados[disciplinasLocal[i].qtdAlunos] = -1; // Limpa o último

            return EXCLUSAO_SUCESSO;
        }
    }
    return DISCIPLINA_INEXISTENTE;
}

//IMPLEMENTACOES DE LISTAGENS 

void listarPessoas(Matriculado vetorLocal[], int qtdPessoas, int tipo) {
    printf("\n--- Lista de %s(s) ---\n", tipo == ALUNO ? "Aluno" : "Professor");
    int achou = 0;
    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].ativo == 1 && vetorLocal[i].tipo == tipo) {
            printf("Mat: %d | Nome: %s | Sexo: %c | Nasc: %02d/%02d/%04d | CPF: %s\n",
                   vetorLocal[i].matricula, vetorLocal[i].nome, vetorLocal[i].sexo,
                   vetorLocal[i].dataNascimento.dia, vetorLocal[i].dataNascimento.mes,
                   vetorLocal[i].dataNascimento.ano, vetorLocal[i].cpf);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum registro.\n");
}

void listarDisciplinaCompleta(Disciplina disciplinasLocal[], int qtdDisciplinasLocal, Matriculado vetorLocal[], int qtdPessoas) {
    printf("\n--- Disciplinas (detalhado) ---\n");
    for (int i = 0; i < qtdDisciplinasLocal; i++) {
        if (disciplinasLocal[i].ativo == 1) {
            printf("\nCódigo: %s | Nome: %s | Semestre: %d | Prof.: %s | Alunos (%d):\n",
                   disciplinasLocal[i].codigo, disciplinasLocal[i].nome, disciplinasLocal[i].semestre,
                   disciplinasLocal[i].professor, disciplinasLocal[i].qtdAlunos);
            int achouAluno = 0;
            for (int j = 0; j < disciplinasLocal[i].qtdAlunos; j++) {
                int mat = disciplinasLocal[i].alunosMatriculados[j];
                int idx = buscarPessoa(vetorLocal, qtdPessoas, mat);
                if (idx != -1 && vetorLocal[idx].ativo == 1) {
                    printf("  - %s (mat %d)\n", vetorLocal[idx].nome, mat);
                    achouAluno = 1;
                }
            }
            if (!achouAluno) printf("  (Nenhum aluno ativo matriculado)\n");
        }
    }
}

// Funcao auxiliar para ordenacao (copia e ordena para nao mudar o vetor principal)
void listarPessoasOrdenadas(Matriculado vetorLocal[], int qtdPessoas, int tipo, int porNome) {
    Matriculado copia[MAX_PESSOAS];
    int qtdCopia = 0;

    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].ativo == 1 && vetorLocal[i].tipo == tipo) {
            copia[qtdCopia++] = vetorLocal[i];
        }
    }

    if (qtdCopia == 0) {
        printf("Nenhum %s ativo para listar.\n", tipo == ALUNO ? "aluno" : "professor");
        return;
    }

    // Ordenacao Bubble Sort na copia
    for (int i = 0; i < qtdCopia - 1; i++) {
        for (int j = i + 1; j < qtdCopia; j++) {
            int troca = 0;
            if (porNome) {
                if (strcasecmp(copia[i].nome, copia[j].nome) > 0) troca = 1;
            } else { // Por Data de Nascimento
                Data a = copia[i].dataNascimento;
                Data b = copia[j].dataNascimento;
                if (a.ano > b.ano || (a.ano == b.ano && (a.mes > b.mes || (a.mes == b.mes && a.dia > b.dia)))) troca = 1;
            }
            if (troca) {
                Matriculado tmp = copia[i];
                copia[i] = copia[j];
                copia[j] = tmp;
            }
        }
    }

    printf("\n--- Lista de %s(s) Ordenada por %s ---\n", tipo == ALUNO ? "Aluno" : "Professor", porNome ? "Nome" : "Data de Nasc.");
    for (int i = 0; i < qtdCopia; i++) {
        printf("Mat: %d | Nome: %s | Nasc: %02d/%02d/%04d\n",
               copia[i].matricula, copia[i].nome, copia[i].dataNascimento.dia,
               copia[i].dataNascimento.mes, copia[i].dataNascimento.ano);
    }
}

void ordenarPessoasPorNome(Matriculado vetorLocal[], int qtdPessoas, int tipo) {
    listarPessoasOrdenadas(vetorLocal, qtdPessoas, tipo, 1);
}

void ordenarPessoasPorDataNascimento(Matriculado vetorLocal[], int qtdPessoas, int tipo) {
    listarPessoasOrdenadas(vetorLocal, qtdPessoas, tipo, 0);
}

void listarPessoasPorSexo(Matriculado vetorLocal[], int qtdPessoas, int tipo, char sexo) {
    printf("\n--- Lista de %s(s) por sexo: %c ---\n", tipo == ALUNO ? "Aluno" : "Professor", toupper((unsigned char)sexo));
    int achou = 0;
    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].ativo == 1 && vetorLocal[i].tipo == tipo && toupper((unsigned char)vetorLocal[i].sexo) == toupper((unsigned char)sexo)) {
            printf("Mat: %d | Nome: %s\n", vetorLocal[i].matricula, vetorLocal[i].nome);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum registro encontrado.\n");
}

void listarAniversariantesDoMes(Matriculado vetorLocal[], int qtdPessoas) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int mes = tm.tm_mon + 1;

    printf("\n--- Aniversariantes do mês atual (%02d) (Item 15) ---\n", mes);
    int achou = 0;
    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].ativo == 1 && vetorLocal[i].dataNascimento.mes == mes) {
            printf("%s (%s) - %02d/%02d/%04d (mat %d)\n", vetorLocal[i].nome,
                   vetorLocal[i].tipo == ALUNO ? "Aluno" : "Professor",
                   vetorLocal[i].dataNascimento.dia, vetorLocal[i].dataNascimento.mes,
                   vetorLocal[i].dataNascimento.ano, vetorLocal[i].matricula);
            achou = 1;
        }
    }
    if (!achou) printf("Nenhum aniversariante no mês.\n");
}

void buscarPessoasPorString(Matriculado vetorLocal[], int qtdPessoas, char busca[]) {
    printf("\n--- Busca por nome (Item 16) ---\n");
    if (strlen(busca) < 3) {
        printf("A busca requer no mínimo 3 letras.\n");
        return;
    }

    int achou = 0;
    for (int i = 0; i < qtdPessoas; i++) {
        if (vetorLocal[i].ativo == 1) {
            if (strcasestr_custom(vetorLocal[i].nome, busca) != NULL) {
                printf("%s | Mat: %d | Nome: %s\n",
                       vetorLocal[i].tipo == ALUNO ? "Aluno" : "Professor",
                       vetorLocal[i].matricula, vetorLocal[i].nome);
                achou = 1;
            }
        }
    }
    if (!achou) printf("Nenhuma pessoa encontrada com '%s' no nome.\n", busca);
}

int contarDisciplinasMatriculadas(int matricula) {
    int count = 0;
    for (int d = 0; d < qtdDisciplinas; d++) {
        if (!disciplinas[d].ativo) continue;
        for (int k = 0; k < disciplinas[d].qtdAlunos; k++) {
            if (disciplinas[d].alunosMatriculados[k] == matricula) {
                count++;
                break;
            }
        }
    }
    return count;
}

int listarAlunosMenos3Disciplinas() {
    int encontrados = 0;
    printf("\n--- Alunos matriculados em menos de 3 disciplinas (Item 17) ---\n");
    for (int i = 0; i < qtdMatriculados; i++) {
        if (vetor[i].ativo == 1 && vetor[i].tipo == ALUNO) {
            int qtd = contarDisciplinasMatriculadas(vetor[i].matricula);
            if (qtd < 3) {
                printf("Mat: %d | Nome: %s | Disciplinas: %d\n", vetor[i].matricula, vetor[i].nome, qtd);
                encontrados++;
            }
        }
    }
    if (encontrados == 0) {
        printf("Nenhum aluno encontrado com menos de 3 disciplinas.\n");
    }
    return encontrados;
}

void listarDisciplinasVagasExcedidas() {
    int achou = 0;
    printf("\n--- Disciplinas que extrapolam %d vagas (Item 18) ---\n", VAGAS_LIMITE);
    for (int i = 0; i < qtdDisciplinas; i++) {
        if (disciplinas[i].ativo == 1 && disciplinas[i].qtdAlunos > VAGAS_LIMITE) {
            printf("Código: %s | Nome: %s | Professor: %s | Alunos: %d\n",
                   disciplinas[i].codigo, disciplinas[i].nome, disciplinas[i].professor, disciplinas[i].qtdAlunos);
            achou = 1;
        }
    }
    if (!achou) {
        printf("Nenhuma disciplina excede %d vagas.\n", VAGAS_LIMITE);
    }
}


// ===================== FUNCAO PRINCIPAL (MAIN) =====================

int main(void) {
    int opcao;
    int sair = 0;

    // Inicializa arrays de disciplinas
    for (int i = 0; i < MAX_DISCI; i++) {
        disciplinas[i].ativo = 0;
        disciplinas[i].qtdAlunos = 0;
        for (int j = 0; j < MAX_ALUNOS_DISCI; j++) disciplinas[i].alunosMatriculados[j] = -1;
    }

    while (!sair) {
        opcao = menuGeral();
        if (opcao == -1) { printf("Opção inválida.\n"); continue; }

        switch (opcao) {
            case 0: sair = 1; break;
            
            case 1: { // ALUNO
                int sairAluno = 0;
                int opcaoAluno;
                while (!sairAluno) {
                    opcaoAluno = menuMatriculados("Aluno"); 
                    switch (opcaoAluno) {
                        case 0: sairAluno = 1; break;
                        case 1: cadastrarPessoa(vetor, &qtdMatriculados, ALUNO); break;
                        case 2: listarPessoas(vetor, qtdMatriculados, ALUNO); break;
                        case 3: atualizarPessoa(vetor, qtdMatriculados, ALUNO); break;
                        case 4: excluirPessoa(vetor, &qtdMatriculados, ALUNO); break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;
            }
            
            case 2: { // PROFESSOR
                int sairProf = 0;
                int opcaoProf;
                while (!sairProf) {
                    opcaoProf = menuMatriculados("Professor");
                    switch (opcaoProf) {
                        case 0: sairProf = 1; break;
                        case 1: cadastrarPessoa(vetor, &qtdMatriculados, PROFESSOR); break;
                        case 2: listarPessoas(vetor, qtdMatriculados, PROFESSOR); break;
                        case 3: atualizarPessoa(vetor, qtdMatriculados, PROFESSOR); break;
                        case 4: excluirPessoa(vetor, &qtdMatriculados, PROFESSOR); break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;
            }
            
            case 3: { // DISCIPLINA
                int sairDisciplina = 0, opcaoDisciplina;
                while (!sairDisciplina) {
                    opcaoDisciplina = menuDisciplina();
                    switch (opcaoDisciplina) {
                        case 0: sairDisciplina = 1; break;
                        case 1: cadastrarDisciplina(disciplinas, &qtdDisciplinas); break;
                        // CORREÇÃO: Chamada para a função recém-implementada
                        case 2: listarDisciplina(disciplinas, qtdDisciplinas); break; 
                        case 3: excluirDisciplina(disciplinas, &qtdDisciplinas); break;
                        
                        case 4: { // INSERIR ALUNO (Matricula)
                            char cod[10];
                            int mat;
                            printf("Código da disciplina: "); scanf(" %9s", cod);
                            printf("Matrícula do aluno: "); if (scanf("%d", &mat) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); break; }
                            int r = inserirAlunoDisciplina(disciplinas, qtdDisciplinas, cod, mat);
                            if (r == CAD_SUCESSO) printf("Aluno inserido na disciplina.\n");
                            else if (r == LISTA_CHEIA) printf("Disciplina lotada.\n");
                            else if (r == MATRICULA_INEXISTENTE) printf("Aluno inexistente ou inativo.\n");
                            else if (r == DISCIPLINA_INEXISTENTE) printf("Disciplina não encontrada.\n");
                            else if (r == 0) printf("Aluno já matriculado.\n");
                            break;
                        }
                        case 5: { // REMOVER ALUNO (Desmatricula)
                            char cod[10];
                            int mat;
                            printf("Código da disciplina: "); scanf(" %9s", cod);
                            printf("Matrícula do aluno: "); if (scanf("%d", &mat) != 1) { while(getchar()!='\n'); printf("Entrada inválida.\n"); break; }
                            int r = excluirAlunoDisciplina(disciplinas, qtdDisciplinas, cod, mat);
                            if (r == EXCLUSAO_SUCESSO) printf("Aluno removido da disciplina.\n");
                            else if (r == MATRICULA_INEXISTENTE) printf("Aluno não encontrado nesta disciplina.\n");
                            else if (r == DISCIPLINA_INEXISTENTE) printf("Disciplina não encontrada.\n");
                            break;
                        }
                        case 6: // ATUALIZAR DISCIPLINA
                            atualizarDisciplina(disciplinas, qtdDisciplinas);
                            break;
                        case 7: // LISTAR DISCIPLINA COMPLETA 
                            listarDisciplinaCompleta(disciplinas, qtdDisciplinas, vetor, qtdMatriculados);
                            break;
                        default: printf("Opção inválida!\n");
                    }
                }
                break;
            }
            
            case 4: { // LISTAGENS E RELATORIOS
                int sairLista = 0, opcaoLista;
                while (!sairLista) {
                    opcaoLista = menuListagens();
                    switch (opcaoLista) {
                        case 0: sairLista = 1; break;
                        case 1: listarAniversariantesDoMes(vetor, qtdMatriculados); break; 
                        case 2: listarDisciplinasVagasExcedidas(); break;                 
                        case 3: listarAlunosMenos3Disciplinas(); break;                

                        case 4: { // LISTAGENS DE PESSOAS (Por Sexo/Busca - Submenu)
                            int sairLP = 0, opLP;
                            while (!sairLP) {
                                opLP = menuListagensPessoas();
                                char sexo;
                                char busca[NOME_SIZE];
                                switch (opLP) {
                                    case 0: sairLP = 1; break;
                                    case 1: printf("Sexo (M/F): "); scanf(" %c", &sexo); listarPessoasPorSexo(vetor, qtdMatriculados, ALUNO, sexo); break;     // Item 9
                                    case 2: printf("Sexo (M/F): "); scanf(" %c", &sexo); listarPessoasPorSexo(vetor, qtdMatriculados, PROFESSOR, sexo); break; // Item 12
                                    case 3: printf("Digite 3+ letras para buscar: "); scanf(" %[^\n]", busca); buscarPessoasPorString(vetor, qtdMatriculados, busca); break; // Item 16
                                    default: printf("Opção inválida!\n");
                                }
                            }
                            break;
                        }

                        case 5: { // ORDENACAO DE PESSOAS (Submenu)
                            int sairOrd = 0, opOrd;
                            while (!sairOrd) {
                                opOrd = menuOrdenacao();
                                switch (opOrd) {
                                    case 0: sairOrd = 1; break;
                                    case 1: ordenarPessoasPorNome(vetor, qtdMatriculados, ALUNO); break;              
                                    case 2: ordenarPessoasPorDataNascimento(vetor, qtdMatriculados, ALUNO); break;    
                                    case 3: ordenarPessoasPorNome(vetor, qtdMatriculados, PROFESSOR); break;          
                                    case 4: ordenarPessoasPorDataNascimento(vetor, qtdMatriculados, PROFESSOR); break;
                                    default: printf("Opção inválida!\n");
                                }
                            }
                            break;
                        }
                        default: printf("Opção inválida!\n");
                    }
                }
                break;
            }
            default:
                printf("Opção inválida!\n");
        }
    }
    printf("Programa finalizado.\n");
    return 0;
}
