#include "TypeEnvironment.h"

TypeEnvironment::~TypeEnvironment()
{
    for (auto it = types.begin(); it != types.end(); ++it)
    {
        delete it->second;
    }
}

Type *TypeEnvironment::getType(const std::string &typeName)
{
    if (types.find(typeName) == types.end())
    {
        throw TypeException("Type " + typeName + " unexisted.");
    }

    return types[typeName];
}

void TypeEnvironment::insertType(const std::string &typeName, Type *type)
{
    if (types.find(typeName) != types.end())
    {
        throw TypeException("Type " + typeName + "redefind.");
    }

    types.insert(make_pair(typeName, type));
}

TypeEnvironment::TypeEnvironment()
{
    Type *intType = new Type("int", ObjectType::INT);
    Type *doubleType = new Type("double", ObjectType::DOUBLE);
    Type *stringType = new Type("string", ObjectType::STRING);
    Type *voidType = new Type("void", ObjectType::VOID);

    insertType("int", intType);
    insertType("double", doubleType);
    insertType("string", stringType);
    insertType("void", voidType);

#ifdef DEBUG
    Type *testInt = getType("int");
    Type *testDouble = getType("double");
#endif
}

bool TypeEnvironment::contains(const std::string &typeName) const
{
    return types.find(typeName) != types.end();
}