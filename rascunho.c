//oque falta:
//validacao de cpf e de matricula/gerador de matricula
//atualizar o CRUD pessoa

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // para verificar se a entrada tem exclusivamente numeros ou exclusivamente letras
#include <time.h>

#define MAX_PESSOAS 200
#define MAX_DISCI 100
#define MAX_ALUNOS_DISCI 50
#define NOME_SIZE 100
#define CPF_SIZE 15

#define ALUNO 1 
#define PROFESSOR 2

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
} Data;

typedef struct {
    int matricula;
    char nome[NOME_SIZE];
    char sexo;
    Data dataNascimento; //usa o struct data
    char cpf[CPF_SIZE];
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

//vetores globais >>> sao envolvidos em todo codigo, nao em uma funcao especifica
Matriculado vetor[MAX_PESSOAS]; //vetor do struct Matriculados 
Disciplina disciplinas[MAX_DISCI]; //vetor do struct disciplinas
int qtdMatriculados = 0;
int qtdDisciplinas = 0;

//prototipos das funcoes

//funcoes menu
int menuGeral();
int menuListagens();
int menuCadastro();

//funcoes CRUD pessoa
int cadastrarPessoa(Matriculado vetor[], int qtdPessoas, int tipo);
int buscarPessoa(Matriculado vetor[], int qtdPessoas, int matricula); // Adicionado para facilitar CRUD
void listarPessoas(Matriculado vetor[], int qtdPessoas, int tipo);
int atualizarPessoa(Matriculado vetor[], int qtdPessoas, int tipo);
int excluirPessoa(Matriculado vetor[], int qtdPessoas, int tipo);

//funcoes CRUD disciplina
int cadastrarDisciplina(Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinas(Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinaCompleta(Disciplina disciplinas[], int qtdDisciplinas, Matriculado vetor[], int qtdPessoas);
int inserirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);
int excluirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);

//funcoes de validacao
int ehBissexto(int ano);
int validar_data(char data[]);
int validar_nome(char nome[]);
int validar_sexo(char sexo);
int validar_matricula(int matricula);  
int validar_CPF(char cpf[]);
//revisar isso
int validar_codigoDisciplina(int codigo);

//funcoes de listagem
void listarPessoasPorSexo(Matriculado vetor[], int qtdPessoas, int tipo, char sexo);
void ordenarPessoasPorNome(Matriculado vetor[], int qtdPessoas, int tipo);
void ordenarPessoasPorDataNascimento(Matriculado vetor[], int qtdPessoas, int tipo);
void listarAniversariantesDoMes(Matriculado vetor[], int qtdPessoas);
void buscarPessoasPorString(Matriculado vetor[], int qtdPessoas, char busca[]);
void listarAlunosMenos3Disciplinas(Matriculado vetor[], int qtdPessoas, Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinasVagasExcedidas(Disciplina disciplinas[], int qtdDisciplinas, Matriculado professores[], int qtdProfessores);

//funcoes implementadas

//funcoes de validacao
int ehBissexto(int ano){
    if((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) return 1;
    else return 0;
}

int validar_data(Data d){ //Data d?
    if(d.ano < 1900 || d.ano > 2025) return 0;
    if(d.mes < 1 || d.mes > 12) return 0;
    int diasPorMes[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(d.mes == 2 && ehBissexto(d.ano)) diasPorMes[2] = 29;
    if(d.dia < 1 || d.dia > diasPorMes[d.mes]) return 0;
    return 1;
}

int validar_nome(char nome[]){
    if(strlen(nome) < 3) return 0; // se o nome for menor que tres, return 0
    for(i = 0; nome[i] != '\0'; i++){
        if(!isalpha(nome[i]) && nome[i] != ' ') return 0; //se o nome tiver caracteres que nao sao letras ou espacos
    }
    return 1; // retorna 1 se o nome eh valido
}

int validar_sexo(char sexo){
    if(!(sexo == 'M' || sexo == 'F' || sexo == 'm' || sexo == 'f')) return 0;
    else return 1;
}

int validar_cpf(char cpf[]){ 
    int i, j, digito_v1, digito_v2, soma, resto;
    char cpf_limpo[12]; // 9 digitos (XXX XXX XXX) + dois digitos de verificacao e o \0
    if(strlen(cpf) != 14) return 0;
    if(cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') return 0;
    for(i = 0; i < 14; i++){
        if(i == 3 || i == 7 || i == 11) continue; pula os separadores
        if(!isdigit(cpf[i])) return 0;
    }
    j = 0;
    for(i = 0; i < 14; i++){
        if(isdigit(cpf[i])) cpf_limpo[j++] = cpf[i];
        else if(cpf[i] == '.' || cpf[i] == '-') return 0;
    }
    cpf_limpo[11] = '\0';
    if(j != 11) return 0; //se nao tiver 11 digitos, algo deu errado
    for(i = 0; i < j - 1; i++){
        if(cpf_limpo[i] == cpf_limpo[i + 1] && cpf_limpo[i] == cpf_limpo[0]) return 0; //verifica sequencia de digitos iguais (111.111.111-11)
    }
    //digito verificador 1:
    soma = 0;
    for(i = 0, j = 10; i < 9; i++, j--){ // multiplica os digitos de 10 a dois
        soma += (cpf_limpo[i] - '0') * j; // transforma o char em int e multiplica
    }
}

int validar_matricula(int matricula){ //revisar pos adicionar o gerador de matricula
    if(matricula > 0) return 1;
    else return 0;
}

int validar_data(char data[]){
    if(strlen(data) != 10) return 0; //se a data nao tiver 10 digitos (dd/mm/aaaa), retorna falso
    if(data[2] != '/' || data[5] != '/') return 0; //verifica a formatacao/uso da barra
    //verificar se os caracteres inseridos sao digitos
    for(i = 0; i < 10; i++){
        if(i != 2 && i != 5){
            if(!isdigit(data[i])) return 0;
            else return 1;
        } 
    }
}


//editar a parte de aluno para funcionar para Matriculados

// prototipos das funcoes alunos
int menuGeral();
int menuAluno();
int cadastrarAluno(Aluno listaAluno[], int qtdAluno);
void listarAluno(Aluno listaAluno[], int qtdAluno);
int atualizarAluno(Aluno listaAluno[], int qtdAluno);
int excluirAluno(Aluno listaAluno[], int qtdAluno);

//funcao menu principal
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

// funcoes alunos 
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
            return MATRICULA_INEXISTENTE;
    }
}
