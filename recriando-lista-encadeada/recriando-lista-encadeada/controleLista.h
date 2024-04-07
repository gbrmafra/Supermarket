#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

struct No
{
    string nome;
    double valor;
    No *elo = nullptr;
};

struct Estoque
{
    No *start = nullptr;
    No *end = nullptr;
};

void adicionar(Estoque &estoque, string nome, double valor)
{
    No *newNo = new No;
    newNo->nome = nome;
    newNo->valor = valor;
    newNo->elo = nullptr;

    if (estoque.start == nullptr)
    {
        estoque.start = newNo;
        estoque.end = newNo;
    }
    else
    {
        estoque.end->elo = newNo;
        estoque.end = newNo;
    }
}

void exibir(Estoque estoque)
{
    No *temp = estoque.start;

    // Exibindo cabeçalho da tabela
    system("cls");
    cout << setw(30) << left << "Nome:";
    cout << setw(10) << left << "Preco:" << endl;
    cout << "______________________________________" << endl;

    // Exibindo elementos da estoque
    while (temp != nullptr)
    {
        cout << setw(30) << left << temp->nome;
        cout << setw(10) << left << temp->valor << endl;
        temp = temp->elo;
    }
    cout << endl;
}

No *jaExiste(Estoque &estoque, string nome)
{
    No *newEstoque = estoque.start;
    while (newEstoque != nullptr)
    {
        if (nome == newEstoque->nome)
        {
            return newEstoque;
        }
        newEstoque = newEstoque->elo;
    }
    return nullptr;
}

void novoProduto(Estoque &estoque)
{
    string nome;
    double valor;
    cout << "\n digite o nome: " << endl;
    cin.ignore();
    getline(cin, nome);
    cout << "\n digite o valor: " << endl;
    cin >> valor;
    if (!jaExiste(estoque, nome))
    {
        adicionar(estoque, nome, valor);
    }
    else
    {
        cout << "Produto ja existe" << endl;
        novoProduto(estoque);
    }
}

void editarProduto(Estoque &estoque, string nome)
{
    No *produto = jaExiste(estoque, nome);
    if (produto != nullptr)
    {
        string novoNome;
        double novoValor;

        cout << "Digite o novo nome para o produto '" << nome << "': ";
        cin.ignore();
        getline(cin, novoNome);
        cout << "Digite o novo valor para o produto '" << nome << "': ";
        cin >> novoValor;

        produto->nome = novoNome;
        produto->valor = novoValor;
        cout << "Produto " << nome << " editado com sucesso!" << endl;
    }
    else
    {
        cout << "Produto '" << nome << "' nao encontrado." << endl;
    }
}

void deletarProduto(Estoque &estoque, string nome)
{
    No *anterior = nullptr;
    No *atual = estoque.start;

    // Procurar o produto na lista
    while (atual != nullptr && atual->nome != nome)
    {
        anterior = atual;
        atual = atual->elo;
    }

    // Se o produto foi encontrado
    if (atual != nullptr)
    {
        // Se o produto a ser deletado é o primeiro da lista
        if (anterior == nullptr)
        {
            estoque.start = atual->elo;
        }
        else
        {
            anterior->elo = atual->elo;
        }

        // Se o produto a ser deletado é o último da lista
        if (atual == estoque.end)
        {
            estoque.end = anterior;
        }

        // Deletar o nó correspondente ao produto
        delete atual;
        cout << "Produto '" << nome << "' deletado com sucesso!" << endl;
    }
    else
    {
        cout << "Produto '" << nome << "' nao encontrado." << endl;
    }
}

void opcoesProduto(Estoque &estoque, string nome)
{
    if (jaExiste(estoque, nome) != nullptr)
    {
        int seletor;
        cout << "Deseja (1) deletar, ou (2) editar?" << endl;
        cin >> seletor;
        if (seletor == 1)
        {
            deletarProduto(estoque, nome);
        }
        else if (seletor == 2)
        {
            editarProduto(estoque, nome);
        }
    }
    else
    {
        cout << "produto nao " << nome << " encontrado";
    }
}

void salvarStoque(Estoque &estoque)
{
    No *aux = estoque.start;
    ofstream fileStream("Estoque.txt");
    while (aux != nullptr)
    {
        // Salvar o nome e o valor do produto, separados por ponto e vírgula
        fileStream << aux->nome << ";" << aux->valor << endl;
        aux = aux->elo;
    }
    fileStream.close();
    cout << "Dados gravados com sucesso!" << endl;
}

void carregarStoque(Estoque &estoque)
{
    ifstream fileReader("Estoque.txt");
    if (!fileReader)
    {
        cout << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }

    string linha;
    while (getline(fileReader, linha))
    {
        // Encontrar a posição do ponto e vírgula para separar o nome do valor
        size_t pos = linha.find(";");
        if (pos != string::npos)
        {
            // Extrair o nome e o valor do produto da linha
            string nome = linha.substr(0, pos);
            double valor = stod(linha.substr(pos + 1));

            // Adicionar o produto ao estoque
            adicionar(estoque, nome, valor);
        }
    }

    fileReader.close();
    cout << "Dados lidos com sucesso!" << endl;
}
