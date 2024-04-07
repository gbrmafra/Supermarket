#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>

using namespace std;

struct NoCarrinho
{
    string nome;
    double valor;
    int quantidade;
    NoCarrinho *elo = nullptr;
};

struct Carrinho
{
    NoCarrinho *start = nullptr;
    NoCarrinho *end = nullptr;
};

struct NoBigData
{
    string data, pagamento;
    double valor;
    NoBigData *elo = nullptr;
};

struct BigData
{
    NoBigData *start = nullptr;
    NoBigData *end = nullptr;
};

void adicionarAoCarrinho(Carrinho &carrinho, string nome, double valor, int quantidade)
{
    NoCarrinho *newNo = new NoCarrinho;
    newNo->nome = nome;
    newNo->valor = valor;
    newNo->quantidade = quantidade;
    newNo->elo = nullptr;

    if (carrinho.start == nullptr)
    {
        carrinho.start = newNo;
        carrinho.end = newNo;
    }
    else
    {
        carrinho.end->elo = newNo;
        carrinho.end = newNo;
    }
}

void exibirCarrinho(Carrinho carrinho)
{
    NoCarrinho *temp = carrinho.start;

    // Exibindo cabeçalho da tabela
    cout << setw(30) << left << "Nome:";
    cout << setw(10) << left << "Preco:";
    cout << setw(30) << left << "Quantidade:" << endl;

    // Exibindo elementos do carrinho
    while (temp != nullptr)
    {
        cout << setw(30) << left << temp->nome;
        cout << setw(10) << left << temp->valor;
        cout << setw(30) << left << temp->quantidade << endl;
        temp = temp->elo;
    }
    cout << endl;
}

double somaCarrinho(Carrinho carrinho)
{
    NoCarrinho *temp = carrinho.start;
    double soma = 0;

    while (temp != nullptr)
    {
        soma += temp->quantidade * temp->valor;
        temp = temp->elo;
    }
    cout << "________________________________________________________" << endl;
    cout << setw(30) << left << "Total: ";
    cout << setw(40) << left << soma << endl;
    return soma;
}

NoCarrinho *buscarNoCarrinho(Carrinho &carrinho, string nome)
{
    NoCarrinho *temp = carrinho.start;
    while (temp != nullptr)
    {
        if (temp->nome == nome)
        {
            return temp;
        }
        temp = temp->elo;
    }
    return nullptr;
}

void removerDoCarrinho(Carrinho &carrinho, string nome)
{
    NoCarrinho *anterior = nullptr;
    NoCarrinho *atual = carrinho.start;

    // Procurar o produto no carrinho
    while (atual != nullptr && atual->nome != nome)
    {
        anterior = atual;
        atual = atual->elo;
    }

    // Se o produto foi encontrado
    if (atual != nullptr)
    {
        // Se o produto a ser removido é o primeiro do carrinho
        if (anterior == nullptr)
        {
            carrinho.start = atual->elo;
        }
        else
        {
            anterior->elo = atual->elo;
        }

        // Se o produto a ser removido é o último do carrinho
        if (atual == carrinho.end)
        {
            carrinho.end = anterior;
        }

        // Deletar o nó correspondente ao produto
        delete atual;
        cout << "Produto '" << nome << "' removido do carrinho com sucesso!" << endl;
    }
    else
    {
        cout << "Produto '" << nome << "' nao encontrado no carrinho." << endl;
    }
}
void salvarCompras(Carrinho &carrinho)
{
    NoCarrinho *aux = carrinho.start;
    ofstream fileStream("Carrinho.txt");
    while (aux != nullptr)
    {
        // Salvar o nome, o valor e a quantidade do produto, separados por ponto e vírgula
        fileStream << aux->nome << ";" << aux->valor << ";" << aux->quantidade << endl;
        aux = aux->elo;
    }
    fileStream.close();
    cout << "Dados gravados com sucesso!" << endl;
}

void carregarCarrinho(Carrinho &carrinho)
{
    ifstream fileReader("Carrinho.txt");
    if (!fileReader)
    {
        cout << "Erro ao abrir o arquivo para leitura." << endl;
        return;
    }

    string linha;
    while (getline(fileReader, linha))
    {
        // Encontrar a posição dos ponto e vírgulas para separar o nome, o valor e a quantidade
        size_t pos1 = linha.find(";");
        size_t pos2 = linha.find(";", pos1 + 1);
        if (pos1 != string::npos && pos2 != string::npos)
        {
            // Extrair o nome, o valor e a quantidade do produto da linha
            string nome = linha.substr(0, pos1);
            double valor = stod(linha.substr(pos1 + 1, pos2 - pos1 - 1));
            int quantidade = stoi(linha.substr(pos2 + 1));

            // Adicionar o produto ao carrinho
            adicionarAoCarrinho(carrinho, nome, valor, quantidade);
        }
    }

    fileReader.close();
    cout << "Dados lidos com sucesso!" << endl;
}

void salvarBigData(Carrinho &carrinho, BigData &dados, string pagamento, double custoTotal, string horaCompra)
{
    NoBigData *novoDado = new NoBigData;
    novoDado->data = horaCompra;
    novoDado->pagamento = pagamento;
    novoDado->valor = custoTotal;
    novoDado->elo = nullptr;

    if (dados.start == nullptr)
    {
        dados.start = novoDado;
        dados.end = novoDado;
    }
    else
    {
        dados.end->elo = novoDado;
        dados.end = novoDado;
    }

    // Salvar os dados da compra no arquivo "dados.txt"
    ofstream dadosStream("dados.txt", ofstream::app);
    if (dadosStream.is_open())
    {
        dadosStream << horaCompra << ";" << pagamento << ";" << custoTotal << endl;
        dadosStream.close();
    }
    else
    {
        cout << "Erro ao abrir o arquivo 'dados.txt'." << endl;
    }

    // Limpar o arquivo do carrinho
    ofstream carrinhoStream("Carrinho.txt", ofstream::out | ofstream::trunc);
    carrinhoStream.close();

    // Recarregar o carrinho após limpeza
    carregarCarrinho(carrinho);

    cout << "Compra salva e carrinho limpo com sucesso!" << endl;
}

string getHoraAtual()
{
    auto agora = chrono::system_clock::now();
    time_t tempo = chrono::system_clock::to_time_t(agora);
    stringstream ss;
    ss << put_time(localtime(&tempo), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool calcularTroco(double soma)
{
    double dinheiroUsuario = 0;
    cout << "Digite seu dinheiro: ";
    cin >> dinheiroUsuario;
    if (dinheiroUsuario > soma)
    {
        cout << "seu troco e de : " << dinheiroUsuario - soma << endl;
        return true;
    }
    else if (dinheiroUsuario == soma)
    {
        cout << "obrigado por comprar aqui!! " << endl;
        return true;
    }
    else
    {
        cout << "Valor insufuciente" << endl;
        return false;
    }
}
void limparCarrinho(Carrinho &carrinho)
{
    NoCarrinho *atual = carrinho.start;
    while (atual != nullptr)
    {
        NoCarrinho *prox = atual->elo;
        delete atual;
        atual = prox;
    }
    carrinho.start = nullptr;
    carrinho.end = nullptr;
}

void formaPagamento(Carrinho &carrinho, BigData dados)
{
    void salvarBigData(Carrinho & carrinho, BigData & dados, string pagamento, double custoTotal, string horaCompra);

    int selecao;
    bool escolha = false;
    cout << "qual a forma de pagamento?" << endl;
    cout << "0. cancelar" << endl;
    cout << "1. pix" << endl;
    cout << "2. credito" << endl;
    cout << "3. debito" << endl;
    cout << "4. dinheiro" << endl;
    cout << endl;

    cin >> selecao;
    double soma = somaCarrinho(carrinho);

    while (!escolha)
    {
        string hora = getHoraAtual();
        string pagamento;
        switch (selecao)
        {

        case 0:
            system("cls");

            cout << "voltando..." << endl;
            escolha = true;
            break;
        case 1:
            system("cls");

            cout << "Pix confirmado!!" << endl;
            pagamento = "pix";
            salvarBigData(carrinho, dados, pagamento, soma, hora);
            escolha = true;
            break;
        case 2:
            system("cls");

            cout << "credito confirmado!!" << endl;
            pagamento = "credito";
            salvarBigData(carrinho, dados, pagamento, soma, hora);
            escolha = true;
            break;
        case 3:
            system("cls");

            cout << "debito confirmado!!" << endl;
            pagamento = "debito";
            salvarBigData(carrinho, dados, pagamento, soma, hora);
            escolha = true;
            break;
        case 4:
            system("cls");
            cout << "Pagamento com dinheiro!!" << endl;
            if (calcularTroco(soma))
            {
                pagamento = "dinheiro";
                salvarBigData(carrinho, dados, pagamento, soma, hora);
                escolha = true;
            }
            else
            {
                cout << "Valor invalido!" << endl;
            }
            break;

        default:
            system("cls");

            cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
            break;
        }
    }
}