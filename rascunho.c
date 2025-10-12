//oque falta:
// revisar o CRUD pessoa
// adicionar funcionalidades ao modulo da disciplina 
// terminar as listagens

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // para verificar se a entrada tem exclusivamente numeros ou exclusivamente letras
#include <time.h>

#define ALUNO 1 
#define PROFESSOR 2

#define MAX_PESSOAS 200
#define MAX_DISCI 100
#define MAX_ALUNOS_DISCI 50
#define NOME_SIZE 100
#define CPF_SIZE 15
#define MATRICULA_INICIAL 1000
//codigos de retorno
#define LISTA_CHEIA 1 
#define LISTA_VAZIA 0
#define CAD_SUCESSO 1
#define MATRICULA_INVALIDA -1
#define ATUALIZACAO_SUCESSO 2
#define EXCLUSAO_SUCESSO 3
#define MATRICULA_INEXISTENTE -3

//revisar data de nascimento
typedef struct{
  int dia, mes, ano;
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
    char nome[100]; // falta
    char codigo[10]; // Ex: "INFO201"
    int semestre; // falta
    char professor[50];
    int alunosMatriculados[MAX_ALUNOS_DISCI]; // falta
    int qtdAlunos; //falta
    int ativo;
} Disciplina;

//vetores globais >>> sao envolvidos em todo codigo, nao em uma funcao especifica
Matriculado vetor[MAX_PESSOAS]; //vetor do struct Matriculados 
Disciplina disciplinas[MAX_DISCI]; //vetor do struct disciplinas >>> REVER
int proximaMatricula = MATRICULA_INICIAL; 
int qtdMatriculados = 0;
int qtdDisciplinas = 0;
int i = 0;

//funcoes menu
int menuGeral();
int menuListagens();
int menuMatriculado();
int menuDisciplina();

//funcoes CRUD pessoa
int cadastrarPessoa(Matriculado lista[TAM], int *qtd, int tipo);
int excluirPessoa(Matriculado vetor[], int qtdPessoas, int tipo);
int atualizarPessoa(Matriculado vetor[], int qtdPessoas, int tipo);

//funcoes CRUD disciplina
int cadastrarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina); // feito
void listarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina); // feito
int excluirDisciplina(Disciplina listaDisciplina[], int qtdDisciplina); // feito
//falta:
void listarDisciplinaCompleta(Disciplina disciplinas[], int qtdDisciplinas, Matriculado vetor[], int qtdPessoas);
int inserirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);
int excluirAlunoDisciplina(Disciplina disciplinas[], int qtdDisciplinas, int codDisciplina, int matAluno);

//funcoes de validacao
int ehBissexto(int ano);
int validar_data(Data d);
int validar_nome(char nome[]);
int validar_sexo(char sexo);
int validar_matricula(int matricula);  
int validar_CPF(char cpf[]);
//revisar isso
int validar_codigoDisciplina(int codigo);

//funcoes de listagem FALTA
void listarPessoasPorSexo(Matriculado vetor[], int qtdPessoas, int tipo, char sexo);
void ordenarPessoasPorNome(Matriculado vetor[], int qtdPessoas, int tipo);
void ordenarPessoasPorDataNascimento(Matriculado vetor[], int qtdPessoas, int tipo);
void listarAniversariantesDoMes(Matriculado vetor[], int qtdPessoas);
void buscarPessoasPorString(Matriculado vetor[], int qtdPessoas, char busca[]);
void listarAlunosMenos3Disciplinas(Matriculado vetor[], int qtdPessoas, Disciplina disciplinas[], int qtdDisciplinas);
void listarDisciplinasVagasExcedidas(Disciplina disciplinas[], int qtdDisciplinas, Matriculado professores[], int qtdProfessores);
int buscarPessoa(Matriculado vetor[], int qtdPessoas, int matricula); // Adicionado para facilitar CRUD
void listarPessoas(Matriculado vetor[], int qtdPessoas, int tipo);

//funcoes implementadas + modulos

int menuGeral() {
    int opcao;
    printf("\nMENU GERAL \n");
    printf("0 - Sair\n");
    printf("1 - Aluno\n");
    printf("2 - Professor\n");
    printf("3 - Disciplina\n");
    printf("4 - Listagens\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    return opcao;
}

int main(void) {
    int opcao;
    int sair = 0;

    while (!sair) {
        opcao = menuGeral();

        switch (opcao) {
            case 0:
                sair = 1;
                break;
            // ===================== ALUNO =====================
            case 1: {
                int sairAluno = 0;
                int opcaoAluno;

                while (!sairAluno) {
                    opcaoAluno = menuMatriculados("Aluno");

                    switch (opcaoAluno) {
                        case 0:
                            sairAluno = 1;
                            break;

                        case 1: { // CADASTRAR ALUNO
                            int retorno = cadastrarPessoa(NULL, vetor, &qtdMatriculados, ALUNO);
                            if (retorno == LISTA_CHEIA)
                                printf("Lista de alunos cheia!\n");
                            else if (retorno == CAD_SUCESSO)
                                printf("Aluno cadastrado com sucesso!\n");
                            break;
                        }

                        case 2: // LISTAR ALUNOS
                            listarPessoas(vetor, qtdMatriculados, ALUNO);
                            break;

                        case 3: { // ATUALIZAR ALUNO
                            int retorno = atualizarPessoa(vetor, qtdMatriculados, ALUNO);
                            if (retorno == ATUALIZACAO_SUCESSO)
                                printf("Aluno atualizado com sucesso!\n");
                            else
                                printf("Matrícula inexistente ou inválida!\n");
                            break;
                        }

                        case 4: { // EXCLUIR ALUNO
                            int retorno = excluirPessoa(vetor, qtdMatriculados, ALUNO);
                            if (retorno == EXCLUSAO_SUCESSO)
                                printf("Aluno excluído com sucesso!\n");
                            else
                                printf("Erro ao excluir aluno!\n");
                            break;
                        }

                        default:
                            printf("Opção inválida!\n");
                    }
                }
                break;
            }
            // ===================== PROFESSOR =====================
            case 2: {
                int sairProf = 0;
                int opcaoProf;

                while (!sairProf) {
                    opcaoProf = menuMatriculados("Professor");

                    switch (opcaoProf) {
                        case 0:
                            sairProf = 1;
                            break;

                        case 1:
                            cadastrarPessoa(NULL, vetor, &qtdMatriculados, PROFESSOR);
                            break;

                        case 2:
                            listarPessoas(vetor, qtdMatriculados, PROFESSOR);
                            break;

                        case 3:
                            atualizarPessoa(vetor, qtdMatriculados, PROFESSOR);
                            break;

                        case 4:
                            excluirPessoa(vetor, qtdMatriculados, PROFESSOR);
                            break;

                        default:
                            printf("Opção inválida!\n");
                    }
                }
                break;
            }
            // ===================== DISCIPLINA =====================
            case 3: {
                case 3: { 
                int sairDisciplina = 0, opcaoDisciplina;
                while (!sairDisciplina) {
                    opcaoDisciplina = menuDisciplina();
                    switch (opcaoDisciplina) {
                        case 0:
                            sairDisciplina = 1;
                            break;
                        case 1: {
                            int retorno = cadastrarDisciplina(listaDisciplina, qtdDisciplina);
                            if (retorno == LISTA_CHEIA)
                                printf("Lista de disciplinas cheia!\n");
                            else if (retorno == CODIGO_INVALIDO)
                                printf("Código de disciplina inválido ou já cadastrado!\n");
                            else if (retorno == CAD_DISCIPLINA_SUCESSO) {
                                printf("Disciplina cadastrada com sucesso!\n");
                                qtdDisciplina++;
                            }
                            break;
                        }
                        case 2:
                            listarDisciplina(listaDisciplina, qtdDisciplina);
                            break;
                        case 3: {
                            int retorno = excluirDisciplina(listaDisciplina, qtdDisciplina);
                            if (retorno == DISCIPLINA_INEXISTENTE)
                                printf("Disciplina inexistente!\n");
                            else if (retorno == EXCLUSAO_DISCIPLINA_SUCESSO) {
                                printf("Disciplina excluída com sucesso!\n");
                                qtdDisciplina--;
                            }
                            break;
                        }
                        default:
                            printf("Opção inválida!\n");
                    }
                }
                break;
            }
            
            case 4: { // Listagens
    int sairListagem = 0, opcaoListagem;
    while (!sairListagem) {
        opcaoListagem = menuListagens();
        switch(opcaoListagem) {
            case 0:
                sairListagem = 1;
                break;
            case 1:
           // fazer     listarAniversariantesDoMes
                break;
            case 2:
             //   listarDisciplinasVagasExcedidas(listaDisciplina, qtdDisciplina, listaAluno, qtdAluno);
                break;
            case 3:
          //      listarAlunosMenos3Disciplinas(listaAluno, qtdAluno, listaDisciplina, qtdDisciplina);
                break;
            default:
                printf("Opção inválida!\n");
        }
    }
    break;
}

            default:
                printf("Opção inválida!\n");
        }
    }

    printf("\nConsulta finalizada.\n");
    return 0;
}



// MODULO MATRICULADOS
int menuMatriculados(char c[15]) {
    int opcao;
    printf("\nMENU MATRICULADOS\n");
    printf("0 - Voltar %s\n", c);
    printf("1 - Cadastrar %s\n", c);
    printf("2 - Listar %s\n", c);
    printf("3 - Atualizar %s\n", c);
    printf("4 - Excluir %s\n", c);
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    return opcao;
}

int cadastrarPessoa(Matriculado lista[], int *qtd, int tipo) {
    if (*qtd >= TAM) {  // Limite máximo de cadastros
        return LISTA_CHEIA;
    }

    Matriculado novaPessoa;
    novaPessoa.matricula = gerarMatricula();  
    novaPessoa.tipo = tipo;
    novaPessoa.ativo = 1;
    } else {
        printf("Cadastro de %s\n", tipo == ALUNO ? "Aluno" : "Professor");
        printf("Matricula gerada: %d\n", novaPessoao.matricula);
        do{
            printf("Insira o nome: ");
            scanf(" %[^\n]", temp_nome);
            if(!validar_nome(temo_nome))
                printf("Nome invalido (min. 3 letras, apenas letras e espacos.\n");
        } while (!validar_nome(temp_nome));
        strcpy(novaPessoa.nome, temp_nome);
        do{
            printf("Insira o sexo: ");
            scanf(" %c", &temp_sexo);
            if(!validar_sexo(temp_sexo))
                printf("Sexo inválido. Digite M ou F.")
        } while (!validar_sexo(temp_sexo));
        novaPessoa.sexo = toupper(temp_sexo); 
        do{
            printf("Insira a data de nascimento (dd/mm/aaaa): ");
            scanf(" %[^\n]", temp_data_str);
            
            int campos_lidos = sscanf(temp_data_str, "%d%d%d"
                                &novaPessoa.dataNascimento.dia, 
                                &novaPessoa.dataNascimento.mes, 
                                &novaPessoa.dataNascimento.ano);
            if(campos_lidos != 3){
                printf("Formato invalido. Use dd/mm/aaaa\n");
                data_valida = 0;
                continue;
            }
            data_valida = validar_data(novaPessoa.dataNascimento);
            if(!data_valida) printf("Data invalida ou fora dos limites.\n");
        } while(!data_valida);
        do{
            printf("Insira o CPF (XXX.XXX.XXX-XX): ");
            scanf(" %s", temp_cpf);
            if(!validar_cpf(temp_cpf)) printf("CPF invalido (formato ou digitos verificadores incorretos).\n");
        } while (!validar_cpf(temp_cpf));
        strcpy(novaPessoa.cpf, temp_cpf);
        
        novaPessoa.tipo = tipo;
        novaPessoa.ativo = 1;
        
        vetor[qtdMatriculados] = novaPessoa;
        qtdMatriculados++;
        
        printf("\n%s cadastrado com sucesso! Matrícula: %d\n", tipo == ALUNO ? "Aluno" : "Professor", novaPessoa.matricula);
        return CAD_SUCESSO;
    }
}

int excluirPessoa(Matriculado vetor[], int qtdPessoas, int tipo){
    int matricula_busca;
    
    printf("Exclusao de %s\n", tipo == ALUNO ? "Aluno" : "Professor");
    printf("Digite a matricula a excluir: ");
    if(scanf("%d", &matricula_busca) != 1){
        while(getchar() != '\n');
        printf("Entrada invalida.\n");
        return -1;
    }
    
     int indice = buscarPessoa(vetor, qtdPessoas, matricula_busca);

   if (indice == -1 || vetor[indice].ativo == 0) {
    printf("Matrícula não encontrada ou já inativa.\n");
    return MATRICULA_INEXISTENTE;
}

    if (vetor[indice].tipo != tipo) {
         printf("Erro: Matrícula encontrada, mas não é um %s.\n", tipo == ALUNO ? "Aluno" : "Professor");
         return -1;
    }

    vetor[indice].ativo = 0; 
    printf("%s com matrícula %d excluído (logicamente) com sucesso.\n", 
           tipo == ALUNO ? "Aluno" : "Professor", 
           matricula_busca);
           
    return EXCLUSAO_SUCESSO;
}
    
int atualizarPessoa(Matriculado vetor[], int qtdPessoas, int tipo){
    int matricula_busca;
    int indice;
    int opcao;
    
    printf("Atualizacao de cadastro de %s\n", tipo == ALUNO ? "Aluno" : "Professor");
    printf("Digite a matricula do %s\n a ser atualizado: ", tipo == ALUNO ? "Aluno" : "Professor");
    if(scanf("%d", &matricula_busca) != 1){
        while(getchar() != '\n');
        printf("Entrada invalida.\n");
        return -1;
    }
    
     indice = buscarPessoa(vetor, qtdPessoas, matricula_busca);
     
     indice = buscarPessoa(vetor, qtdPessoas, matricula_busca);

    if (indice == -1 || vetor[indice].tipo != tipo) {
        printf("Erro: Matrícula não encontrada, inativa ou tipo incorreto.\n");
        return MATRICULA_INEXISTENTE;
    }

    // A pessoa foi encontrada no índice 'indice'
    Matriculado *p = &vetor[indice]; // Ponteiro para facilitar a manipulação

    printf("\n%s encontrado: %s\n", tipo == ALUNO ? "Aluno" : "Professor", p->nome);
    
    do {
        printf("\nQual campo deseja atualizar?\n");
        printf("1. Nome: %s\n", p->nome);
        printf("2. Sexo: %c\n", p->sexo);
        printf("3. Data de Nascimento: %02d/%02d/%d\n", p->dataNascimento.dia, p->dataNascimento.mes, p->dataNascimento.ano);
        printf("4. CPF: %s\n", p->cpf);
        printf("0. Finalizar Atualização\n");
        printf("Opção: ");

        // Leitura de opção de forma segura
        opcao = ler_opcao(0, 4);

        char temp_str[100]; // Variável temporária para Nome/CPF/Data
        Data temp_data;
        int valida;

        switch (opcao) {
            case 1: // ATUALIZAR NOME
                do {
                    printf("Novo Nome: ");
                    scanf(" %[^\n]", temp_str);
                    if ((valida = validar_nome(temp_str))) {
                        strcpy(p->nome, temp_str);
                        printf("Nome atualizado com sucesso.\n");
                    } else {
                        printf("Nome inválido.\n");
                    }
                } while (!valida);
                break;

            case 2: // ATUALIZAR SEXO
                do {
                    printf("Novo Sexo (M/F): ");
                    scanf(" %c", &temp_str[0]); // Lê o caractere
                    if ((valida = validar_sexo(temp_str[0]))) {
                        p->sexo = toupper(temp_str[0]);
                        printf("Sexo atualizado com sucesso.\n");
                    } else {
                        printf("Sexo inválido.\n");
                    }
                } while (!valida);
                break;
                
            case 3: // ATUALIZAR DATA DE NASCIMENTO
                do {
                    printf("Nova Data (DD/MM/AAAA): ");
                    scanf(" %[^\n]", temp_str);
                    
                    int campos_lidos = sscanf(temp_str, "%d/%d/%d", 
                                             &temp_data.dia, &temp_data.mes, &temp_data.ano);

                    if (campos_lidos != 3 || !(valida = validar_data(temp_data))) {
                        printf("Data inválida ou formato incorreto (DD/MM/AAAA).\n");
                        valida = 0;
                    } else {
                        p->dataNascimento = temp_data;
                        printf("Data de nascimento atualizada com sucesso.\n");
                    }
                } while (!valida);
                break;
                
            case 4: // ATUALIZAR CPF
                do {
                    printf("Novo CPF (XXX.XXX.XXX-XX): ");
                    scanf(" %s", temp_str);
                    if ((valida = validar_CPF(temp_str))) {
                        strcpy(p->cpf, temp_str);
                        printf("CPF atualizado com sucesso.\n");
                    } else {
                        printf("CPF inválido.\n");
                    }
                } while (!valida);
                break;

            case 0:
                printf("Atualização finalizada.\n");
                return ATUALIZACAO_SUCESSO;
            
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return ATUALIZACAO_SUCESSO;
}

int ler_opcao(int min, int max) {
    int opcao;
    if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n'); 
        return -1; 
    }
    if (opcao < min || opcao > max) {
        return -1;
    }
    return opcao;
}

// MODULO DISCIPLINA
int menuDisciplina() {
    int opcao;
    printf("\nMENU DISCIPLINA \n");
    printf("0 - Voltar\n");
    printf("1 - Cadastrar Disciplina\n");
    printf("2 - Listar Disciplinas\n");
    printf("3 - Excluir Disciplina\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    return opcao;
}

int cadastrarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina) {
    if (qtdDisciplina == TAM_DISCIPLINA) {
        return LISTA_CHEIA;
    }

    char codigo[10];
    int semestre;
    char professor[50];

    printf("Digite o código da disciplina (ex: INFO201): ");
    scanf("%s", codigo);

    // Verifica duplicidade
    for (int i = 0; i < qtdDisciplina; i++) {
        if (strcmp(listaDisciplina[i].codigo, codigo) == 0 && listaDisciplina[i].ativo == 1)
            return CODIGO_INVALIDO;
    }

    printf("Digite o semestre (ex: 1, 2, 3...): ");
    scanf("%d", &semestre);

    printf("Digite o nome do professor: ");
    scanf(" %[^\n]", professor);

    strcpy(listaDisciplina[qtdDisciplina].codigo, codigo);
    listaDisciplina[qtdDisciplina].semestre = semestre;
    strcpy(listaDisciplina[qtdDisciplina].professor, professor);
    listaDisciplina[qtdDisciplina].ativo = 1;

    return CAD_DISCIPLINA_SUCESSO;
}

void listarDisciplina(Disciplina listaDisciplina[], int qtdDisciplina) {
    printf("\n--- Lista de Disciplinas ---\n");
    if (qtdDisciplina == 0) {
        printf("Nenhuma disciplina cadastrada.\n");
    } else {
        for (int i = 0; i < qtdDisciplina; i++) {
            if (listaDisciplina[i].ativo == 1)
                printf("Código: %s | Semestre: %d | Professor: %s\n",
                       listaDisciplina[i].codigo,
                       listaDisciplina[i].semestre,
                       listaDisciplina[i].professor);
        }
    }
}

int excluirDisciplina(Disciplina listaDisciplina[], int qtdDisciplina) {
    char codigo[10];
    printf("Digite o código da disciplina a excluir: ");
    scanf("%s", codigo);

    for (int i = 0; i < qtdDisciplina; i++) {
        if (strcmp(listaDisciplina[i].codigo, codigo) == 0 && listaDisciplina[i].ativo == 1) {
            listaDisciplina[i].ativo = -1;

            for (int j = i; j < qtdDisciplina - 1; j++) {
                listaDisciplina[j] = listaDisciplina[j + 1];
            }

            return EXCLUSAO_DISCIPLINA_SUCESSO;
        }
    }

    return DISCIPLINA_INEXISTENTE;
}

// MODULO LISTAGENS
int menuListagens() {
    int opcao;
    printf("\nMENU LISTAGENS \n");
    printf("0 - Voltar\n");
    printf("1 - Aniversariantes do mês\n");
    printf("2 - Disciplinas com mais de 40 alunos\n");
    printf("3 - Alunos matriculados em menos de 3 disciplinas\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    return opcao;
}


//funcoes auxiliares
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
    for(int i = 0; nome[i] != '\0'; i++){
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
        if(i == 3 || i == 7 || i == 11) continue; //pula os separadores
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
 
    soma = 0;
    for(i = 0, j = 10; i < 9; i++, j--){ // multiplica os digitos de 10 a dois
        soma += (cpf_limpo[i] - '0') * j; // transforma o char em int e multiplica
    }
    resto = soma % 11;
    digito_v1 = (resto < 2) ? 0 : 11 - resto;
    
    soma = 0;
    for(i = 0, j = 11; i < 10; i++, j--){
        soma += (cpf_limpo[i] - '0') * j;
    }
    resto = soma % 11;
    digito_v2 = (resto < 2) ? 0 : 11 - resto;
    
    if((cpf_limpo[9] == digito_v1) && (cpf_limpo[10] == digito_v2)) return 1;
    else return 0;
}

int validar_data(char data[]){
    if(strlen(data) != 10) return 0; //se a data nao tiver 10 digitos (dd/mm/aaaa), retorna falso
    if(data[2] != '/' || data[5] != '/') return 0; //verifica a formatacao/uso da barra
    //verificar se os caracteres inseridos sao digitos
    for(int i = 0; i < 10; i++){
        if(i != 2 && i != 5){
            if(!isdigit(data[i])) return 0;
            else return 1;
        } 
    }
}

int tam_lista(int qtd) {
    if (qtd >= MAX_PESSOAS) return LISTA_CHEIA;
    if (qtd == 0) return LISTA_VAZIA;
    return 0;
}

int gerarMatricula() {
    int matriculaGerada = proximaMatricula;
    proximaMatricula++; // incrementa para a próxima pessoa
    return matriculaGerada;
}

int validar_matricula(int matricula){
    for (int i = 0; i < qtdMatriculados; i++) {
        // Checa se a matrícula existe, independente de estar ativo ou inativo
        if (vetorMatriculados[i].matricula == matricula) {
            return 0; // Matrícula DUPLICADA
        }
    }
    return 1;
}
