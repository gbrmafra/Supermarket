#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cctype> 
using namespace std;

// Estruturas da Lista Duplamente Encadeada - LDE
struct No {
    string nome;
    double preco;
    No *eloA = nullptr;
    No *eloP = nullptr;
};

struct LDE {
    No *comeco = nullptr;
    No *fim = nullptr;
};

// Funções para manipulação da LDE
bool inserirFinalLde(LDE &lista, string nome, double preco) {
    No *novo = new No;
    if (novo == nullptr) return false;
    novo->nome = nome;
    novo->preco = preco;

    if (lista.comeco == nullptr) {
        lista.comeco = novo;
        lista.fim = novo;
    } else {
        lista.fim->eloP = novo;
        novo->eloA = lista.fim;
        lista.fim = novo;
    }
    return true;
}

bool retirarLde(LDE &lista, string nome) {
    if (lista.comeco == nullptr) return false;

    No *aux = lista.comeco;
    while (aux != nullptr && nome != aux->nome) {
        aux = aux->eloP;
    }
    if (aux == nullptr) return false;

    if (aux == lista.comeco && aux == lista.fim) { // Caso 1
        lista.comeco = nullptr;
        lista.fim = nullptr;
        delete aux;
        return true;
    }
    if (aux == lista.comeco) { // Caso 2
        lista.comeco = aux->eloP;
        lista.comeco->eloA = nullptr;
        delete aux;
        return true;
    }
    No *ant = aux->eloA;
    if (aux == lista.fim) { // Caso 3
        ant->eloP = nullptr;
        lista.fim = ant;
        delete aux;
        return true;
    }
    // Caso 4
    No *prox = aux->eloP;
    ant->eloP = prox;
    prox->eloA = ant;
    delete aux;
    return true;
}

void mostrarC(LDE lista) {
    No *aux = lista.comeco;
    double total = 0.0;

    cout << "Lista de Itens no Caixa:" << endl;
    while (aux != nullptr) {
        cout << "Nome: " << aux->nome << ", Preço: " << fixed << setprecision(2) << aux->preco << endl;
        total += aux->preco;
        aux = aux->eloP;
    }
    cout << "Valor Total: " << fixed << setprecision(2) << total << endl;
}

void liberarLde(LDE &lista) {
    No *aux = lista.comeco;
    No *prox;

    while (aux != nullptr) {
        prox = aux->eloP;
        delete aux;
        aux = prox;
    }
    lista.comeco = nullptr;
    lista.fim = nullptr;
}

// checkout
void checkout(LDE &lista) {
    cout << "Finalizando compra..." << endl;
    
    // mostrar cesta
    mostrarC(lista);

    // calcular valor da comprra
    double total = 0.0;
    No *aux = lista.comeco;
    while (aux != nullptr) {
        total += aux->preco;
        aux = aux->eloP;
    }
    cout << "Valor Total: " << fixed << setprecision(2) << total << endl;

    // Limpar cesta
    liberarLde(lista);

    cout << "Compra finalizada com sucesso!" << endl;
}

//caixa
void adicionarItem(LDE &lista, string nome, const vector<No> &lista_produtos) {
    // transformar em minusculo
    transform(nome.begin(), nome.end(), nome.begin(), ::tolower);

    No *produto_encontrado = nullptr;
    // procurar produto
    for (const No &produto : lista_produtos) {
        string nome_produto = produto.nome;
        // transformar em minusculo
        transform(nome_produto.begin(), nome_produto.end(), nome_produto.begin(), ::tolower);
        if (nome_produto == nome) {
            produto_encontrado = new No; // alocação dinâmica para evitar perda de referência
            produto_encontrado->nome = produto.nome;
            produto_encontrado->preco = produto.preco;
            break;
        }
    }

    if (produto_encontrado != nullptr) {
        if (inserirFinalLde(lista, produto_encontrado->nome, produto_encontrado->preco)) {
            cout << "Item adicionado: " << produto_encontrado->nome << " - Preço: " << fixed << setprecision(2) << produto_encontrado->preco << endl;
        } else {
            cout << "Erro ao adicionar item!" << endl;
        }
        delete produto_encontrado; // liberar memoria
    } else {
        cout << "Produto não encontrado!" << endl;
    }
}

void retirarItem(LDE &lista, string nome) {
    // transformar em minusculo
    transform(nome.begin(), nome.end(), nome.begin(), ::tolower);

    // ver se o item ta no caixa
    bool encontrado = false;
    No *aux = lista.comeco;
    while (aux != nullptr) {
        string nome_produto = aux->nome;
        // transformar em minusculo
        transform(nome_produto.begin(), nome_produto.end(), nome_produto.begin(), ::tolower);
        if (nome_produto == nome) {
            encontrado = true;
            break;
        }
        aux = aux->eloP;
    }

    if (encontrado) {
        if (aux == lista.comeco && aux == lista.fim) { // Caso 1: Único nó na lista
            lista.comeco = nullptr;
            lista.fim = nullptr;
            delete aux;
        } else if (aux == lista.comeco) { // Caso 2: Nó a ser removido é o primeiro da lista
            lista.comeco = aux->eloP;
            lista.comeco->eloA = nullptr;
            delete aux;
        } else if (aux == lista.fim) { // Caso 3: Nó a ser removido é o último da lista
            lista.fim = aux->eloA;
            lista.fim->eloP = nullptr;
            delete aux;
        } else { // Caso 4: Nó a ser removido está no meio da lista
            aux->eloA->eloP = aux->eloP;
            aux->eloP->eloA = aux->eloA;
            delete aux;
        }
        cout << "Item retirado: " << nome << endl;
    } else {
        cout << "Item não encontrado no caixa!" << endl;
    }
}

// func pra ver a lista de produtos
vector<No> lerListaProdutos(const string &nome_arquivo) {
    vector<No> lista_produtos;
    ifstream arquivo(nome_arquivo);

    if (arquivo.is_open()) {
        cout << "Arquivo aberto com sucesso: " << nome_arquivo << endl;
        string linha;
        while (getline(arquivo, linha)) {
            stringstream ss(linha);
            string nome_produto, preco_produto_str;
            double preco_produto;
            if (getline(ss, nome_produto, ';') && getline(ss, preco_produto_str)) {
                preco_produto = stod(preco_produto_str);
                lista_produtos.push_back({nome_produto, preco_produto});
            }
        }
        arquivo.close();
    } else {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
    }

    return lista_produtos;
}

int main() {
    LDE caixa;
    string nome_arquivo = "produtos.txt";
    vector<No> lista_produtos = lerListaProdutos(nome_arquivo);

    if (lista_produtos.empty()) {
        cout << "A lista de produtos está vazia!" << endl;
        return 1;
    }

    cout << "Lista de Produtos:" << endl;
    for (const No &produto : lista_produtos) {
        cout << "Nome: " << produto.nome << ", Preço: " << fixed << setprecision(2) << produto.preco << endl;
    }

    char opcao;
    string item_nome;

    do {
        cout << "Escolha uma opção: (A) Adicionar item, (R) Retirar item, (C) Checkout, (S) Sair: ";
        cin >> opcao;

        switch (opcao) {
            case 'A':
            case 'a':
                cout << "Digite o nome do item a adicionar: ";
                cin >> item_nome;
                adicionarItem(caixa, item_nome, lista_produtos);
                if (item_nome != "Sair") {
                    cout << endl;
                    mostrarC(caixa);
                    cout << endl;
                }
                break;
            case 'R':
            case 'r':
                cout << "Digite o nome do item a retirar: ";
                cin >> item_nome;
                retirarItem(caixa, item_nome);
                if (item_nome != "Sair") {
                    cout << endl;
                    mostrarC(caixa);
                    cout << endl;
                }
                break;
            case 'C':
            case 'c':
                cout << "Finalizando compra..." << endl;
                checkout(caixa);
                cout << endl;
                break;
            case 'S':
            case 's':
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opção inválida!" << endl;
        }
    } while (opcao != 'S' && opcao != 's');

    return 0;
}
