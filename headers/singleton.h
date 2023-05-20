#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>
#include <QMap>

class Singleton : public QObject
{
public:
    static Singleton& get_instance();
    

private:
    Singleton();
    Singleton (Singleton const&) = delete;
    Singleton (Singleton const&&) = delete;
    Singleton& operator= (Singleton const) = delete;
    Singleton& operator= (Singleton const&&) = delete;
    ~Singleton() {};
};

class General : public QObject
{
public:
   QString name_object = "";
   QMap <int, QStringList> class_property;
   QString generated_property;
   
   General();
   template <typename OBJECT> void generateProperty(OBJECT *object, int key);
   
   int counter_names[4] = {1,1,1,1};
   static int counter_names_city;
};

class City: public General
{
public:
   City(Singleton &parent);
};

class Street: public General{
public:
   Street(City *parent = nullptr);
};

class Home: public General
{
public:
    Home (Street *parent = nullptr);
};

class People: public General
{
public:
    People(Home *parent = nullptr);
};

template <typename PARENT, typename CHILD>
CHILD* generateDefaultObjects(PARENT *parent_object, int amount_object)
{
    CHILD *objects[amount_object];
    
    for (int i = 0; i < amount_object; i++)
    {
        objects[i] = new CHILD(parent_object);
    }
    
    return objects[0];
}

template <typename PARENT>
void makeDefaultTree(PARENT *parent_object)
{
    Street *main_street = generateDefaultObjects<City, Street>(parent_object, 3);
    Home *main_home = generateDefaultObjects<Street, Home>(main_street, 3);
    People *main_people = generateDefaultObjects<Home, People>(main_home, 2);

    Q_UNUSED(main_people);
}

template <typename OBJECT>
void General::generateProperty(OBJECT *object, int key)
{
    srand(time(0));
    int random_key = rand() % 3;
    object->generated_property = class_property[key][random_key];
}

#endif // SINGLETON_H
