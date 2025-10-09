//oque falta nas funÃ§Ãµes de validaÃ§Ã£o e protÃ³tipos: adicionar alguns protÃ³tipos de funÃ§Ã£o, validaÃ§Ã£o de data e de cpf

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // para verificar se a entrada tem exclusivamente nÃºmeros ou exclusivamente letras
#include <time.h>

#define MAX_PESSOAS 200
#define MAX_DISCI 100
#define MAX_ALUNOS_DISCI 50
#define NOME_SIZE
#define CPF_SIZE 15

#define CAD_SUCESSO 1
#define MATRICULA_INVALIDA -1
#define LISTA_CHEIA -2
#define ATUALIZACAO_SUCESSO 2
#define EXCLUSAO_SUCESSO 3
#define MATRICULA_INEXISTENTE -3

//revisar data de nascimento
typedef struct{
  int dia;
  int mes;
  int ano;
}Data;
typedef struct {
    int matricula;
    char nome[100];
    char sexo;
    char dataNascimento[12];
    Data nascimento;
    char cpf[15];
    int ativo; // 1= ativo, 0 = excluido
    int tipo;  // 1 = aluno, 2 = professor
} Matriculado;

typedef struct {
    char nome[100];
    int codigo;
    int semestre;
    int professorMatricula;
    int alunosMatriculados[MAX_ALUNOS_DISCI];
    int qtdAlunos;
    int ativo;
} Disciplina;

//vetores globais >>> sÃ£o envolvidos em todo cÃ³digo, nÃ£o em uma funÃ§Ã£o especÃ­fica
Matriculado vetor[MAX_PESSOAS]; //vetor do struct Matriculados 
Disciplina disciplinas[MAX_DISCI]; //vetor do struct disciplinas
int qtdMatriculados = 0;
int qtdDisciplinas = 0;

//prototipos das funÃ§Ãµes

//funÃ§Ãµes menu
int menuGeral();
int menuListagens();
int menuCadastro();

//funÃ§Ãµes CRUD pessoa
int cadastrarPessoa(Matriculado vetor[], int qtdPessoas, int tipo);
int buscarPessoa(Matriculado vetor[], int qtdPessoas, int matricula); // Adicionado para facilitar CRUD
void listarPessoas(Matriculado vetor[], int qtdPessoas, int tipo);
int atualizarPessoa(Matriculado vetor[], int qtdPessoas, int tipo);
int excluirPessoa(Matriculado vetor[], int qtdPessoas, int tipo);

//funÃ§Ãµes CRUD disciplina
int cadastrarDisciplina(Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinas(Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinaCompleta(Disciplina disciplinas[], int qtdDisciplinas, Matriculado vetor[], int qtdPessoas);
int inserirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);
int excluirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);

//funÃ§Ãµes de validaÃ§Ã£o
int validar_nome(char nome[]);
int validar_sexo(char sexo);
int validar_matricula(int matricula); // Para verificar se Ã© positiva/vÃ¡lida 
int validar_data(char data[]);
int validar_CPF(char cpf[]);
int validar_codigoDisciplina(int codigo);

//funÃ§Ãµes de listagem
void listarPessoasPorSexo(Matriculado vetor[], int qtdPessoas, int tipo, char sexo);
void ordenarPessoasPorNome(Matriculado vetor[], int qtdPessoas, int tipo);
void ordenarPessoasPorDataNascimento(Matriculado vetor[], int qtdPessoas, int tipo);
void listarAniversariantesDoMes(Matriculado vetor[], int qtdPessoas);
void buscarPessoasPorString(Matriculado vetor[], int qtdPessoas, char busca[]);
void listarAlunosMenos3Disciplinas(Matriculado vetor[], int qtdPessoas, Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinasVagasExcedidas(Disciplina disciplinas[], int qtdDisciplinas, Matriculado professores[], int qtdProfessores);

//funÃ§Ãµes implementadas

//funÃ§Ãµes de validaÃ§Ã£o
int validar_nome(char nome[]){
    if(strlen(nome) < 3) return 0; // se o nome for menor que trÃªs, return 0
    for(i = 0; nome[i] != '\0'; i++){
        if(!isalpha(nome[i]) && nome[i] != ' ') return 0; //se o nome tiver caracteres que nÃ£o sÃ£o letras ou espaÃ§os
    }
    return 1; // retorna 1 se o nome Ã© vÃ¡lido
}

int validar_sexo(char sexo){
    if(!(sexo == 'M' || sexo == 'F' || sexo == 'm' || sexo == 'f')) return 0;
    else return 1;
}

int validar_matricula(int matricula){
    if(matricula > 0) return 1;
    else return 0;
}

int validar_data(char data[]){
    if(strlen(data) != 10) return 0; //se a data nÃ£o tiver 10 dÃ­gitos (dd/mm/aaaa), retorna falso
    if(data[2] != '/' || data[5] != '/') return 0; //verifica a formataÃ§Ã£o/uso da barra
    //verificar se os caracteres inseridos sÃ£o digitos
    for(i = 0; i < 10; i++){
        if(i != 2 && i != 5){
            if(!isdigit(data[i])) return 0;
            else return 1;
        } 
    }
}



int validar_cpf(char cpf[CPF]){
    
}


//editar a parte de aluno para funcionar para Matriculados

// protótipos das funções alunos
int menuGeral();
int menuAluno();
int cadastrarAluno(Aluno listaAluno[], int qtdAluno);
void listarAluno(Aluno listaAluno[], int qtdAluno);
int atualizarAluno(Aluno listaAluno[], int qtdAluno);
int excluirAluno(Aluno listaAluno[], int qtdAluno);

//criar gerador automatico de matricula
// novos atributos na struct aluno e leitura deles
//validação da matricula

int main(void) {
    Aluno listaAluno[TAM_ALUNO];
    int opcao;
    int qtdAluno = 0;
    int sair = 0; // falso

    while (!sair) {
        opcao = menuGeral();

        switch (opcao) {
            case 0: {
                sair = 1;
                break;
            }
            case 1: {
                printf("Módulo Aluno\n");
                int sairAluno = 0; 
                int opcaoAluno;

                while (!sairAluno) {
                    opcaoAluno = menuAluno();
                    switch (opcaoAluno) {
                        case 0: {
                            sairAluno = 1;
                            break;
                        }
                        case 1: {
                            int retorno = cadastrarAluno(listaAluno, qtdAluno);

                            if (retorno == LISTA_CHEIA)
                                printf("Lista de aluno cheia\n");
                            else if (retorno == MATRICULA_INVALIDA)
                                printf("Matrícula Inválida\n");
                            else 
                                printf("Cadastrado com sucesso\n");

                            qtdAluno++;
                            break;
                        }
                        case 2: {
                            listarAluno(listaAluno, qtdAluno);
                            break;
                        }
                        case 3: {
                            int retorno = atualizarAluno(listaAluno, qtdAluno);
                            switch (retorno) {
                                case MATRICULA_INVALIDA:
                                    printf("Matrícula Inválida\n"); 
                                    break;
                                case MATRICULA_INEXISTENTE:
                                    printf("Matrícula Inexistente\n");
                                    break;
                                case ATUALIZACAO_ALUNO_SUCESSO:
                                    printf("Aluno atualizado com sucesso\n");
                                    break;
                            }
                            break;
                        }
                        case 4: {
                            int retorno = excluirAluno(listaAluno, qtdAluno);
                            switch (retorno) {
                                case MATRICULA_INVALIDA:
                                    printf("Matrícula Inválida\n"); 
                                    break;
                                case MATRICULA_INEXISTENTE:
                                    printf("Matrícula Inexistente\n");
                                    break;
                                case EXCLUSAO_ALUNO_SUCESSO:
                                    printf("Aluno excluído com sucesso\n");
                                    qtdAluno--;
                                    break;
                            }
                            break;
                        }
                        default: {
                            printf("Opção inválida\n");
                        }
                    }
                }
                break;
            }
            case 2: {
                printf("Módulo Professor\n");
                break;
            }
            case 3: {
                printf("Módulo Disciplina\n");
                break;
            }
            default: {
                printf("Opção inválida\n");
            }
        }
    }
    return 0;
}

// funções
int menuGeral() {
    int opcao;
    printf("Projeto Escola\n");
    printf("0 - Sair\n");
    printf("1 - Aluno\n");
    printf("2 - Professor\n");
    printf("3 - Disciplina\n");
    scanf("%d", &opcao);
    return opcao;
}

int menuAluno() {
    int opcao;
    printf("0 - Voltar\n");
    printf("1 - Cadastrar Aluno\n");
    printf("2 - Listar Aluno\n");
    printf("3 - Atualizar Aluno\n");
    printf("4 - Excluir Aluno\n");
    scanf("%d", &opcao);
    return opcao;
}   

int cadastrarAluno(Aluno listaAluno[], int qtdAluno) {
    printf("Cadastrar Aluno\n");
    if (qtdAluno == TAM_ALUNO) {
        return LISTA_CHEIA;
    } else {
        printf("Digite a matrícula:\n");
        int matricula;
        scanf("%d", &matricula);

        if (matricula < 0) {
            return MATRICULA_INVALIDA;
        }
        listaAluno[qtdAluno].matricula = matricula;
        listaAluno[qtdAluno].ativo = 1;
        return CAD_ALUNO_SUCESSO;
    }
}

void listarAluno(Aluno listaAluno[], int qtdAluno) {
    printf("Listar Aluno\n");
    if (qtdAluno == 0) {
        printf("Lista de aluno vazia\n");
    } else {
        for (int i = 0; i < qtdAluno; i++) {
            if (listaAluno[i].ativo == 1)
                printf("Matrícula: %d\n", listaAluno[i].matricula);
        }
    }
}

int atualizarAluno(Aluno listaAluno[], int qtdAluno) {
    printf("Atualizar Aluno\n");
    printf("Digite a matrícula:\n");
    int matricula;
    scanf("%d", &matricula);
    int achou = 0;

    if (matricula < 0) {
        return MATRICULA_INVALIDA;
    } else {
        for (int i = 0; i < qtdAluno; i++) {
            if (matricula == listaAluno[i].matricula && listaAluno[i].ativo) {
                printf("Digite a nova matrícula:\n");
                int novamatricula;
                scanf("%d", &novamatricula);
                if (novamatricula < 0) {
                    return MATRICULA_INVALIDA;
                }
                listaAluno[i].matricula = novamatricula;
                achou = 1;
                break;
            }
        }
        if (achou)
            return ATUALIZACAO_ALUNO_SUCESSO;
        else 
            return MATRICULA_INEXISTENTE;
    }
}

int excluirAluno(Aluno listaAluno[], int qtdAluno) {
    printf("Excluir Aluno\n");
    printf("Digite a matrícula:\n");
    int matricula;
    scanf("%d", &matricula);
    int achou = 0;

    if (matricula < 0) {
        return MATRICULA_INVALIDA;
    } else {
        for (int i = 0; i < qtdAluno; i++) {
            if (matricula == listaAluno[i].matricula) {
                listaAluno[i].ativo = -1;
                for (int j = i; j < qtdAluno - 1; j++) {
                    listaAluno[j].matricula = listaAluno[j + 1].matricula;
                    listaAluno[j].sexo = listaAluno[j + 1].sexo;
                    listaAluno[j].ativo = listaAluno[j + 1].ativo;
                }
                achou = 1;
                break;
            }
        }
        if (achou)
            return EXCLUSAO_ALUNO_SUCESSO;
        else 
            return MATRICULA_INEXISTENTE;
    }
}

