#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

class Product {

protected:
    string name;
    int stock;

public:
    double price;
    Product() {}
    Product(string name, double price, int stock) : name(name), price(price), stock(stock) {}

    string getName() const { return name; }
    int getStock() const { return stock; }

    // Operator overloading == pentru compararea produselor dupa nume
    bool operator==(const Product& other) const {
        return name == other.name;
    }

    // Operator overloading -= pentru scaderea stocului unui produs
    void operator-=(int quantity) {
        stock = (stock >= quantity) ? stock - quantity : 0;
    }

    virtual void display() const {
        cout << "Produs: " << name << ", Pret: " << fixed << setprecision(2) << price << " lei, Stoc: " << stock << endl;
    }

    virtual void displayWithoutStock() const {
        cout << "Produs: " << name << ", Pret: " << fixed << setprecision(2) << price << " lei" << endl;
    }


};

class TableLamp : virtual public Product{
public:
    TableLamp(string name, double price, int stock) : Product(name, price, stock) {}

    void displayLampaVeioza() const  {
        cout << "Veioza - ";
        Product::display();
    }

    void displayWithoutStockVeioza() const {
        cout << "Veioza - ";
        Product::displayWithoutStock();
    }
};

class CeilingLamp : virtual public Product {
public:
    CeilingLamp(string name, double price, int stock) : Product(name, price, stock) {}

    void displayLampaTavan() const {
        cout << "Lampa de Tavan - ";
        Product::display();
    }

    void displayWithoutStockTavan() const {
        cout << "Lampa de Tavan - ";
        Product::displayWithoutStock();
    }
};

class WallLamp : virtual  public Product {
public:
    WallLamp(string name, double price, int stock) : Product(name, price, stock) {}

    void displayLampaPerete() const {
        cout << "Lampa de Perete - ";
        Product::display();
    }

    void displayWithoutStockPerete() const  {
        cout << "Lampa de Perete - ";
        Product::displayWithoutStock();
    }
};
//
// class parinte {
// public:
//     string name;
//     bool disponibil;
//     double price;
//     virtual void display() const {
//         cout<< "Produs: " << name<<endl;
//     }
//
//     virtual bool esteDisponibil() {
//         return disponibil;
//     }
//
//     virtual void afisarePret() {
//         cout<<"Price: "<<price<<endl;
//     }
//
// };
//
// class copil1 : virtual  public parinte {
// public:
//     void display() const {
//         cout<< "Nume copil1 : " << name<<endl;
//     }
//
//     bool esteDisponibil() {
//         return disponibil;
//     }
//
//
// };
//
// class copil2: virtual public parinte {
// public:
//     void afisarePret() {
//         cout<<"Price copil1: "<<price<<endl;
//     }
// };
//
// class diamant:public copil1,public copil2 {
// public:
//     string hello;
//     void afisare(){
//         cout<<"Hello = "<<hello<<endl;
//     }
// };




class CartItem{
public:
    Product* product;
    int quantity;
    CartItem(Product* product, int quantity) : product(product), quantity(quantity) {}
};

class Cart:public WallLamp, public CeilingLamp, public TableLamp  {
    vector<CartItem> items;
    double total;

public:
    Cart() : total(0), WallLamp("", 0, 0), CeilingLamp("", 0, 0), TableLamp("", 0, 0) {}

    // Operator overloading += pentru adaugarea unui produs in cos
    Cart& operator+=(CartItem item) {
        bool found = false;
        for (auto& i : items) {
            if (*i.product == *item.product) {
                i.quantity += item.quantity;
                found = true;
                break;
            }
        }
        if (!found) {
            items.push_back(item);
        }
        total += item.product->price * item.quantity;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Cart& cart) {
        os << "\nCosul dumneavoastra:\n";
        for (const auto& item : cart.items) {
            os << item.quantity << " x ";
            item.product->displayWithoutStock();
        }
        os << "Total: " << fixed << setprecision(2) << cart.total << " lei\n";
        return os;
    }

    double getTotal() const {
        return total;
    }

    void applyDiscount(double discount) {
        total *= (1 - discount);
    }

    void clearCart() {
        items.clear();
        total = 0;
    }
};

class User {

    string name;
    string idnp;
    int visits;
    double totalSpent;

public:
    User(string name, string idnp) : name(name), idnp(idnp), visits(0), totalSpent(0) {}

    void loadData() {
        ifstream file("users.txt");
        if (file.is_open()) {
            string fileIdnp, fileName;
            int fileVisits;
            double fileTotalSpent;
            while (file >> fileIdnp >> fileName >> fileVisits >> fileTotalSpent) {
                if (fileIdnp == idnp) {
                    visits = fileVisits;
                    totalSpent = fileTotalSpent;
                    break;
                }
            }
            file.close();
        }
    }

    void updateData(double cartTotal) {
        visits++; // Chemarea operatorului supraincarcat ++
        totalSpent += cartTotal;

        vector<string> lines;
        ifstream infile("users.txt");
        string line;
        bool userExists = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            string fileIdnp, fileName;
            int fileVisits;
            double fileTotalSpent;
            ss >> fileIdnp >> fileName >> fileVisits >> fileTotalSpent;
            if (fileIdnp == idnp) {
                line = idnp + " " + name + " " + to_string(visits) + " " + to_string(totalSpent);
                userExists = true;
            }
            lines.push_back(line);
        }
        infile.close();

        if (!userExists) {
            line = idnp + " " + name + " " + to_string(visits) + " " + to_string(totalSpent);
            lines.push_back(line);
        }

        ofstream outfile("users.txt");
        for (const auto& ln : lines) {
            outfile << ln << endl;
        }
        outfile.close();

        if (totalSpent > 500000) {
            ofstream inviteFile("invitatie.txt");
            inviteFile << "Drag client " << name << ",\n\n"
                      << "Va multumim pentru achizitiile dvs. impresionante in valoare de peste 500,000 lei!\n"
                      << "Va invitam la un eveniment special dedicat clientilor nostri de top.\n\n"
                      << "Echipa noastra va doreste o zi buna,\n" << name << "\n";
            inviteFile.close();
            cout << "\nInvitație VIP generată pentru clientul " << name << ".\n";
        }
    }

    bool eligibleForDiscount() const {
        return totalSpent > 4000 && visits > 5;
    }

    // Operator overloading << pentru afisarea datelor despre utilizator
    friend ostream& operator<<(ostream& os, const User& user) {
        os << "Utilizator: " << user.name << ", IDNP: " << user.idnp << ", Vizite: " << user.visits
           << ", Total Cheltuit: " << fixed << setprecision(2) << user.totalSpent << " lei";
        return os;
    }

    // Operator overloading ++ pentru incrementarea numarului de vizite
    User& operator++() {
        visits++;
        return *this;
    }
};

void displayMenu() {
    cout << "\n--- Meniu Magazin de Lampi Electrice ---\n";
    cout << "1. Vizualizare produse\n";
    cout << "2. Adaugare produs in cos\n";
    cout << "3. Vizualizare cos\n";
    cout << "4. Finalizare comanda (Plata cu cardul)\n";
    cout << "5. Iesire\n";
    cout << "Selectati o optiune: ";
}

bool processPayment(double totalAmount) {
    string cardNumber, expiryDate, cvv;
    cout << "Introduceti numarul cardului (16 cifre): ";
    cin >> cardNumber;
    cout << "Introduceti data expirarii (LL/AA): ";
    cin >> expiryDate;
    cout << "Introduceti CVV: ";
    cin >> cvv;

    if (cardNumber.length() == 16 && expiryDate.length() == 5 && cvv.length() == 3) {
        cout << "\nPlata de " << fixed << setprecision(2) << totalAmount << " lei efectuata cu succes.\n";
        return true;
    } else {
        cout << "Date card invalide.\n";
        return false;
    }
}

int main() {
    vector<TableLamp> tableLamps = {
        TableLamp("Veioza_Modern", 299.99, 10),
        TableLamp("Veioza_Clasic", 249.99, 10),
        TableLamp("Veioza_Birou_LED", 399.99, 10),
        TableLamp("Veioza_Vintage", 349.99, 10),
        TableLamp("Veioza_Smart", 499.99, 10),
        TableLamp("Veioza_Copii", 199.99, 10),
        TableLamp("Veioza_Dormitor", 279.99, 10),
        TableLamp("Veioza_Studio", 329.99, 10),
        TableLamp("Veioza_Artistica", 449.99, 10),
        TableLamp("Veioza_Minimalist", 289.99, 10)
    };

    vector<CeilingLamp> ceilingLamps = {
        CeilingLamp("Lustra_Cristal", 999.99, 10),
        CeilingLamp("Lustra_Moderna", 799.99, 10),
        CeilingLamp("Candelabru_Elegant", 1299.99, 10),
        CeilingLamp("Plafoniera_LED", 599.99, 10),
        CeilingLamp("Lustra_Industrial", 899.99, 10),
        CeilingLamp("Lustra_Vintage", 849.99, 10),
        CeilingLamp("Lustra_Smart", 1199.99, 10),
        CeilingLamp("Plafoniera_Baie", 399.99, 10),
        CeilingLamp("Lustra_Living", 949.99, 10),
        CeilingLamp("Lustra_Minimalist", 699.99, 10)
    };

    vector<WallLamp> wallLamps = {
        WallLamp("Aplica_Moderna", 199.99, 10),
        WallLamp("Aplica_LED", 249.99, 10),
        WallLamp("Aplica_Clasica", 179.99, 10),
        WallLamp("Aplica_Vintage", 229.99, 10),
        WallLamp("Aplica_Industrial", 259.99, 10),
        WallLamp("Aplica_Exterior", 299.99, 10),
        WallLamp("Aplica_Baie", 189.99, 10),
        WallLamp("Aplica_Oglinda", 219.99, 10),
        WallLamp("Aplica_Smart", 349.99, 10),
        WallLamp("Aplica_Minimalist", 209.99, 10)
    };

    vector<Product*> products;

    for(auto& lamp : tableLamps) {
        products.push_back(&lamp);
    }

    for(auto& lamp : ceilingLamps) {
        products.push_back(&lamp);
    }

    for(auto& lamp : wallLamps) {
        products.push_back(&lamp);
    }

    string name, idnp;
    cout << "Introduceti numele: ";
    getline(cin, name);
    cout << "Introduceti IDNP-ul: ";
    getline(cin, idnp);

    User user(name, idnp);
    user.loadData();

    Cart cart;
    int option;

    bool discountCard = user.eligibleForDiscount();
    if (discountCard) {
        cout << "Felicitari! Va calificati pentru un card de reducere cu 15% pentru cumparaturile viitoare.\n";
    }

    do {
        displayMenu();
        cin >> option;
        switch (option) {
            case 1:
                cout << "\nProduse disponibile:\n\n";
                cout << "VEIOZE:\n";
                for (int i = 0; i < 10; i++) {
                    cout << (i+1) << ". ";
                    products[i]->display();
                }
                cout << "\nLAMPI DE TAVAN:\n";
                for (int i = 10; i < 20; i++) {
                    cout << (i+1) << ". ";
                    products[i]->display();
                }
                cout << "\nLAMPI DE PERETE:\n";
                for (int i = 20; i < 30; i++) {
                    cout << (i+1) << ". ";
                    products[i]->display();
                }
                break;

            case 2: {
                int choice, quantity;
                cout << "Introduceti numarul produsului (1-30): ";
                cin >> choice;
                if (choice < 1 || choice > 30) {
                    cout << "Optiune invalida.\n";
                    break;
                }
                cout << "Introduceti cantitatea: ";
                cin >> quantity;
                if (quantity <= 0) {
                    cout << "Cantitatea trebuie sa fie pozitiva.\n";
                    break;
                }

                Product* selectedProduct = products[choice - 1];
                if (selectedProduct->getStock() < quantity) {
                    cout << "Stoc insuficient pentru produsul ales.\n";
                } else {
                    selectedProduct->operator-=(quantity);  // Chemarea operatorului supraincarcat -=
                    CartItem item(selectedProduct, quantity);
                    cart += item;  // Chemarea operatorului supraincarcat +=
                    cout << "Produsul a fost adaugat in cos.\n";
                }
                break;
            }
            case 3:
                cout << cart;  // Chemarea operatorului supraincarcat <<
                break;


            case 4: {
                if (cart.getTotal() == 0) {
                    cout << "Cosul este gol. Adaugati produse inainte de a finaliza comanda.\n";
                    break;
                }

                string promoCode;
                bool promoApplied = false;
                cout << "Doriti sa introduceti un cod promotional? (da/nu): ";
                string response;
                cin >> response;

                if (response == "da") {
                    cout << "Introduceti codul promotional: ";
                    cin >> promoCode;
                    // Verificăm dacă codul promoțional este valid (în acest caz "PROMO2024")
                    if (promoCode == "PROMO2024") {
                        cart.applyDiscount(0.03); // aplicăm reducerea de 3%
                        promoApplied = true;
                        cout << "Cod promotional valid! S-a aplicat o reducere de 3%.\n";
                    } else {
                        cout << "Cod promotional invalid.\n";
                    }
                }

                if (discountCard && !promoApplied) {
                    cart.applyDiscount(0.15);
                    cout << "S-a aplicat o reducere de 15% pentru cardul de fidelitate.\n";
                } else if (discountCard && promoApplied) {
                    cart.applyDiscount(0.15);
                    cout << "S-a aplicat reducerea de 15% pentru cardul de fidelitate (după reducerea promoțională).\n";
                }

                cout << "Total de plata: " << fixed << setprecision(2) << cart.getTotal() << " lei\n";

                if (processPayment(cart.getTotal())) {
                    user.updateData(cart.getTotal());
                    cout << "Comanda a fost finalizata cu succes.\n";

                    if (cart.getTotal() >= 2000) {
                        cout << "Livrare gratuita! A-ti cumparat la o suma > 2000 lei.\n";
                    }

                    cart.clearCart();
                } else {
                    cout << "Plata a esuat. Incercati din nou.\n";
                }
            }
            break;


            case 5:
                cout << "Multumim pentru vizita!\n";
            break;


            default:
                cout << "Optiune invalida. Va rugam sa selectati o optiune valida.\n";
            break;
        }
    } while (option != 5);

    return 0;
}
