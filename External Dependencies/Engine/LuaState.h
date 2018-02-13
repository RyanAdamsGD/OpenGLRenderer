#ifndef LUASTATE_H
#define LUASTATE_H
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

class LuaState
{
private:
    lua_State *L;

public:
	LuaState(const char* fileName)
	{
		L = lua_open();
		luaL_loadfile(L,fileName);
		lua_pcall(L,0,0,0);
	}
	~LuaState()
	{
		lua_close(L);
	}
    // get function
    template<typename T>
    T get(const char * varname) 
	{
      char temp[64];
      memset(temp, 0, sizeof(temp));
      int i=0;
      int j=0;
      int n=0;
      while (varname[i] != '\0') 
	  {
        char c = varname[i];
        if (c == '.') 
		{
          if (n == 0)
            lua_getglobal(L, temp);
          else
            lua_getfield(L, -1, temp);

          ++n;
          memset(temp, 0, sizeof(temp));
          j = 0;

          if (lua_isnil(L, -1))
            return 0;
        }
        else 
		{
          temp[j] = c;
          ++j;
        }
        ++i;
      }
      if (n == 0)
        lua_getglobal(L, temp);
      else
        lua_getfield(L, -1, temp);
      return lua_get<T>();
    }

    // Generic get
    template<typename T>
    T lua_get() {
      return 0;
    }

    // Specializations
    template <> float lua_get<float>() {
      return lua_tonumber(L, -1);
    }
    template <> double lua_get<double>() {
      return lua_tonumber(L, -1);
    }
    template <> bool lua_get<bool>() {
      return lua_toboolean(L, -1);
    }
    template <> int lua_get<int>() {
      return lua_tointeger(L, -1);
    }
    template <> unsigned int lua_get<unsigned int>() {
      return (unsigned int)lua_tonumber(L, -1);
    }
    template <> const char * lua_get<const char *>() {
      return lua_tostring(L, -1);
    }
};

#endif