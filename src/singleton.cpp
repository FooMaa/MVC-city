#include "headers/singleton.h"

int General::counter_names_city = 1;

Singleton &Singleton::get_instance()
{
    static Singleton instance;
    return instance;
}

Singleton::Singleton()
{
   City *city = new City(*this);
   makeDefaultTree<City>(city);
}

City::City(Singleton &parent)
{
    name_object = "City" + QString::number(counter_names_city);
    counter_names_city++;
    setObjectName(name_object);
    setParent(&parent);
    generateProperty(this, 0);
}

Street::Street(City *parent)
{
    name_object = "Street" + QString::number(parent->counter_names[0]);
    parent->counter_names[0]++;
    setObjectName(name_object);

    if (parent != nullptr)
    {
       this->setParent(parent);
    }

    generateProperty(this, 1);
}

Home::Home(Street *parent)
{
    name_object = "Home" + QString::number(parent->counter_names[1]);
    parent->counter_names[1]++;
    setObjectName(name_object);

    if (parent != nullptr)
    {
       setParent(parent);
    }

    generateProperty(this, 2);
}

People::People(Home *parent)
{
    name_object= "People"+QString::number(parent->counter_names[2]);
    parent->counter_names[2]++;
    setObjectName(name_object);

    if (parent != nullptr)
    {
       this->setParent(parent);
    }

    generateProperty(this, 3);
}

General::General()
{
    class_property[0] << "For live" << "For work" << "For relax";
    class_property[1] << "Long" << "Short" << "Middle";
    class_property[2] << "Gray" << "Black" << "White" << "Blue" << "Green";
    class_property[3] << "Clever" << "Atletic" << "Beautiful";
}
