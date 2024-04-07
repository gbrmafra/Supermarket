// gabriel uesler de brito && gabriel anacrecio mafra.
#include <iostream>
#include <string>

#include "controleLista.h"
#include "carrinho.h"
using namespace std;

// Função para mostrar o menu principal
int mostrarMenu()
{
    cout << R"(

            _       _             _       _       
           (_)     | |           | |     (_)      
  _ __ ___  _ _ __ | |__   __ _  | | ___  _  __ _ 
 | '_ ` _ \| | '_ \| '_ \ / _` | | |/ _ \| |/ _` |
 | | | | | | | | | | | | | (_| | | | (_) | | (_| |
 |_| |_| |_|_|_| |_|_| |_|\__,_| |_|\___/| |\__,_|
                                        _/ |      
                                       |__/       
______________________________________________________________________
)" << endl;

    cout << "Menu:" << endl;
    cout << "0. Sair" << endl;
    cout << "1. Adicionar produto" << endl;
    cout << "2. Exibir estoque" << endl;
    cout << "3. Pesquisar produto" << endl;
    cout << "4. Configuracoes do produto" << endl;
    cout << "5. Comprar" << endl;
    cout << "Escolha uma opcao: ";

    int escolha;
    cin >> escolha;

    return escolha;
}

// Função para mostrar o menu de compras
void mostrarMenuCompras()
{
    cout << "Menu de Compras:" << endl;
    cout << "0. Voltar" << endl;
    cout << "1. Adicionar produto ao carrinho" << endl;
    cout << "2. Visualizar carrinho" << endl;
    cout << "3. Remover produto do carrinho" << endl;
    cout << "4. Finalizar compra" << endl;
    cout << "Escolha uma opcao: ";
}

void adicionarAoCarrinhoSeExistir(Estoque &estoque, Carrinho &carrinho, string nome, int quantidade)
{
    No *produto = jaExiste(estoque, nome);
    if (produto != nullptr)
    {
        // Verifica se o produto já está no carrinho
        NoCarrinho *produtoNoCarrinho = buscarNoCarrinho(carrinho, nome);
        if (produtoNoCarrinho != nullptr)
        {
            // Se o produto já estiver no carrinho, atualiza a quantidade
            produtoNoCarrinho->quantidade += quantidade;
            cout << "Produto '" << nome << "' atualizado no carrinho com sucesso!" << endl;
        }
        else
        {
            // Caso contrário, adiciona o produto ao carrinho
            adicionarAoCarrinho(carrinho, nome, produto->valor, quantidade);
            cout << "Produto '" << nome << "' adicionado ao carrinho com sucesso!" << endl;
        }
    }
    else
    {
        cout << "Produto '" << nome << "' nao encontrado no estoque." << endl;
    }
}

void coletarProduto(Estoque &estoque, Carrinho &carrinho)
{
    string nome;
    int quantidade;
    cout << "digite o nome do pruduto: ";
    cin.ignore();
    getline(cin, nome);
    cout << "\n digite a quantidade: ";
    cin >> quantidade;
    adicionarAoCarrinhoSeExistir(estoque, carrinho, nome, quantidade);
}

int main()
{

    Estoque estoque;
    Carrinho carrinho;
    BigData dados;
    bool sair = false;

    carregarStoque(estoque);
    carregarCarrinho(carrinho);
    while (!sair)
    {
        int escolha = mostrarMenu();
        string nomePesquisa;
        bool sairCarrinho = false;

        switch (escolha)
        {
        case 0:
            system("cls");
            cout << "Saindo do programa." << endl;
            sair = true;
            break;
        case 1:
            system("cls");
            cout << "Voce escolheu a opcao 1: Adicionar produto." << endl;
            novoProduto(estoque);
            salvarStoque(estoque);

            break;
        case 2:
            system("cls");
            cout << "Voce escolheu a opcao 2: Exibir estoque." << endl;

            exibir(estoque);
            break;
        case 3:
            system("cls");
            cout << "Voce escolheu a opcao 3: Pesquisar produto." << endl;
            cout << "Digite o nome do produto: ";
            cin.ignore();
            getline(cin, nomePesquisa);

            if (jaExiste(estoque, nomePesquisa) != nullptr)
            {
                cout << "Produto " << nomePesquisa << " encontrado" << endl;
            }
            else
            {
                cout << "Produto nao encontrado" << endl;
            }
            break;
        case 4:
            system("cls");
            exibir(estoque);
            cout << endl;
            cout << "Voce escolheu a opcao 4: Configurações do produto." << endl;
            cout << "Digite o nome do produto: ";
            cin.ignore();
            getline(cin, nomePesquisa);
            opcoesProduto(estoque, nomePesquisa);
            salvarStoque(estoque);

            break;
        case 5:
            system("cls");
            cout << "Voce escolheu a opcao 5: Comprar." << endl;

            while (!sairCarrinho)
            {

                mostrarMenuCompras();
                int escolhaCompra;
                cin >> escolhaCompra;
                switch (escolhaCompra)
                {
                case 0:
                    system("cls");

                    cout << "Voce escolheu a opcao 0: voltar." << endl;
                    sairCarrinho = true;
                    break;
                case 1:
                    system("cls");

                    cout << "Voce escolheu a opcao 1: adicionar produto ao carrinho." << endl;
                    exibir(estoque);
                    coletarProduto(estoque, carrinho);
                    salvarCompras(carrinho);

                    break;
                case 2:
                    system("cls");

                    cout << "Voce escolheu a opcao 2: visualizar carrinho." << endl;
                    exibirCarrinho(carrinho);
                    somaCarrinho(carrinho);
                    break;
                case 3:
                    system("cls");

                    cout << "Voce escolheu a opcao 3: remover produto do carrinho." << endl;
                    // Implemente a lógica para remover produto do carrinho aqui
                    cout << "Digite o nome do produto: ";
                    cin.ignore();
                    getline(cin, nomePesquisa);
                    removerDoCarrinho(carrinho, nomePesquisa);
                    salvarCompras(carrinho);

                    break;
                case 4:
                    system("cls");

                    cout << "Voce escolheu a opcao 4: finalizar compra." << endl;
                    formaPagamento(carrinho, dados);
                    limparCarrinho(carrinho); // Limpar o carrinho após o pagamento

                    break;
                default:
                    system("cls");

                    cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
                    break;
                }
            }
            break;
        default:
            cout << "Opcao invalida. Por favor, escolha uma opcao valida." << endl;
            break;
        }
    }

    salvarStoque(estoque);
    salvarCompras(carrinho);

    return 0;
}
