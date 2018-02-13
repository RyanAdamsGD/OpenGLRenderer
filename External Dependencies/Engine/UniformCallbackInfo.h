#ifndef UNIFORM_CALLBACK_INFO_H
#define UNIFORM_CALLBACK_INFO_H
#include "MyTypeDefs.h"
class UniformCallbackInfo;
typedef void (*glUniformCallback)(const UniformCallbackInfo* uniformCallback);

class UniformCallbackInfo
{
	#pragma region
		static void uniformCallback1i(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback2i(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback3i(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback4i(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback1ui(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback2ui(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback3ui(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback4ui(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback1f(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback2f(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback3f(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback4f(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback2m(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback3m(const UniformCallbackInfo* uniformCallback);
		static void uniformCallback4m(const UniformCallbackInfo* uniformCallback);
		static void uniformCallbackTexture(const UniformCallbackInfo* uniformCallback);
#pragma endregion

		static glUniformCallback glUniformCallbacks[NUM_UNIFORM_CALLBACKS];
		uint programID;
		const void* data;
		uint location;
		DataType type;
		const char* variableName;
public:
		UniformCallbackInfo(uint programID, const void* data, const char* variableName, const DataType& type);
		///bind the data to the appropriate uniform location
		void activateUniform() const;

		void setData(const void* value) { data = value; }

		uint getProgramID() const { return programID; }
		const void* getData() const { return data; }
		const char* getVariableName() const { return variableName; }
};

#endif