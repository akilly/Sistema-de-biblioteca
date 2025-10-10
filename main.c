#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LIVROS 100
#define MAX_USUARIOS 100
#define MAX_EMPRESTIMOS 200

// ===================== Estruturas de Dados =====================
typedef struct {
    int codigo;
    char titulo[101];
    char autor[81];
    char editora[61];
    int ano;
    int exemplares;
    int emprestados; // quantidade emprestada
} Livro;

typedef struct {
    int matricula;
    char nome[101];
    char curso[51];
    char telefone[16];
    char dataCadastro[11]; // dd/mm/aaaa
} Usuario;

typedef struct {
    int codigoEmprestimo;
    int matriculaUsuario;
    int codigoLivro;
    char dataEmprestimo[11]; // dd/mm/aaaa
    char dataDevolucao[11];  // dd/mm/aaaa
    char status[10];         // ativo ou devolvido
} Emprestimo;

// ===================== Variáveis Globais =====================
Livro livros[MAX_LIVROS];
Usuario usuarios[MAX_USUARIOS];
Emprestimo emprestimos[MAX_EMPRESTIMOS];
int totalLivros = 0, totalUsuarios = 0, totalEmprestimos = 0;

// ===================== Funções Auxiliares =====================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int matriculaJaExiste(int matricula){
    for(int i=0;i<totalUsuarios;i++)
        if(usuarios[i].matricula == matricula) return 1;
    return 0;
}

int codigoLivroJaExiste(int codigo){
    for(int i=0;i<totalLivros;i++)
        if(livros[i].codigo == codigo) return 1;
    return 0;
}

// ===================== Backup =====================
void backupArquivos() {
    FILE *f;

    // backup livros
    remove("backup_livros.txt");
    f = fopen("backup_livros.txt","w");
    if(f){
        for(int i=0;i<totalLivros;i++)
            fprintf(f,"%d;%s;%s;%s;%d;%d;%d\n",
                    livros[i].codigo, livros[i].titulo, livros[i].autor,
                    livros[i].editora, livros[i].ano, livros[i].exemplares, livros[i].emprestados);
        fclose(f);
    }

    // backup usuarios
    remove("backup_usuarios.txt");
    f = fopen("backup_usuarios.txt","w");
    if(f){
        for(int i=0;i<totalUsuarios;i++)
            fprintf(f,"%d;%s;%s;%s;%s\n",
                    usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso,
                    usuarios[i].telefone, usuarios[i].dataCadastro);
        fclose(f);
    }

    // backup emprestimos
    remove("backup_emprestimos.txt");
    f = fopen("backup_emprestimos.txt","w");
    if(f){
        for(int i=0;i<totalEmprestimos;i++)
            fprintf(f,"%d;%d;%d;%s;%s;%s\n",
                    emprestimos[i].codigoEmprestimo, emprestimos[i].matriculaUsuario,
                    emprestimos[i].codigoLivro, emprestimos[i].dataEmprestimo,
                    emprestimos[i].dataDevolucao, emprestimos[i].status);
        fclose(f);
    }

    printf("[INFO] Backups atualizados com sucesso.\n");
}

// ===================== Carregar Backup =====================
void carregarBackup() {
    FILE *f;
    char linha[256];

    totalLivros = 0;
    totalUsuarios = 0;
    totalEmprestimos = 0;

    // ===================== Livros =====================
    f = fopen("backup_livros.txt","r");
    if(f){
        while(fgets(linha,sizeof(linha),f)){
            linha[strcspn(linha,"\n")] = 0;

            Livro l;
            int campos = sscanf(linha,"%d;%100[^;];%80[^;];%60[^;];%d;%d;%d",
                                &l.codigo, l.titulo, l.autor, l.editora,
                                &l.ano, &l.exemplares, &l.emprestados);
            if(campos == 7)
                livros[totalLivros++] = l;
            else
                printf("[ERRO] Linha inválida no backup_livros.txt: %s\n", linha);
        }
        fclose(f);
    }

    // ===================== Usuários =====================
    f = fopen("backup_usuarios.txt","r");
    if(f){
        while(fgets(linha,sizeof(linha),f)){
            linha[strcspn(linha,"\n")] = 0;

            Usuario u;
            int campos = sscanf(linha,"%d;%100[^;];%50[^;];%15[^;];%10s",
                                &u.matricula, u.nome, u.curso, u.telefone, u.dataCadastro);
            if(campos == 5)
                usuarios[totalUsuarios++] = u;
            else
                printf("[ERRO] Linha inválida no backup_usuarios.txt: %s\n", linha);
        }
        fclose(f);
    }

    // ===================== Empréstimos =====================
    f = fopen("backup_emprestimos.txt","r");
    if(f){
        while(fgets(linha,sizeof(linha),f)){
            linha[strcspn(linha,"\n")] = 0;

            Emprestimo e;
            int campos = sscanf(linha,"%d;%d;%d;%10s;%10s;%9s",
                                &e.codigoEmprestimo, &e.matriculaUsuario, &e.codigoLivro,
                                e.dataEmprestimo, e.dataDevolucao, e.status);
            if(campos == 6)
                emprestimos[totalEmprestimos++] = e;
            else
                printf("[ERRO] Linha inválida no backup_emprestimos.txt: %s\n", linha);
        }
        fclose(f);
    }
}

// ===================== Funções de Cadastro =====================
void cadastrarLivro() {
    if(totalLivros >= MAX_LIVROS){ printf("Limite de livros atingido.\n"); return; }
    Livro l;

    do {
        printf("Código do livro: "); scanf("%d",&l.codigo); limparBuffer();
        if(codigoLivroJaExiste(l.codigo)) printf("Código já existe! Digite outro.\n");
    } while(codigoLivroJaExiste(l.codigo));

    printf("Título: "); fgets(l.titulo,101,stdin); l.titulo[strcspn(l.titulo,"\n")]=0;
    printf("Autor: "); fgets(l.autor,81,stdin); l.autor[strcspn(l.autor,"\n")]=0;
    printf("Editora: "); fgets(l.editora,61,stdin); l.editora[strcspn(l.editora,"\n")]=0;
    printf("Ano de publicação: "); scanf("%d",&l.ano);
    printf("Número de exemplares: "); scanf("%d",&l.exemplares); limparBuffer();
    l.emprestados = 0;

    livros[totalLivros++] = l;
    printf("Livro cadastrado com sucesso!\n");
    backupArquivos();
}

void cadastrarUsuario() {
    if(totalUsuarios >= MAX_USUARIOS){ printf("Limite de usuários atingido.\n"); return; }
    Usuario u;

    do {
        printf("Matrícula: "); scanf("%d",&u.matricula); limparBuffer();
        if(matriculaJaExiste(u.matricula)) printf("Matrícula já cadastrada! Digite outra.\n");
    } while(matriculaJaExiste(u.matricula));

    printf("Nome: "); fgets(u.nome,101,stdin); u.nome[strcspn(u.nome,"\n")]=0;
    printf("Curso: "); fgets(u.curso,51,stdin); u.curso[strcspn(u.curso,"\n")]=0;
    printf("Telefone: "); fgets(u.telefone,16,stdin); u.telefone[strcspn(u.telefone,"\n")]=0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(u.dataCadastro, sizeof(u.dataCadastro), "%02u/%02u/%04u",
             (unsigned)tm.tm_mday, (unsigned)(tm.tm_mon+1), (unsigned)(tm.tm_year+1900));

    usuarios[totalUsuarios++] = u;
    printf("Usuário cadastrado com sucesso!\n");
    backupArquivos();
}

// ===================== Empréstimos =====================
void realizarEmprestimo() {
    if(totalEmprestimos >= MAX_EMPRESTIMOS){ printf("Limite de empréstimos atingido.\n"); return; }
    int codLivro, matUsuario;
    printf("Código do livro: "); scanf("%d",&codLivro);
    printf("Matrícula do usuário: "); scanf("%d",&matUsuario);

    int iLivro=-1, iUsuario=-1;
    for(int i=0;i<totalLivros;i++) if(livros[i].codigo==codLivro){ iLivro=i; break; }
    for(int i=0;i<totalUsuarios;i++) if(usuarios[i].matricula==matUsuario){ iUsuario=i; break; }

    if(iLivro==-1){ printf("Livro não encontrado.\n"); return; }
    if(iUsuario==-1){ printf("Usuário não encontrado.\n"); return; }
    if(livros[iLivro].emprestados >= livros[iLivro].exemplares){ printf("Todos os exemplares emprestados.\n"); return; }

    Emprestimo e;
    e.codigoEmprestimo = totalEmprestimos+1;
    e.codigoLivro = codLivro;
    e.matriculaUsuario = matUsuario;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(e.dataEmprestimo, sizeof(e.dataEmprestimo), "%02u/%02u/%04u",
             (unsigned)tm.tm_mday, (unsigned)(tm.tm_mon+1), (unsigned)(tm.tm_year+1900));

    tm.tm_mday += 7;
    mktime(&tm);
    snprintf(e.dataDevolucao, sizeof(e.dataDevolucao), "%02u/%02u/%04u",
             (unsigned)tm.tm_mday, (unsigned)(tm.tm_mon+1), (unsigned)(tm.tm_year+1900));

    strcpy(e.status,"ativo");

    emprestimos[totalEmprestimos++] = e;
    livros[iLivro].emprestados++;
    printf("Empréstimo realizado! Exemplares restantes: %d\n", livros[iLivro].exemplares - livros[iLivro].emprestados);
    backupArquivos();
}

void realizarDevolucao() {
    int codEmp;
    printf("Código do empréstimo: "); scanf("%d",&codEmp);
    for(int i=0;i<totalEmprestimos;i++){
        if(emprestimos[i].codigoEmprestimo==codEmp && strcmp(emprestimos[i].status,"ativo")==0){
            strcpy(emprestimos[i].status,"devolvido");
            for(int j=0;j<totalLivros;j++)
                if(livros[j].codigo==emprestimos[i].codigoLivro){ livros[j].emprestados--; break; }
            printf("Devolução realizada com sucesso!\n");
            backupArquivos();
            return;
        }
    }
    printf("Empréstimo não encontrado ou já devolvido.\n");
}

// ===================== Pesquisas =====================
void pesquisarLivro() {
    int opc; printf("Pesquisar por: 1-Código 2-Título 3-Autor: "); scanf("%d",&opc); limparBuffer();
    int encontrado=0;
    if(opc==1){
        int cod; printf("Código: "); scanf("%d",&cod);
        for(int i=0;i<totalLivros;i++){
            if(livros[i].codigo==cod){
                printf("Título: %s | Autor: %s | Disponível: %d/%d\n",
                       livros[i].titulo, livros[i].autor, livros[i].exemplares - livros[i].emprestados, livros[i].exemplares);
                encontrado=1; break;
            }
        }
    } else if(opc==2){
        char t[101]; printf("Título: "); fgets(t,101,stdin); t[strcspn(t,"\n")]=0;
        for(int i=0;i<totalLivros;i++)
            if(strstr(livros[i].titulo,t)!=NULL){
                printf("Código: %d | Autor: %s | Disponível: %d/%d\n",
                       livros[i].codigo, livros[i].autor, livros[i].exemplares - livros[i].emprestados, livros[i].exemplares);
                encontrado=1;
            }
    } else if(opc==3){
        char a[81]; printf("Autor: "); fgets(a,81,stdin); a[strcspn(a,"\n")]=0;
        for(int i=0;i<totalLivros;i++)
            if(strstr(livros[i].autor,a)!=NULL){
                printf("Código: %d | Título: %s | Disponível: %d/%d\n",
                       livros[i].codigo, livros[i].titulo, livros[i].exemplares - livros[i].emprestados, livros[i].exemplares);
                encontrado=1;
            }
    }
    if(!encontrado) printf("Livro não encontrado.\n");
}

void pesquisarUsuario(){
    int opc;
    printf("Pesquisar por:\n1 - Nome\n2 - Matrícula\nEscolha: ");
    scanf("%d", &opc);
    limparBuffer();

    int encontrou = 0;

    if(opc == 1){
        char nome[101];
        printf("Nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0'; // remove \n

        for(int i = 0; i < totalUsuarios; i++){
            if(strcasecmp(usuarios[i].nome, nome) == 0){ // uso de strcasecmp para comparação sem case sensitive
                printf("Aluno: %s | Matrícula: %d | Curso: %s | Telefone: %s | Cadastro: %s\n",
                       usuarios[i].nome, usuarios[i].matricula, usuarios[i].curso,
                       usuarios[i].telefone, usuarios[i].dataCadastro);
                encontrou = 1;
            }
        }
    } else if(opc == 2){
        int mat;
        printf("Matrícula: ");
        scanf("%d", &mat);

        for(int i = 0; i < totalUsuarios; i++){
            if(usuarios[i].matricula == mat){
                printf("Aluno: %s | Matrícula: %d | Curso: %s | Telefone: %s | Cadastro: %s\n",
                       usuarios[i].nome, usuarios[i].matricula, usuarios[i].curso,
                       usuarios[i].telefone, usuarios[i].dataCadastro);
                encontrou = 1;
                break;
            }
        }
    } else {
        printf("Opção inválida!\n");
        return;
    }

    if(!encontrou){
        printf("Usuário não encontrado.\n");
    }
}

// ===================== Listagens =====================
void listarEmprestimosAtivos() {
    int encontrou=0;
    printf("\n===== Empréstimos Ativos =====\n");
    for(int i=0;i<totalEmprestimos;i++){
        if(strcmp(emprestimos[i].status,"ativo")==0){
            int iLivro=-1,iUsuario=-1;
            for(int j=0;j<totalLivros;j++) if(livros[j].codigo==emprestimos[i].codigoLivro){ iLivro=j; break; }
            for(int j=0;j<totalUsuarios;j++) if(usuarios[j].matricula==emprestimos[i].matriculaUsuario){ iUsuario=j; break; }
            if(iLivro!=-1 && iUsuario!=-1){
                printf("Empréstimo: %d | Livro: %s | Usuário: %s | Devolução: %s\n",
                       emprestimos[i].codigoEmprestimo, livros[iLivro].titulo, usuarios[iUsuario].nome,
                       emprestimos[i].dataDevolucao);
                encontrou=1;
            }
        }
    }
    if(!encontrou) printf("Nenhum empréstimo ativo.\n");
}

void listarLivrosDisponiveis() {
    int encontrou = 0;
    printf("\n===== Livros Disponíveis =====\n");
    printf("ID | Título | Disponíveis\n");
    for(int i = 0; i < totalLivros; i++){
        int disponiveis = livros[i].exemplares - livros[i].emprestados;
        if(disponiveis > 0){
            printf("%d | %s | %d\n", livros[i].codigo, livros[i].titulo, disponiveis);
            encontrou = 1;
        }
    }
    if(!encontrou) printf("Nenhum livro disponível no momento.\n");
}

void listarUsuarios() {
    int encontrou=0;
    printf("\n===== Usuários Cadastrados =====\n");
    printf("Matrícula | Nome\n");
    for(int i=0;i<totalUsuarios;i++){
        printf("%d | %s | %s\n", usuarios[i].matricula, usuarios[i].nome, usuarios[i].telefone);
        encontrou=1;
    }
    if(!encontrou) printf("Nenhum usuário cadastrado.\n");
}

// ===================== Verificar pendencias ===============

void gerarRelatorioPendencias() {
    FILE *f = fopen("pendencias.txt", "w");
    if (!f) {
        printf("[ERRO] Não foi possível criar o arquivo de pendências.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);
    char hoje[11];
    snprintf(hoje, sizeof(hoje), "%02d/%02d/%04d", dataAtual->tm_mday, dataAtual->tm_mon + 1, dataAtual->tm_year + 1900);

    int encontrou = 0;

    for (int i = 0; i < totalEmprestimos; i++) {
        if (strcmp(emprestimos[i].status, "ativo") == 0 && strcmp(emprestimos[i].dataDevolucao, hoje) < 0) {
            Usuario *u = NULL;
            Livro *l = NULL;

            for (int j = 0; j < totalUsuarios; j++)
                if (usuarios[j].matricula == emprestimos[i].matriculaUsuario) {
                    u = &usuarios[j];
                    break;
                }

            for (int j = 0; j < totalLivros; j++)
                if (livros[j].codigo == emprestimos[i].codigoLivro) {
                    l = &livros[j];
                    break;
                }

            if (u && l) {
                fprintf(f, "Usuário: %s | Matrícula: %d | Telefone: %s\n", u->nome, u->matricula, u->telefone);
                fprintf(f, "Livro: %s | Código: %d | Data de devolução: %s\n\n", l->titulo, l->codigo, emprestimos[i].dataDevolucao);
                printf("============================\nUsuário: %s | Matrícula: %d | Telefone: %s\n", u->nome, u->matricula, u->telefone);
                printf("Livro: %s | Código: %d | Data de devolução: %s\n\n", l->titulo, l->codigo, emprestimos[i].dataDevolucao);
                encontrou = 1;
            }
        }
    }

    fclose(f);

    if (!encontrou) {
        printf("[INFO] Nenhuma pendência encontrada.\n");
        remove("pendencias.txt"); // remove o arquivo se estiver vazio
    } else {
        printf("[INFO] Pendências atualizadas no arquivo 'pendencias.txt'.\n");
    }
}


// ===================== Menu =====================
void menu() {
    printf("\n===== Biblioteca =====\n");
    printf("1. Cadastrar Livro\n2. Cadastrar Usuário\n3. Realizar Empréstimo\n4. Realizar Devolucão\n");
    printf("5. Pesquisar Livro\n6. Pesquisar Usuário\n7. Listar Empréstimos Ativos\n");
    printf("8. Listar Livros Disponíveis\n9. Listar Usuários\n10. Verificar pendencias\n0. Sair\n");
    printf("Escolha uma opção: ");
}

// ===================== Main =====================
int main() {
    carregarBackup(); // Carrega dados existentes

    int opc;
    do{
        menu(); scanf("%d",&opc); limparBuffer();
        switch(opc){
            case 1: cadastrarLivro(); break;
            case 2: cadastrarUsuario(); break;
            case 3: realizarEmprestimo(); break;
            case 4: realizarDevolucao(); break;
            case 5: pesquisarLivro(); break;
            case 6: pesquisarUsuario(); break;
            case 7: listarEmprestimosAtivos(); break;
            case 8: listarLivrosDisponiveis(); break;
            case 9: listarUsuarios(); break;
            case 10: gerarRelatorioPendencias(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    }while(opc != 0);

    return 0;
}