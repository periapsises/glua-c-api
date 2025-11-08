#pragma once

#include <stddef.h>
#include <type_traits>

#include "types.h"
#include "math.h"

enum {
    SPECIAL_GLOB,
    SPECIAL_ENV,
    SPECIAL_REG
};

struct lua_State;

typedef int (*lua_CFunction)(lua_State* L);

class lua_Base
{
public:
    struct UserData
    {
        void* data;
        unsigned char type;
    };

protected:
    template <class T>
    struct UserData_Value: UserData
    {
        T value;
    };

public:
    // Returns the amount of values on the stack.
    virtual int Top() = 0;

    // Pushes a copy of the value at the given position onto the stack.
    virtual void Push(int position) = 0;

    // Removes the given amount of values from the stack.
    virtual void Pop(int amount) = 0;

    // Creates a new table and pushes it onto the stack.
    virtual void CreateTable() = 0;

    // Pushes table[key] onto the stack.
    // table = value at position
    // key   = value on top of the stack
    // Pops the key from the stack.
    virtual void GetTable(int position) = 0;

    // Sets table[key] to the value at the top of the stack.
    // table = value at position
    // key   = value second from top of the stack
    // Pops the key and the value from the stack.
    virtual void SetTable(int position) = 0;

    // Pushes table[key] onto the stack.
    // table = value at position
    // key   = value second from top of the stack
    virtual void GetField(int position, const char* key) = 0;

    // Sets table[key] to the value at the second from top of the stack.
    // table = value at position
    // key   = key argument
    // Pops the key and the value from the stack.
    virtual void SetField(int position, const char* key) = 0;

    // Pushes the metatable associated with the given type name.
    // Returns the type index to use for this type.
    // If the type does not already exist, it will be created.
    virtual int CreateMetaTable(const char* name) = 0;

    // Pushes the metatable of the associated type onto the stack.
    virtual bool PushMetaTable(int type) = 0;

    // Pushes the metatable of the value at the given position onto the stack.
    // Upon failure, returns false and no value is pushed.
    virtual bool GetMetaTable(int position) = 0;

    // Sets the metatable of the value at the given position to the metatable on top of the stack.
    // Pops the metatable from the stack.
    virtual void SetMetaTable(int position) = 0;

    // Calls a function from the stack.
    // To call a function, you must push the function on the stack followed by the arguments.
    // Pops the function and the arguments from the stack and pushes the results back onto the stack.
    // WARNING: If this function fails, any local C values will not have their destructors called!
    virtual void Call(int numArgs, int numResults) = 0;

    // Calls a function from the stack in protected mode.
    // To call a function, you must push the function on the stack followed by the arguments.
    // Pops the function and the arguments from the stack and pushes the results back onto the stack.
    // If the function fails, the error is caught and the error message pushed onto the stack.
    // If the error function is not 0, `errorFunction` is the index of a handler function on the stack that will be called with the error message and whose return value is pushed onto the stack instead.
    virtual int PCall(int numArgs, int numResults, int errorFunction) = 0;

    // Compares two values on the stack.
    virtual int Equal(int position1, int position2) = 0;

    // Compares two values on the stack without calling the __eq metamethod.
    virtual int RawEqual(int position1, int position2) = 0;

    // Moves the value at the top of the stack to the given position.
    // Any elements above the moved value are shifted up.
    virtual void Insert(int position) = 0;

    // Removes the value at the given position in the stack.
    // Any elements above the removed value are shifted down.
    virtual void Remove(int position) = 0;

    // Pops a key from the stack and pushes a key-value pair from the table at the given position onto the stack.
    // If there are no more elements in the table, returns 0 and pushes nothing.
    virtual int Next(int position) = 0;

    // Throws an error and ceases execution of the function.
    // WARNING: Any local C values will not have their destructors called!
    [[noreturn]]
    virtual void ThrowError(const char* message) = 0;

    // Checks if the type of the value at the given position is the same as the given type.
    // If the type is not the same, throws an error and ceases execution of the function.
    // WARNING: Any local C values will not have their destructors called!
    virtual void CheckType(int position, int type) = 0;

    // Throws a formatted error message about the given argument.
    // WARNING: Any local C values will not have their destructors called!
    [[noreturn]]
    virtual void ArgError(int argPosition, const char* message) = 0;

    // Pushed table[key] onto the stack without calling the __index metamethod.
    // table = value at position
    // key   = value on top of the stack
    // Pops the key from the stack.
    virtual void RawGet(int position) = 0;

    // Sets table[key] to the second value from top of the stack without calling the __newindex metamethod.
    // table = value at position
    // key   = value second from top of the stack
    // Pops the key and the value from the stack.
    virtual void RawSet(int position) = 0;

    // Returns the string value at the given position.
    // If size is not NULL, it is set to the length of the string.
    // If the value is a number, it will be converted into a string.
    // Returns NULL if the value is not a string.
    virtual const char* GetString(int position, unsigned int* size = NULL) = 0;

    // Returns the number value at the given position.
    // Returns 0 if the value is not a number.
    virtual double GetNumber(int position) = 0;

    // Returns the boolean value at the given position.
    // Returns false if the value is not a boolean.
    virtual bool GetBool(int position) = 0;

    // Returns the C function value at the given position.
    // Returns NULL if the value is not a C-Function.
    virtual lua_CFunction GetCFunction(int position) = 0;

    // Pushes a nil value onto the stack.
    virtual void PushNil() = 0;

    // Pushes a string value onto the stack.
    // If size is 0, `strlen` will be used to determine the length of the string.
    virtual void PushString(const char* str, unsigned int size = 0) = 0;

    // Pushes a number value onto the stack.
    virtual void PushNumber(double number) = 0;

    // Pushes a boolean value onto the stack.
    virtual void PushBool(bool boolean) = 0;

    // Pushes a C-Function value onto the stack.
    virtual void PushCFunction(lua_CFunction function) = 0;

    // Pushes a C-Function with upvalues onto the stack.
    virtual void PushCClosure(lua_CFunction function, int numUpvalues) = 0;

#define GMOD_REFERENCE_NIL 0

    // Allows to store values by reference for later use.
    // WARNING: Make sure to call ReferenceFree when you are done with the reference!
    virtual int ReferenceCreate() = 0;
    virtual void ReferenceFree(int reference) = 0;
    virtual void ReferencePush(int reference) = 0;

    // Pushes a special value onto the stack.
    // specialType can be SPECIAL_GLOB, SPECIAL_ENV, or SPECIAL_REG.
    virtual void PushSpecial(int specialType) = 0;

    // Checks if the type of the value at the given position is the same as the given type.
    virtual bool IsType(int position, int type) = 0;

    // Returns the type of the value at the given position.
    virtual int GetType(int position) = 0;

    // Returns the name of the given type.
    // NOTE: Does not work with user-created types.
    virtual const char* GetTypeName(int type) = 0;

    // Checks if the value at the given position is a string.
    // If not, throws an error and ceases execution of the function.
    // WARNING: Any local C values will not have their destructors called!
    virtual const char* CheckString(int position) = 0;

    // Checks if the value at the given position is a number.
    // If not, throws an error and ceases execution of the function.
    // WARNING: Any local C values will not have their destructors called!
    virtual double CheckNumber(int position) = 0;

    // Returns the length of the value at the given position.
    // Only works for string, tables and userdata.
    virtual int ObjLen(int position) = 0;

    // Returns the vector value at the given position.
    virtual const Vector& GetVector(int position) = 0;

    // Pushes a vector value onto the stack.
    virtual void PushVector(const Vector& vector) = 0;

    // Returns the angle value at the given position.
    virtual const QAngle& GetAngle(int position) = 0;

    // Pushes an angle value onto the stack.
    virtual void PushAngle(const QAngle& angle) = 0;

    // Sets the `lua_State` to be used by the `lua_Base` instance.
    // This is only necessary for C-Functions that need to access the `lua_State`.
    virtual void SetState(lua_State* L) = 0;

protected:
    virtual void* NewUserdata(unsigned int size) = 0;

public:
    // Creates a new UserType of the given type and pushes it onto the stack.
    virtual void PushUserType(void* data, int type) = 0;

    // Sets the data of the UserType at the given position.
    // Setting the data to NULL will invalidate the UserType.
    virtual void SetUserType(int position, void* data) = 0;
    
    // Returns the data of the UserType at the given position.
    // Returns NULL if the UserType is invalid or not of the given type.
    template <class T>
    T* GetUserType(int position, int type)
    {
        auto* userData = static_cast<UserData*>(GetUserData(position));

        if (userData == nullptr || userData->type == nullptr || userData->type != type)
            return nullptr;

        return static_cast<T*>(userData->data);
    }


    // Creates a new UserType with the given value and pushes it onto the stack.
    template <class T>
    void PushUserType_Value(const T& value, int type)
    {
        using UserData_T = UserData_Value<T>;

        static_assert(std::alignment_of<UserData_T>::value <= 8, "PushUserType_Value: Alignment of value is too large");
        static_assert(std::is_trivially_destructible<UserData_T>::value, "PushUserType_Value: Value is not trivially destructible");

        auto* userData = static_cast<UserData_T*>(NewUserdata(sizeof(UserData_T)));
        userData->data = new(&userData->value) T(value);
        userData->type = type;

        if (PushMetaTable(type))
            SetMetaTable(-2);
    }
};
