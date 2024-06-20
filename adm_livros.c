#include <stdio.h>
#include <string.h>
#include <ctype.h> // biblioteca para utilizar o tolower
#include <unistd.h> // biblioteca para utilizar o sleep
#include <stdlib.h> // biblioteca para utilizar a função system
#include <locale.h>

#define MAX_LIVROS 100
#define MAX_STRING_LENGTH 100
#define MAX_URL_LENGTH 200

typedef enum Genero {
  FICCAO,
  ROMANCE,
  CIENCIA,
  HISTORIA,
  FILOSOFIA,
  BIOGRAFIA,
  NUM_GENEROS
} Genero;

typedef struct Livro {
  char titulo[MAX_STRING_LENGTH];
  char autor[MAX_STRING_LENGTH];
  int ano;
  Genero genero;
  char url[MAX_URL_LENGTH];
} Livro;

const char* get_gender(Genero genero) {
  switch (genero) {
    case FICCAO: return "Ficção";
    case ROMANCE: return "Romance";
    case CIENCIA: return "Ciência";
    case HISTORIA: return "História";
    case FILOSOFIA: return "Filosofia";
    case BIOGRAFIA: return "Biografia";
    default: return "Desconhecido";
  }
}

// Função para tornar letras maiúsculas em minúsculas
void to_lowercase(char *str) {
  for (int i = 0; str[i]; i++) {
    str[i] = tolower((unsigned char)str[i]);
  }
}

// Função para carregar a biblioteca do arquivo txt para o array
void carregar_biblioteca(Livro livros[], int* contador) {
  FILE *arquivo = fopen("biblioteca.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir a biblioteca.\n");
    return;
  }

  *contador = 0; // Inicializa o contador de livros

  // Lê cada linha do arquivo e carrega os dados dos livros
  while (fscanf(arquivo, "%99[^\n]\n%99[^\n]\n%d\n%d\n%199[^\n]\n",
                livros[*contador].titulo,
                livros[*contador].autor,
                &livros[*contador].ano,
                (int *)&livros[*contador].genero,
                livros[*contador].url) != EOF) {
      (*contador)++;
  }

  fclose(arquivo);
}

// função para salvar a biblioteca atualizada no txt
void salvar_biblioteca(Livro livros[], int contador) {
  FILE *arquivo = fopen("biblioteca.txt", "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  // Escreve cada livro no arquivo
  for (int i = 0; i < contador; i++) {
    fprintf(arquivo, "%s\n%s\n%d\n%d\n%s\n",
            livros[i].titulo,
            livros[i].autor,
            livros[i].ano,
            livros[i].genero,
            livros[i].url);
  }

  fclose(arquivo);
}

// função para abrir a URL dos livros
void abrir_url(const char* url) {
  char command[MAX_URL_LENGTH + 10];
    snprintf(command, sizeof(command), "start %s", url); // Para sistemas Windows
  system(command);
}

void add_livro(Livro livros[], int* contador) {
  if (*contador >= MAX_LIVROS) {
    printf("Limite de livros atingido. Não é possível adicionar mais.\n");
    return;
  }

  Livro novo_livro;

  printf("\nDigite o título do livro: ");
  fgets(novo_livro.titulo, MAX_STRING_LENGTH, stdin);
  novo_livro.titulo[strcspn(novo_livro.titulo, "\n")] = '\0'; // Remove nova linha

  printf("Digite o autor do livro: ");
  fgets(novo_livro.autor, MAX_STRING_LENGTH, stdin);
  novo_livro.autor[strcspn(novo_livro.autor, "\n")] = '\0'; // Remove nova linha

  printf("Digite o ano de publicação do livro: ");
  scanf("%d", &novo_livro.ano);
  getchar(); // Limpa o buffer de entrada

  printf("0. Ficção\n");
  printf("1. Romance\n");
  printf("2. Ciência\n");
  printf("3. História\n");
  printf("4. Filosofia\n");
  printf("5. Biografia\n");
  printf("Qual o gênero do livro?\n");

  int genero;
  scanf("%d", &genero);
  getchar(); // Limpa o buffer de entrada

  // Verifica se o gênero é válido e adiciona o livro à biblioteca
  if (genero >= 0 && genero < NUM_GENEROS) {
    novo_livro.genero = (Genero)genero;

    printf("Digite a URL do livro na Amazon: ");
    fgets(novo_livro.url, MAX_URL_LENGTH, stdin);
    novo_livro.url[strcspn(novo_livro.url, "\n")] = '\0'; // Remove nova linha

    livros[*contador] = novo_livro;
    (*contador)++;
    salvar_biblioteca(livros, *contador); // Salva o livro no arquivo txt
    printf("Livro adicionado com sucesso!\n");
  } else {
      printf("Gênero inválido. Livro não adicionado.\n");
  }
  sleep(3);
}

void exibir_biblioteca(Livro livros[], int contador) {
  printf("\n\t\t\tLivros na Biblioteca\t\t\t\n");

  for (int i = 0; i < contador; i++) {
    printf("Livro %d:\n", i + 1);
    printf("Título: %s\n", livros[i].titulo);
    printf("Autor: %s\n", livros[i].autor);
    printf("Ano: %d\n", livros[i].ano);
    printf("Gênero: %s\n", get_gender(livros[i].genero));
    printf("URL: %s\n", livros[i].url);
    printf("----------------\n");
  }
  sleep(2);
  printf("Digite o número do livro para abrir o URL ou 0 para voltar ao menu: ");
  int escolha;
  scanf("%d", &escolha);
  getchar(); // Limpa o buffer de entrada

  if (escolha > 0 && 0 <= contador) {
    abrir_url(livros[escolha - 1].url);
  }
}

void buscar_livro(Livro livros[], int contador) {
  char termo_busca[MAX_STRING_LENGTH];

  printf("\nDigite o título ou o nome do autor que procura: ");
  fgets(termo_busca, MAX_STRING_LENGTH, stdin);
  termo_busca[strcspn(termo_busca, "\n")] = '\0'; // Remove a nova linha

  to_lowercase(termo_busca);

  int encontrado = 0;

  // Percorre todos os livros para encontrar correspondências
  for (int i = 0; i < contador; i++) {
    char titulo_lower[MAX_STRING_LENGTH], autor_lower[MAX_STRING_LENGTH];
    strcpy(titulo_lower, livros[i].titulo);
    strcpy(autor_lower, livros[i].autor);
    to_lowercase(titulo_lower);
    to_lowercase(autor_lower);

    if (strstr(titulo_lower, termo_busca) != NULL || strstr(autor_lower, termo_busca) != NULL) {
      if (!encontrado) {
        printf("\nLivro(s) encontrado(s):\n");
        encontrado = 1;
      }
      printf("Livro %d:\n", i + 1);
      printf("Título: %s\n", livros[i].titulo);
      printf("Autor: %s\n", livros[i].autor);
      printf("Ano: %d\n", livros[i].ano);
      printf("Gênero: %s\n", get_gender(livros[i].genero));
      printf("URL: %s\n", livros[i].url);
      printf("----------------\n");
    }
  }
  sleep(2);

  if (!encontrado) {
    printf("Livro não encontrado.\n");
  } else {
    printf("Digite o número do livro para abrir o URL ou 0 para voltar ao menu: ");
    int escolha;
    scanf("%d", &escolha);
    getchar(); // Limpa o buffer de entrada

    if (escolha > 0 && escolha <= contador) {
      abrir_url(livros[escolha - 1].url);
    }
  }
}

void remove_livro(Livro livros[], int* contador) {
  char termo_busca[MAX_STRING_LENGTH];

  printf("\nDigite o título do livro que deseja remover: ");
  getchar(); // Limpa o buffer de entrada
  fgets(termo_busca, MAX_STRING_LENGTH, stdin);
  termo_busca[strcspn(termo_busca, "\n")] = '\0'; // Remove a nova linha

  to_lowercase(termo_busca);

  int encontrado = 0;
  int indices_encontrados[MAX_LIVROS];
  int num_livros_encontrados = 0;

  // Busca todos os livros que correspondam ao termo de busca
  for (int i = 0; i < *contador; i++) {
    char titulo_lower[MAX_STRING_LENGTH];
    strcpy(titulo_lower, livros[i].titulo);
    to_lowercase(titulo_lower);

    if (strstr(titulo_lower, termo_busca) != NULL) {
      indices_encontrados[num_livros_encontrados] = i; // Armazena os índices dos livros encontrados
      printf("\nLivro %d encontrado:\n", num_livros_encontrados + 1);
      printf("Título: %s\n", livros[i].titulo);
      printf("Autor: %s\n", livros[i].autor);
      printf("Ano: %d\n", livros[i].ano);
      printf("Gênero: %s\n", get_gender(livros[i].genero));
      printf("URL: %s\n", livros[i].url);
      printf("----------------\n");
      num_livros_encontrados++;
      encontrado = 1;
    }
  }

  if (!encontrado) {
    printf("Livro não encontrado.\n");
    return;
  }

  if (num_livros_encontrados == 1) {
    char resposta;
    printf("\nDeseja realmente remover este livro? (s/n): ");
    scanf(" %c", &resposta);

    // Remove o livro deslocando os livros seguintes
    if (resposta == 's' || resposta == 'S') {
      int indice = indices_encontrados[0]; // Pega o índice do único livro encontrado

      for (int i = indice; i < *contador - 1; i++) {
          livros[i] = livros[i + 1];
      }
      (*contador)--;
      salvar_biblioteca(livros, *contador); // Salvar biblioteca após remoção
      printf("Livro removido com sucesso!\n");
    } else {
      printf("Operação de remoção cancelada.\n");
    }
  } else if (num_livros_encontrados > 1) {
    printf("Mais de um livro encontrado, selecione o número correspondente ao livro que deseja remover:\n");
    int escolha;
    scanf("%d", &escolha);

    if (escolha > 0 && escolha <= num_livros_encontrados) {
      char resposta;
      printf("\nDeseja realmente remover este livro? (s/n): ");
      scanf(" %c", &resposta);

      if (resposta == 's' || resposta == 'S') {
        int indice = indices_encontrados[escolha - 1]; // Pega o índice selecionado pelo usuário

        for (int i = indice; i < *contador - 1; i++) {
          livros[i] = livros[i + 1];
        }
        (*contador)--;
        salvar_biblioteca(livros, *contador); // Salvar biblioteca após remoção
        printf("Livro removido com sucesso!\n");
      } else {
        printf("Operação de remoção cancelada.\n");
      }
    } else {
      printf("Opção inválida.\n");
    }
  }
  sleep(2);
}


int main() {

  setlocale(LC_ALL, "Portuguese_Brazil");

  Livro livros[MAX_LIVROS];
  int contador = 0; // Contador de livros na biblioteca
  int opcao;

  // Carregar biblioteca ao iniciar o programa
  carregar_biblioteca(livros, &contador);

  do {
    printf(" _______________________________\n");
    printf("|              MENU             |\n");
    printf("|                               |\n");
    printf("| 1. Exibir Biblioteca          |\n");
    printf("| 2. Buscar Livro               |\n");
    printf("| 3. Adicionar livro            |\n");
    printf("| 4. Remover Livro              |\n");
    printf("| 5. Sair                       |\n");
    printf("|_______________________________|\n");
    printf(" Escolha uma opção: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o buffer de entrada

    switch (opcao) {
      case 1:
        exibir_biblioteca(livros, contador);
        break;
      case 2:
        buscar_livro(livros, contador);
        break;
      case 3:
        add_livro(livros, &contador);
        break;
      case 4:
        remove_livro(livros, &contador);
        break;
      case 5:
        printf("\nSaindo...\n");
        break;

      default:
        printf("Opção inválida! Tente novamente.\n");
        break;
    }
  } while (opcao != 5);
  return 0;
}
