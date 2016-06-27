#define tower_cost 10
#define tower_damage 15
#define tower_fire_rate 100
#define tower_range 50


/*
Classe Torre(Apenas um tipo - prototipo):
    Definida pelos:
======> Atributos:
            * Area de ataque = attackRange  
            * Dano = damage 
            * Cadencia de tiro = firerate
            * Custo = cost

=======> Funcoes:
            * Setar Custo = setcost (OK) 
            * Setar Dano = setdamage (OK)
            * Setar Cadencia de tiro = setrate
            * Setar Area de ataque = setrange
            * Pegar Custo = getcost
            * Pegar dano = getdam
            * Pegar cadencia = getrate
            * Pegar area = getrange
*/
class Tower

{
    public:
        // Setar valores
        void setcost();
        void setdamage();
        void setrate();
        void setrange();
        // Pegar valores
        int getcost();
        int getdamage();
        double getrate();
        int getrange();
    private:
        int attackRange;
        int damage;
        double firerate;
        int cost;
};

// Leia a descrição, por favor

void Tower::setcost()
{
    this->cost = tower_cost;
};

void Tower::setdamage()
{
    this->damage = tower_damage;
};

void Tower::setrate()
{
    this->firerate = tower_fire_rate;
};

void Tower::setrange()
{
    this->attackRange = tower_range;
};

// Leia a descrição, amiguinho :^)

double Tower::getrate()
{
    return firerate;
};

int Tower::getrange()
{
    return attackRange;
};

int Tower::getdamage()
{
    return damage;
};

int Tower::getcost()
{
    return cost;
};