#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

#include <lua.hpp>
#include <memory>

class LuaInstance
{
public:
	typedef std::shared_ptr<LuaInstance> Ptr;
	typedef std::weak_ptr<LuaInstance> WeakPtr;

public:
	LuaInstance();
	LuaInstance(lua_State *luaState);
	virtual ~LuaInstance();

	static LuaInstance::Ptr Create();

	lua_State *getLuaState();

	void registerLibs(const luaL_Reg lualibs[]);
	
	int execScript(const char* script);
	int execScriptFromFile(const char* scriptFileName);

	template<typename T>
	void registerConstant(const char* constantname, T value);
	void registerFunction(const char* functionName, lua_CFunction function);
	
	int getArgumentsCount();

	void beginInitializeTable();
	template<typename T1, typename T2>
	void registerTableConstant(T1 key, T2 value);
	void registerTableFunction(const char* functionName, lua_CFunction function);
	void endInitializeTable(const char* tableName);
	void endInitializeSubtable(const char* tableName);

	void removeSymbol(const char* symbolName);

	template<typename T>
	T getFromLua(int index);
	template<typename T>
	void sendToLua(T value);
private:
	lua_State *luaState;
	bool isMainInstance;

	LuaInstance(const LuaInstance&);
	void operator=(const LuaInstance&);
};

template<typename T>
void LuaInstance::registerConstant(const char* constantName, T value)
{
	this->sendToLua<T>(value);
	lua_setglobal(this->luaState, constantName);
}

template<typename T1, typename T2>
void LuaInstance::registerTableConstant(T1 key, T2 value)
{
	this->sendToLua<T1>(key);
	this->sendToLua<T2>(value);
	lua_settable(this->luaState, -3);
}

#endif